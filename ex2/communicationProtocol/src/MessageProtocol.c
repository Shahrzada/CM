
// ------------------------------ includes ------------------------------

#include <base64.h>
#include <config.h>
#include "MessageProtocol.h"
#include "CommunicationMethods.h"

// ------------------------------ global variables ------------------------------

#define IS_SERVER (serverCMethod == NULL)

static server_communication_method *serverCMethod = NULL;
static client_communication_method *clientCMethod = NULL;

// ------------------------------ private functions -----------------------------

static char *MPGetLogFilePath()
{
    return (IS_SERVER) ? getServerLogCommunicationFilePath() : getClientLogCommunicationFilePath();
}

static ReturnValue MPWriteToLog(Message *msg)
{
    CHECK_NULL_RETURN_ERROR(msg);
    ReturnValue result = PROJECT_ERROR;

    char *logPath = MPGetLogFilePath();
    CHECK_NULL_RETURN_ERROR(logPath);

    // Open log file for appending
    FILE *pFile = fopen(logPath, FILE_APPEND_MODE);
    CHECK_NULL_RETURN_ERROR(pFile);

    // Get the formatted string out of the msg
    size_t bufferSize = msg->contentsLength + MSG_PRINT_FORMAT_LENGTH;
    char buffer[bufferSize + NULL_CHAR_SIZE];
    result = messageToPrintableCString(msg, buffer);
    CHECK_ERROR_GOTO_CLEANUP(result);

    // Write it to the log
    size_t totalBytesWritten = fwrite(buffer, 1, bufferSize, pFile);
    if (totalBytesWritten != bufferSize)
        goto cleanup;

    result = PROJECT_SUCCESS;

    cleanup:
    fclose(pFile);
    return result;
}

static Message *MPDecodeAndPrint(char *encodedMsg)
{
    CHECK_NULL_RETURN_NULL(encodedMsg);

    Message *msg = messageDecodeStringIntoMsg(encodedMsg, Base64decode);
    MSG_CHECK_VALID_RETURN_NULL(msg);

    (IS_SERVER) ? printf("\nServer received:\n") : printf("\nClient received:\n");
    PRINT_MSG(msg);

    MPWriteToLog(msg);

    return msg;
}

static char *MPPrintAndEncode(Message *msg)
{
    CHECK_NULL_RETURN_NULL(msg);

    (IS_SERVER) ? printf("\nServer is sending:\n") : printf("\nClient is sending:\n");
    PRINT_MSG(msg);

    MPWriteToLog(msg);

    return messageIntoEncodedString(msg, Base64encode);
}

// ------------------------------ functions -----------------------------

ReturnValue MPServerInitConnection(CommunicationMethodCode cMethodCode)
{
    if (serverCMethod == NULL)
        serverCMethod = serverCMethodSet(cMethodCode);
    CHECK_NULL_RETURN_ERROR(serverCMethod);
    return serverCMethod->serverInitConnectionFunction();
}

ReturnValue MPServerCloseConnection()
{
    CHECK_NULL_RETURN_ERROR(serverCMethod);
    ReturnValue result = serverCMethod->serverCloseConnectionFunction();
    free(serverCMethod);
    serverCMethod = NULL;
    return result;
}

Message *MPServerListen()
{
    CHECK_NULL_RETURN_NULL(serverCMethod);

    char encodedMsg[MAX_MSG_LENGTH];
    unsigned int encodedMsgLength = serverCMethod->listenFunction(encodedMsg);
    CHECK_ZERO_RETURN_NULL(encodedMsgLength);

    return MPDecodeAndPrint(encodedMsg);
}

ReturnValue MPServerSend(Message *msg)
{
    CHECK_NULL_RETURN_ERROR(serverCMethod);
    MSG_CHECK_VALID_RETURN_ERROR(msg);
    char *encodedMsg = NULL;

    encodedMsg = MPPrintAndEncode(msg);
    CHECK_NULL_RETURN_ERROR(encodedMsg);

    unsigned int encodedMsgLength = strnlen(encodedMsg, MAX_MSG_LENGTH);
    if (encodedMsgLength == 0 || encodedMsgLength == MAX_MSG_LENGTH)
    {
        free(encodedMsg);
        PRINT_ERROR_WITH_FUNCTION_AND_RETURN_ERROR("MPServerSend", "Bad encoded msg");
    }

    ReturnValue result = serverCMethod->sendFunction(encodedMsg, encodedMsgLength);
    free(encodedMsg);
    return result;
}

ReturnValue MPServerSendSuccessOrFailure(ReturnValue result)
{
    Message *msg = messageSetSuccessOrFailure(SERVER, REPLY, (result == PROJECT_SUCCESS));
    CHECK_NULL_RETURN_ERROR(msg);
    ReturnValue sendResult = MPServerSend(msg);
    free(msg);
    return sendResult;
}

ReturnValue MPClientInitConnection(CommunicationMethodCode cMethodCode)
{
    if (clientCMethod == NULL)
        clientCMethod = clientCMethodSet(cMethodCode);
    CHECK_NULL_RETURN_ERROR(clientCMethod);
    return clientCMethod->clientInitConnectionFunction();
}

ReturnValue MPClientCloseConnection(ReturnValue result)
{
    CHECK_NULL_RETURN_ERROR(clientCMethod);
    ReturnValue closingResult = PROJECT_ERROR;

    if (result == PROJECT_ERROR)  // send the server an ABORT msg
    {
        Message *msg = messageSet(CLIENT, ABORT, 0, EMPTY_STRING);
        MSG_CHECK_VALID_GOTO_CLEANUP(msg);
        Message *reply = MPClientSend(msg);
        messageFree(msg);
        messageFree(reply); // ignoring reply due to closing because of an error
    }

    cleanup:
    closingResult = clientCMethod->clientCloseConnectionFunction();
    free(clientCMethod);
    clientCMethod = NULL;
    return closingResult;
}

Message *MPClientSend(Message *msg)
{
    CHECK_NULL_RETURN_NULL(clientCMethod);
    MSG_CHECK_VALID_RETURN_NULL(msg);

    // Encode the msg
    char *encodedMsg = MPPrintAndEncode(msg);
    CHECK_NULL_RETURN_NULL(encodedMsg);

    unsigned int encodedMsgLength = strnlen(encodedMsg, MAX_MSG_LENGTH);
    if (encodedMsgLength == MAX_MSG_LENGTH) // CR: if message legnth is MAX_MSG_LENGTH that means you exceed your buffer...
    {
        free(encodedMsg);
        PRINT_ERROR_WITH_FUNCTION_AND_RETURN_NULL("MPClientSend", "Bad encoded msg");
    }

    // Send it and wait for a reply
    char encodedReply[MAX_MSG_LENGTH];
    unsigned int encodedReplyLength = clientCMethod->sendFunction(encodedMsg, encodedMsgLength, encodedReply);
    free(encodedMsg);
    if (encodedReplyLength == 0)
        return NULL;

    // Decode the reply and create a msg object for the client
    return MPDecodeAndPrint(encodedReply);
}
