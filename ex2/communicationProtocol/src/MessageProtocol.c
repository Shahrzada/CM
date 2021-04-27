
// ------------------------------ includes ------------------------------

#include <base64.h>
#include "MessageProtocol.h"
#include "CommunicationMethods.h"

// ------------------------------ global variables ------------------------------

static server_communication_method *serverCMethod = NULL;
static client_communication_method *clientCMethod = NULL;

// ------------------------------ private functions -----------------------------

static char *MPMessageToEncodedBytes(Message *msg)
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

static Message *MPEncodedBytesToMsg(char *encodedMsg)
{
    CHECK_NULL_RETURN_NULL(encodedMsg);

    // decode the data
    char *decodedMsg = (char *) malloc(sizeof(char) * MAX_MSG_LENGTH);
    CHECK_NULL_RETURN_NULL(decodedMsg);

    unsigned int msgLength = Base64decode(decodedMsg, encodedMsg);
    if (msgLength == 0)
        goto cleanup;

    // create the msg object
    Message *msg = messageFromCString(decodedMsg, msgLength);
    CHECK_NULL_GOTO_CLEANUP(msg);

    free(decodedMsg);
    return msg;

    cleanup:
    free(decodedMsg);
    return NULL;
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
        MPClientSend(msg);
        free(msg);
    }

cleanup:
    connectionResult = clientCMethod->clientCloseConnectionFunction();
    free(clientCMethod);
    clientCMethod = NULL;
    return connectionResult;
}

Message *MPClientReceive()
{
    CHECK_NULL_RETURN_NULL(clientCMethod);
    char *encodedMsg = clientCMethod->clientReceiveFunction();

    if (encodedMsg == NULL)
        return NULL;

    // decode it and create a msg object for the client
    Message *msg = MPEncodedBytesToMsg(encodedMsg);
    free(encodedMsg);
    MSG_CHECK_VALID_RETURN_NULL(msg);

    printf("Client received:\n");
    PRINT_MSG(msg);
    return msg;
}

Message *MPServerListen()
{
    CHECK_NULL_RETURN_NULL(serverCMethod);

    char *encodedMsg = serverCMethod->listenFunction();
    if (encodedMsg == NULL)
        return NULL;

    // decode it and create a msg object for the client
    Message *msg = MPEncodedBytesToMsg(encodedMsg);
    free(encodedMsg);
    CHECK_NULL_RETURN_NULL(msg);

    printf("Client received:\n");
    PRINT_MSG(msg);
    return msg;
}

ReturnValue MPServerSend(Message *msg)
{
    CHECK_NULL_RETURN_ERROR(serverCMethod);
    if (!messageValidateFormat(msg))
        return PROJECT_ERROR;

    printf("Server is sending:\n");
    PRINT_MSG(msg);

    char *encodedMsg = MPMessageToEncodedBytes(msg);
    return serverCMethod->sendFunction(encodedMsg);
}

Message *MPClientSend(Message *msg)
{
    CHECK_NULL_RETURN_NULL(clientCMethod);
    if (!messageValidateFormat(msg))
        return NULL;

    printf("Client is sending:\n");
    PRINT_MSG(msg);

    // encode the msg and send it
    char *encodedMsg = MPMessageToEncodedBytes(msg);
    CHECK_NULL_RETURN_NULL(encodedMsg);

    // wait for the encoded reply
    char *encodedReply = clientCMethod->sendFunction(encodedMsg);
    free(encodedMsg);
    CHECK_NULL_RETURN_NULL(encodedReply);

    // decode it and create a msg object for the client
    Message *reply = MPEncodedBytesToMsg(encodedReply);
    free(encodedReply);
    CHECK_NULL_RETURN_NULL(reply);

    printf("Client received:\n");
    PRINT_MSG(reply);
    return reply;
}

void MPServerSendSuccessOrFailure(ReturnValue result)
{

    char *contents = (result == PROJECT_SUCCESS) ? "SUCCESS" : "FAILURE";
    Message *msg = messageSet(SERVER, REPLY, 7, contents);
    MPServerSend(msg);
    free(msg);
}
