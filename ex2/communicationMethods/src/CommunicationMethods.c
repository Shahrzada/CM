#include "../include/CommunicationMethods.h"

#include "../include/FileMethod.h"
//#include "../include/SocketMethod.h"
/* This should be the only file (besides main due to input) that will
 * be edited after adding another communicationMethods method. */

/* It is static because it should only be initialized once */
static ServerCommunicationMethod *serverCMethod = NULL;
static ClientCommunicationMethod *clientCMethod = NULL;

ReturnValue initServerSocketCMethod()
{
    return (ReturnValue) NULL;
}

ReturnValue initServerFileCMethod()
{
    serverCMethod->serverInitConnectionFunction = fileServerInitConnect;
    serverCMethod->serverCloseConnectionFunction = fileServerCloseConnection;
    serverCMethod->receiveFunction = fileListen;
    serverCMethod->sendFunction = fileServerSend;

    return SUCCESS;
}

ReturnValue initClientSocketCMethod()
{
    return (ReturnValue) NULL;
}

ReturnValue initClientFileCMethod()
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
    if (serverCMethod != NULL)
        return serverCMethod;

    serverCMethod = (ServerCommunicationMethod *) malloc(sizeof(ServerCommunicationMethod));
    CHECK_NULL_RETURN_NULL(serverCMethod);

    ReturnValue result = ERROR;
    if (cMethod == SOCKET_METHOD)
    {
        result = initServerSocketCMethod();
    }
    else if (cMethod == FILE_METHOD)
    {
        result = initServerFileCMethod();
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
    if (clientCMethod != NULL)
        return clientCMethod;

    clientCMethod = (ClientCommunicationMethod *) malloc(sizeof(ClientCommunicationMethod));
    CHECK_NULL_RETURN_NULL(clientCMethod);

    ReturnValue result = ERROR;
    if (cMethod == SOCKET_METHOD)
    {
        result = initClientSocketCMethod();
    }
    else if (cMethod == FILE_METHOD)
    {
        result = initClientFileCMethod();
    }

    CHECK_ERROR_GOTO_CLEANUP(result);
    return clientCMethod;

cleanup:
    free(clientCMethod);
    return NULL;
}
