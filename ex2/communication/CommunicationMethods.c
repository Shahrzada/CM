#include "CommunicationMethods.h"

#include "FileMethod.h"
#include "SocketMethod.h"
/* This should be the only file (besides main due to input) that will
 * be edited after adding another communication method. */

/* It is static because it should only be initialized once */
static ServerCommunicationMethod *serverCMethod = NULL;
static ClientCommunicationMethod *clientCMethod = NULL;

ReturnValue initServerSocketCMethod()
{
    return (ReturnValue) NULL;
}

ReturnValue initServerFileCMethod()
{
    serverCMethod = (ServerCommunicationMethod *) malloc(sizeof(ServerCommunicationMethod));
    CHECK_NULL_RETURN_ERROR(serverCMethod);

    serverCMethod->serverInitConnectionFunction = (void *(*)()) &fileServerInitConnect;
    serverCMethod->serverCloseConnectionFunction = (ReturnValue (*)(void *)) fileServerCloseConnection;
    serverCMethod->listenFunction = (ReturnValue (*)(void *, Message *)) fileListen;
    serverCMethod->sendFunction = fileSend;

    return SUCCESS;
}

ReturnValue initClientSocketCMethod()
{
    return (ReturnValue) NULL;
}

ReturnValue initClientFileCMethod()
{
    clientCMethod = (ClientCommunicationMethod *) malloc(sizeof(ClientCommunicationMethod));
    CHECK_NULL_RETURN_ERROR(clientCMethod);

    clientCMethod->clientInitConnectionFunction = fileClientInitConnect;
    clientCMethod->clientCloseConnectionFunction = fileClientCloseConnect;
    clientCMethod->sendFunction = fileSend;

    return SUCCESS;
}

// if serverCMethod is null then allocate and set it, ow return the existing one
ServerCommunicationMethod *serverCMethodSet(CommunicationMethodCode cMethod)
{
    ReturnValue result = 0;

    if (cMethod == SOCKET_METHOD)
    {
        result = initServerSocketCMethod();
    }
    else if (cMethod == FILE_METHOD)
    {
        result = initServerFileCMethod();
    }

    CHECK_ERROR_RETURN_NULL(result);
    return serverCMethod;
}

// if clientCMethod is null then allocate and set it, ow return the existing one
ClientCommunicationMethod *clientCMethodSet(CommunicationMethodCode cMethod)
{
    ReturnValue result = 0;
    if (cMethod == SOCKET_METHOD)
    {
        result = initClientSocketCMethod();
    }
    else if (cMethod == FILE_METHOD)
    {
        result = initClientFileCMethod();
    }

    CHECK_ERROR_RETURN_NULL(result);
    return clientCMethod;

}
