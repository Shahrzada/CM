
// ------------------------------ includes ------------------------------

#include "../include/MessageProtocol.h"
#include "../../communicationMethods/include/CommunicationMethods.h"

// ------------------------------ global variables ------------------------------

static ServerCommunicationMethod *serverCMethod = NULL;
static ClientCommunicationMethod *clientCMethod = NULL;

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
    bool errorFlag = (result == PROJECT_ERROR) ? true:false;

    if (errorFlag)
    {
        char *abortMsg = messageSet(CLIENT, ABORT, "");
        // if it is null we want to free and close anyways, don't care about return at this point
        clientCMethod->sendFunction(abortMsg);
    }

    ReturnValue connectionResult = clientCMethod->clientCloseConnectionFunction();
    free(clientCMethod);
    clientCMethod = NULL;
    return connectionResult;
}

char *MPClientReceive()
{
    CHECK_NULL_RETURN_NULL(clientCMethod);
    char * msg = clientCMethod->clientReceiveFunction();
    if (msg != NULL)
        printf("Got the msg: %s\n", msg);
    else
        printf("Got NULL msg.\n");

    return msg;
//    return clientCMethod->clientReceiveFunction();
}

char *MPServerListen()
{
    CHECK_NULL_RETURN_NULL(serverCMethod);

    char * msg = serverCMethod->listenFunction();
    if (msg != NULL)
        printf("Got the msg: %s\n", msg);
    else
        printf("Got NULL msg.\n");

    return msg;

//    return serverCMethod->listenFunction();
}

ReturnValue MPServerSend(char *msg)
{
    CHECK_NULL_RETURN_ERROR(serverCMethod);
    if (!messageValidateFormat(msg))
        return PROJECT_ERROR;

    printf("Sent the msg: %s\n", msg);
    return serverCMethod->sendFunction(msg);
}

char *MPClientSend(const char *msg)
{
    CHECK_NULL_RETURN_NULL(clientCMethod);
    CHECK_NULL_RETURN_NULL(msg);

    if (!messageValidateFormat(msg))
        return NULL;


    char * reply = clientCMethod->sendFunction(msg);
    if (reply != NULL)
        printf("Got the msg: %s\n", reply);
    else
        printf("Got NULL msg.\n");

    return reply;

//    return clientCMethod->sendFunction(msg);
}

void MPServerSendSuccessOrFailure(ReturnValue result)
{
    switch (result)
    {
        case PROJECT_SUCCESS: MPServerSend(SERVER_SUCCESS_MSG); break;
        case PROJECT_ERROR: MPServerSend(SERVER_FAILURE_MSG); break;
        default: PRINT_ERROR_MSG_AND_FUNCTION_NAME("MPServerSendSuccessOrFailure", "Bad result value"); break;
    }
}
