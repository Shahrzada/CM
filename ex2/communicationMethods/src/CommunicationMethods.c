#include "../include/CommunicationMethods.h"

#include "../include/FileMethod.h"
//#include "../include/SocketMethod.h"
/* This should be the only file (besides main due to input) that will
 * be edited after adding another communicationMethods method. */

ReturnValue initServerSocketCMethod(ServerCommunicationMethod *serverCMethod)
{
    CHECK_NULL_RETURN_ERROR(serverCMethod);

    return SUCCESS;
}

ReturnValue initServerFileCMethod(ServerCommunicationMethod *serverCMethod)
{
    CHECK_NULL_RETURN_ERROR(serverCMethod);

    serverCMethod->serverInitConnectionFunction = fileServerInitConnect;
    serverCMethod->serverCloseConnectionFunction = fileServerCloseConnection;
    serverCMethod->receiveFunction = fileListen;
    serverCMethod->sendFunction = fileServerSend;

    return SUCCESS;
}

ReturnValue initClientSocketCMethod(ClientCommunicationMethod *clientCMethod)
{
    CHECK_NULL_RETURN_ERROR(clientCMethod);

    return SUCCESS;
}

ReturnValue initClientFileCMethod(ClientCommunicationMethod *clientCMethod)
{
    CHECK_NULL_RETURN_ERROR(clientCMethod);

    clientCMethod->clientInitConnectionFunction = fileClientInitConnect;
    clientCMethod->clientCloseConnectionFunction = fileClientCloseConnect;
    clientCMethod->sendFunction = fileClientSend;

    return SUCCESS;
}

// if serverCMethod is null then allocate and set it, ow return the existing one
ServerCommunicationMethod *serverCMethodSet(CommunicationMethodCode cMethod)
{
    ServerCommunicationMethod *serverCMethod = (ServerCommunicationMethod *) malloc(sizeof(ServerCommunicationMethod));
    CHECK_NULL_RETURN_NULL(serverCMethod);

    ReturnValue result = ERROR;
    if (cMethod == SOCKET_METHOD)
    {
        result = initServerSocketCMethod(serverCMethod);
    }
    else if (cMethod == FILE_METHOD)
    {
        result = initServerFileCMethod(serverCMethod);
    }
    CHECK_ERROR_GOTO_CLEANUP(result);
    return serverCMethod;

cleanup:
    free(serverCMethod);
    return NULL;
}

// if clientCMethod is null then allocate and set it, ow return the existing one
ClientCommunicationMethod *clientCMethodSet(CommunicationMethodCode cMethod)
{
    ClientCommunicationMethod *clientCMethod = (ClientCommunicationMethod *) malloc(sizeof(ClientCommunicationMethod));
    CHECK_NULL_RETURN_NULL(clientCMethod);

    ReturnValue result = ERROR;
    if (cMethod == SOCKET_METHOD)
    {
        result = initClientSocketCMethod(clientCMethod);
    }
    else if (cMethod == FILE_METHOD)
    {
        result = initClientFileCMethod(clientCMethod);
    }

    CHECK_ERROR_GOTO_CLEANUP(result);
    return clientCMethod;

cleanup:
    free(clientCMethod);
    return NULL;
}
