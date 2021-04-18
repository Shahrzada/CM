
#include "../include/MessageProtocol.h"
#include "../../communicationMethods/include/CommunicationMethods.h"


static ServerCommunicationMethod *serverCMethod = NULL;
static ClientCommunicationMethod *clientCMethod = NULL;


ReturnValue MPServerInitConnection(CommunicationMethodCode cMethodCode)
{
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
    return result;
}

char *MPServerReceive()
{
    CHECK_NULL_RETURN_NULL(serverCMethod);
    return serverCMethod->receiveFunction();
}

ReturnValue MPServerSend(char *msg)
{
    CHECK_NULL_RETURN_ERROR(serverCMethod);
    CHECK_NULL_RETURN_ERROR(msg);

    if (!messageValidateFormat(msg))
        return ERROR;

    // use the communication method to send the msg
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
    if (result == SUCCESS)
        MPServerSend(SERVER_SUCCESS_MSG);
    else if (result == ERROR)
        MPServerSend(SERVER_FAILURE_MSG);
}
