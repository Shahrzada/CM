
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
    ReturnValue result = serverCMethod->serverCloseConnectionFunction();
    free(serverCMethod);
    serverCMethod = NULL;
    return result;
}

ReturnValue MPClientInitConnection(CommunicationMethodCode cMethodCode)
{
    clientCMethod = clientCMethodSet(cMethodCode);
    CHECK_NULL_RETURN_ERROR(clientCMethod);
    return clientCMethod->clientInitConnectionFunction();
}

ReturnValue MPClientCloseConnection()
{
    ReturnValue result = clientCMethod->clientCloseConnectionFunction();
    free(clientCMethod);
    return result;
}


char *MPServerReceive()
{
    CHECK_NULL_RETURN_NULL(serverCMethod);
    return serverCMethod->receiveFunction();
}


char *MPSend(char *msg)
{
    CHECK_NULL_RETURN_NULL(clientCMethod);
    CHECK_NULL_RETURN_NULL(msg);

    if (!messageValidateFormat(msg))
        return NULL;

    // use the communication method to send the msg
    return clientCMethod->sendFunction(msg);
}
