#include "../include/CommunicationMethods.h"

#include "../include/FileMethod.h"
//#include "../include/SocketMethod.h"

/* SHAH: This should be the only existing file that will
 * be edited after adding another communicationMethods method. */

ReturnValue initServerCMethod(ServerCommunicationMethod *serverCMethod,
                              server_init_connection_function_t *serverInitConnectionFunction,
                              server_close_connection_function_t *serverCloseConnectionFunction,
                              listen_function_t *listenFunction, server_send_function_t *sendFunction)
{
    CHECK_NULL_RETURN_ERROR(serverCMethod);
    CHECK_NULL_RETURN_ERROR(serverInitConnectionFunction);
    CHECK_NULL_RETURN_ERROR(serverCloseConnectionFunction);
    CHECK_NULL_RETURN_ERROR(listenFunction);
    CHECK_NULL_RETURN_ERROR(sendFunction);

    serverCMethod->serverInitConnectionFunction = serverInitConnectionFunction;
    serverCMethod->serverCloseConnectionFunction = serverCloseConnectionFunction;
    serverCMethod->listenFunction = listenFunction;
    serverCMethod->sendFunction = sendFunction;

    return SUCCESS;
}

ReturnValue initClientCMethod(ClientCommunicationMethod *clientCMethod,
                              client_init_connection_function_t *clientInitConnectionFunction,
                              client_close_connection_function_t *clientCloseConnectionFunction,
                              client_send_function_t *sendFunction)
{
    CHECK_NULL_RETURN_ERROR(clientCMethod);
    CHECK_NULL_RETURN_ERROR(clientInitConnectionFunction);
    CHECK_NULL_RETURN_ERROR(clientCloseConnectionFunction);
    CHECK_NULL_RETURN_ERROR(sendFunction);

    clientCMethod->clientInitConnectionFunction = clientInitConnectionFunction;
    clientCMethod->clientCloseConnectionFunction = clientCloseConnectionFunction;
    clientCMethod->sendFunction = sendFunction;

    return SUCCESS;
}

ServerCommunicationMethod *serverCMethodSet(CommunicationMethodCode cMethod)
{
    ServerCommunicationMethod *serverCMethod = (ServerCommunicationMethod *) malloc(sizeof(ServerCommunicationMethod));
    CHECK_NULL_RETURN_NULL(serverCMethod);

    ReturnValue result = ERROR;
    switch (cMethod)
    {
        case SOCKET_METHOD: /* result = send socket funcs; */ break;
        case FILE_METHOD: result = initServerCMethod(serverCMethod, fileServerInitConnect, fileServerCloseConnection,
                              fileListen, fileSend); break;
        default: PRINT_ERROR_MSG_AND_FUNCTION_NAME("serverCMethodSet", "Bad cMethod"); break;
    }

    CHECK_ERROR_GOTO_CLEANUP(result);
    return serverCMethod;

cleanup:
    free(serverCMethod);
    return NULL;
}

ClientCommunicationMethod *clientCMethodSet(CommunicationMethodCode cMethod)
{
    ClientCommunicationMethod *clientCMethod = (ClientCommunicationMethod *) malloc(sizeof(ClientCommunicationMethod));
    CHECK_NULL_RETURN_NULL(clientCMethod);

    ReturnValue result = ERROR;
    switch (cMethod)
    {
        case SOCKET_METHOD: /* result = send socket funcs; */ break;
        case FILE_METHOD: result = initClientCMethod(clientCMethod, fileClientInitConnect, fileClientCloseConnect,
                                                     fileClientSend); break;
        default: PRINT_ERROR_MSG_AND_FUNCTION_NAME("clientCMethodSet", "Bad cMethod"); break;
    }

    CHECK_ERROR_GOTO_CLEANUP(result);
    return clientCMethod;

cleanup:
    free(clientCMethod);
    return NULL;
}
