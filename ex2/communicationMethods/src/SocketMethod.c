
// ------------------------------ includes ------------------------------

#include "../include/SocketMethod.h"

#include <winsock2.h>
#include <stdlib.h>

// -------------------------- const definitions -------------------------

#define DEFAULT_PORT 5656
#define DEFAULT_ADDRESS_FAMILY AF_INET // IPv4
#define DEFAULT_SOCKET_TYPE SOCK_STREAM
#define DEFAULT_PROTOCOL IPPROTO_TCP
#define MAXIMUM_ALLOWED_CONNECTIONS 1

#define MAX_MSG_LENGTH 512

// -------------------------- macros -------------------------

#define PRINT_ERROR_CALL_CLEANUP_RETURN_ERROR(functionName, returnValue) do { \
           fprintf(stderr,"Server: %s failed with error %d\n", functionName, returnValue); \
           WSACleanup(); \
           return ERROR; \
           } while(0)
#define PRINT_ERROR_CALL_CLEANUP_RETURN_NULL(functionName, returnValue) do { \
           fprintf(stderr,"Server: %s failed with error %d\n", functionName, returnValue); \
           WSACleanup(); \
           return NULL; \
           } while(0)


#define CHECK_INVALID_SOCKET_RETURN_ERROR(socket) do {if ((socket) == INVALID_SOCKET) return ERROR;} while(0)

// ------------------------------ global variables -----------------------------

static SOCKET listenSocket = INVALID_SOCKET;
static SOCKET clientSocket = INVALID_SOCKET;

// ------------------------------ private functions -----------------------------

static ReturnValue initWinSock() {
    WSADATA wsaData;

    // initialize Winsock version 2.2
    int returnValue = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (returnValue != 0)
        PRINT_ERROR_CALL_CLEANUP_RETURN_ERROR("WSAStartup()", returnValue);

    return SUCCESS;
}

// ------------------------------ functions -----------------------------

ReturnValue socketServerInitConnection() {
    ReturnValue result = initWinSock();
//    CHECK_ERROR_RETURN_ERROR(result);

    // Initialize the socket object for the server + Resolve the local address and port
    struct sockaddr_in local;
    local.sin_family = DEFAULT_ADDRESS_FAMILY;
    local.sin_addr.s_addr = INADDR_ANY;
    local.sin_port = htons(DEFAULT_PORT);

    // Create a SOCKET for the server to listen for client connections
    listenSocket = socket(DEFAULT_ADDRESS_FAMILY, DEFAULT_SOCKET_TYPE, DEFAULT_PROTOCOL);
    if (listenSocket == INVALID_SOCKET)
        PRINT_ERROR_CALL_CLEANUP_RETURN_ERROR("bind()", WSAGetLastError());

    // Binding - setup the TCP listening socket
    int returnValue = bind(listenSocket, (struct sockaddr*)&local, sizeof(local));
    if (returnValue == SOCKET_ERROR)
        goto cleanup;

    // Start listening
    returnValue = listen(listenSocket, MAXIMUM_ALLOWED_CONNECTIONS);
    if (returnValue == SOCKET_ERROR)
        goto cleanup;

    // Accepting a single Connection
    clientSocket = accept(listenSocket, NULL, NULL);
    if (clientSocket == INVALID_SOCKET)
        goto cleanup;

    return SUCCESS;

cleanup:
    closesocket(listenSocket);
    listenSocket = INVALID_SOCKET;
    PRINT_ERROR_CALL_CLEANUP_RETURN_ERROR("listen()", returnValue);
}

ReturnValue socketServerCloseConnection() {
    return SUCCESS;
}

char *socketListen() {
    CHECK_INVALID_SOCKET_RETURN_ERROR(listenSocket);
    CHECK_INVALID_SOCKET_RETURN_ERROR(clientSocket);

    char buf[MAX_MSG_LENGTH] = {0};
    int returnValue = 0;

    while(true)
    {
        returnValue = recv(clientSocket, buf, MAX_MSG_LENGTH, 0);
        if (returnValue == SOCKET_ERROR || returnValue == 0)
            goto cleanup;

        // todo: return real msg lol
        printf("Server got this msg: %s", buf);
        return "TEST";
    }

cleanup:
    closesocket(clientSocket);
    PRINT_ERROR_CALL_CLEANUP_RETURN_NULL("recv()", returnValue);
}

ReturnValue socketSend(const char *msg) {
    CHECK_NULL_RETURN_ERROR(msg);
    CHECK_INVALID_SOCKET_RETURN_ERROR(listenSocket);
    CHECK_INVALID_SOCKET_RETURN_ERROR(clientSocket);

    int returnValue = send(clientSocket, msg, (int)strlen(msg), 0);
    if (returnValue == SOCKET_ERROR)
        goto cleanup;

    return SUCCESS;

cleanup:
    closesocket(clientSocket);
    PRINT_ERROR_CALL_CLEANUP_RETURN_ERROR("send()", returnValue);
}

ReturnValue socketClientInitConnection() {
    return SUCCESS;
}

ReturnValue socketClientCloseConnection() {
    return SUCCESS;
}
char *socketClientSend(const char *msg)
{
    return NULL;
}
