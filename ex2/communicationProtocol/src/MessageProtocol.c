
#include "../include/MessageProtocol.h"
#include "../../communicationMethods/include/CommunicationMethods.h"


static ServerCommunicationMethod *serverCMethod = NULL;
static ClientCommunicationMethod *clientCMethod = NULL;

static ReturnValue getServerCMethod(CommunicationMethodCode cMethod)
{
    serverCMethod = serverCMethodSet(cMethod);
    CHECK_NULL_RETURN_ERROR(serverCMethod);
    return SUCCESS;
}

static ReturnValue getClientCMethodSet(CommunicationMethodCode cMethod)
{
    clientCMethod = clientCMethodSet(cMethod);
    CHECK_NULL_RETURN_ERROR(clientCMethod);
    return SUCCESS;
}

ReturnValue msgServerInitConnect(CommunicationMethodCode cMethod)
{
    getServerCMethod(cMethod);
    CHECK_NULL_RETURN_ERROR(serverCMethod);
    return serverCMethod->serverInitConnectionFunction();
}

ReturnValue msgServerCloseConnection()
{
    return serverCMethod->serverCloseConnectionFunction();
}

ReturnValue clientInitConnect(CommunicationMethodCode cMethod)
{
    return ERROR;
}

ReturnValue clientCloseConnect()
{
    return ERROR;
}


ReturnValue msgServerReceive(Message *msg)
{
    CHECK_NULL_RETURN_ERROR(msg);

    char * incomingMsg = (char *) malloc(sizeof(char));
    CHECK_NULL_RETURN_ERROR(incomingMsg);

    ReturnValue result = serverCMethod->receiveFunction(incomingMsg);
    CHECK_ERROR_GOTO_CLEANUP(result);

    result = messageFromCString(msg, incomingMsg);
    free(incomingMsg);
    return result;

    cleanup:
    free(incomingMsg);
    return ERROR;
}


ReturnValue send(Message *msg, Message *reply)
{
    return ERROR;
}
