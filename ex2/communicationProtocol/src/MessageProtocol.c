
// ------------------------------ includes ------------------------------

#include <base64.h>
#include "MessageProtocol.h"
#include "CommunicationMethods.h"

// ------------------------------ macros ------------------------------

#define MP_SUCCESS_MSG "Success"
#define MP_FAILURE_MSG "Failure"
#define MP_SUCCESS_FAILURE_MSG_LENGTH 7

// ------------------------------ global variables ------------------------------

static server_communication_method *serverCMethod = NULL;
static client_communication_method *clientCMethod = NULL;

// ------------------------------ private functions -----------------------------

static char *MPMessageIntoEncodedString(Message *msg)
{
    MSG_CHECK_VALID_RETURN_NULL(msg);

    // copy into char array
    unsigned int msgStrLength = 0;
    char *msgStr = messageToCString(msg, &msgStrLength);
    CHECK_NULL_RETURN_NULL(msgStr);


    // Encode le msg
    char *encodedMsg = (char *) malloc(MAX_MSG_LENGTH);
    CHECK_NULL_RETURN_NULL(encodedMsg);

    int encodedMsgLength = Base64encode(encodedMsg, msgStr, (int)msgStrLength);
    free(msgStr);
    if (encodedMsgLength == 0)
    {
        free(encodedMsg);
        printf("Error with encoding file...");
        return NULL;
    }

    encodedMsg = realloc(encodedMsg, encodedMsgLength);
    return encodedMsg;
}

static Message *MPDecodeStringIntoMsg(char *encodedMsg)
{
    CHECK_NULL_RETURN_NULL(encodedMsg);
    Message *msg = NULL;

    // decode the data
    char decodedMsg[MAX_MSG_LENGTH];
    unsigned int msgLength = Base64decode(decodedMsg, encodedMsg);
    if (msgLength == 0)
        goto cleanup;

    // create the msg object
    msg = messageFromCString(decodedMsg, msgLength);
    CHECK_NULL_GOTO_CLEANUP(msg);

    cleanup:
    return msg;
}

static Message *MPDecodeAndPrint(char *encodedMsg, bool isServer)
{
    CHECK_NULL_RETURN_NULL(encodedMsg);

    // decode it and create a msg object for the client
    Message *msg = MPDecodeStringIntoMsg(encodedMsg);
    MSG_CHECK_VALID_RETURN_NULL(msg);

    if (isServer)
        printf("\nServer received:\n");
    else
        printf("\nClient received:\n");
    PRINT_MSG(msg);

    return msg;
}

static char *MPPrintAndEncode(Message *msg, bool isServer)
{
    CHECK_NULL_RETURN_NULL(msg);

    if (isServer)
        printf("\nServer is sending:\n");
    else
        printf("\nClient is sending:\n");
    PRINT_MSG(msg);

    return MPMessageIntoEncodedString(msg);
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
    if (encodedMsgLength == 0)
        return NULL;

    // decode it and create a msg object for the client
    return MPDecodeAndPrint(encodedMsg, true);
}

ReturnValue MPServerSend(Message *msg)
{
    CHECK_NULL_RETURN_ERROR(serverCMethod);
    if (!messageValidateFormat(msg))
        return PROJECT_ERROR;

    // encode the msg and send it
    char *encodedMsg = MPPrintAndEncode(msg, true);
    CHECK_NULL_RETURN_ERROR(encodedMsg);

    unsigned int encodedMsgLength = strnlen(encodedMsg, MAX_MSG_LENGTH);
    if (encodedMsgLength == MAX_MSG_LENGTH)
    {
        free(encodedMsg);
        printf("\nError in MPClientSend: Encoded msg is too big...\n");
        return PROJECT_ERROR;
    }

    ReturnValue result = serverCMethod->sendFunction(encodedMsg, encodedMsgLength);
    free(encodedMsg);
    return result;
}


void MPServerSendSuccessOrFailure(ReturnValue result)
{
    Message *msg = messageSet(SERVER,
                              REPLY,
                              MP_SUCCESS_FAILURE_MSG_LENGTH,
                              (result == PROJECT_SUCCESS) ? MP_SUCCESS_MSG : MP_FAILURE_MSG);
    MPServerSend(msg);
    free(msg);
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
    ReturnValue connectionResult = PROJECT_ERROR;

    if (result == PROJECT_ERROR)  // send the server an ABORT msg
    {
        Message *msg = messageSet(CLIENT, ABORT, 0, "");
        MSG_CHECK_VALID_GOTO_CLEANUP(msg);
        Message *reply = MPClientSend(msg);
        free(msg);
        free(reply);
    }

    cleanup:
    connectionResult = clientCMethod->clientCloseConnectionFunction();
    free(clientCMethod);
    clientCMethod = NULL;
    return connectionResult;
}

Message *MPClientSend(Message *msg)
{
    CHECK_NULL_RETURN_NULL(clientCMethod);
    if (!messageValidateFormat(msg))
        return NULL;

    // encode the msg and send it
    char *encodedMsg = MPPrintAndEncode(msg, false);
    CHECK_NULL_RETURN_NULL(encodedMsg);

    unsigned int encodedMsgLength = strnlen(encodedMsg, MAX_MSG_LENGTH);
    if (encodedMsgLength == MAX_MSG_LENGTH)
    {
        free(encodedMsg);
        printf("\nError in MPClientSend: Encoded msg is too big...\n");
        return NULL;
    }

    // wait for a reply
    char encodedReply[MAX_MSG_LENGTH];
    unsigned int encodedReplyLength = clientCMethod->sendFunction(encodedMsg, encodedMsgLength, encodedReply);
    free(encodedMsg);
    if (encodedReplyLength == 0)
        return NULL;

    // decode it and create a msg object for the client
    return MPDecodeAndPrint(encodedReply, false);
}

Message *MPClientReceive()
{
    CHECK_NULL_RETURN_NULL(clientCMethod);
    char encodedMsg[MAX_MSG_LENGTH];
    unsigned int encodedMsgLength = clientCMethod->clientReceiveFunction(encodedMsg);
    if (encodedMsgLength == 0)
        return NULL;

    // decode it and create a msg object for the client
    return MPDecodeAndPrint(encodedMsg, false);
}