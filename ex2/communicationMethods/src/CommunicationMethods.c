
// ------------------------------ includes ------------------------------

#include "CommunicationMethods.h"
#include "FileMethod.h"
#include "SocketMethod.h"
//#include "NewMethod.h"

// ------------------------------ private functions -----------------------------

static ReturnValue initServerCMethod(server_communication_method *serverCMethod,
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

    return PROJECT_SUCCESS;
}

static ReturnValue initClientCMethod(client_communication_method *clientCMethod,
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

    return PROJECT_SUCCESS;
}

// ------------------------------ functions -----------------------------

server_communication_method *serverCMethodSet(CommunicationMethodCode cMethod)
{
    server_communication_method *serverCMethod = (server_communication_method *) malloc(sizeof(server_communication_method));
    CHECK_NULL_RETURN_NULL(serverCMethod);

    ReturnValue result = PROJECT_ERROR;
    switch (cMethod)
    {
        case FILE_METHOD:
            result = initServerCMethod(serverCMethod, fileServerInitConnection,
                                       fileServerCloseConnection,fileServerListen, fileServerSend);
            break;
        case SOCKET_METHOD:
            result = initServerCMethod(serverCMethod, socketServerInitConnection,
                                       socketServerCloseConnection, socketServerListen, socketServerSend);
            break;
        /**case NEW_METHOD:
         * result = initServerCMethod(relevant functions...);
         * break;**/
        default:
            PRINT_ERROR_WITH_FUNCTION("serverCMethodSet", "Bad cMethod");
            break;
    }

    CHECK_ERROR_GOTO_CLEANUP(result);
    return serverCMethod;

cleanup:
    free(serverCMethod);
    return NULL;
}

client_communication_method *clientCMethodSet(CommunicationMethodCode cMethod)
{
    client_communication_method *clientCMethod = (client_communication_method *) malloc(sizeof(client_communication_method));
    CHECK_NULL_RETURN_NULL(clientCMethod);

    ReturnValue result = PROJECT_ERROR;
    switch (cMethod)
    {
        case FILE_METHOD:
            result = initClientCMethod(clientCMethod, fileClientInitConnection,
                                       fileClientCloseConnection, fileClientSend);
            break;
        case SOCKET_METHOD:
            result = initClientCMethod(clientCMethod, socketClientInitConnection,
                                       socketClientCloseConnection, socketClientSend);
            break;
        /**case NEW_METHOD:
         * result = initClientCMethod(relevant functions...);
         * break;**/
        default:
            PRINT_ERROR_WITH_FUNCTION("clientCMethodSet", "Bad cMethod");
            break;
    }

    CHECK_ERROR_GOTO_CLEANUP(result);
    return clientCMethod;

cleanup:
    free(clientCMethod);
    return NULL;
}
