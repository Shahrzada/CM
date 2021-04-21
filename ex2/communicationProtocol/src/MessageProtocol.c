
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

ReturnValue MPClientCloseConnection()
{
    CHECK_NULL_RETURN_ERROR(clientCMethod);
    ReturnValue result = clientCMethod->clientCloseConnectionFunction();
    free(clientCMethod);
    clientCMethod = NULL;
    return result;
}

char *MPClientReceive()
{
    CHECK_NULL_RETURN_NULL(clientCMethod);
    return clientCMethod->clientReceiveFunction();
}

char *MPServerListen()
{
    CHECK_NULL_RETURN_NULL(serverCMethod);
    return serverCMethod->listenFunction();
}

ReturnValue MPServerSend(char *msg)
{
    CHECK_NULL_RETURN_ERROR(serverCMethod);
    if (!messageValidateFormat(msg))
        return PROJECT_ERROR;
    return serverCMethod->sendFunction(msg);
}

char *MPClientSend(const char *msg)
{
    CHECK_NULL_RETURN_NULL(clientCMethod);
    CHECK_NULL_RETURN_NULL(msg);

    if (!messageValidateFormat(msg))
        return NULL;

    // use the communication method to send the msg
    return clientCMethod->sendFunction(msg);
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
