
// ------------------------------ includes ------------------------------

#include "../include/SocketMethod.h"

#include <winsock2.h>
#include <stdlib.h>

// -------------------------- const definitions -------------------------

#define DEFAULT_SERVER_NAME "127.0.0.1"
#define DEFAULT_PORT 5656
#define DEFAULT_ADDRESS_FAMILY AF_INET // IPv4
#define DEFAULT_SOCKET_TYPE SOCK_STREAM
#define DEFAULT_PROTOCOL IPPROTO_TCP
#define MAXIMUM_ALLOWED_CONNECTIONS 1

#define MAX_MSG_LENGTH 512

// -------------------------- macros -------------------------

#define PRINT_ERROR_CALL_CLEANUP_RETURN_ERROR(role, functionName, returnValue) do { \
           fprintf(stderr,"%s: %s failed with error %d\n", role, functionName, returnValue); \
           WSACleanup(); \
           return ERROR; \
           } while(0)
#define PRINT_ERROR_CALL_CLEANUP_RETURN_NULL(role, functionName, returnValue) do { \
           fprintf(stderr,"%s: %s failed with error %d\n", role, functionName, returnValue); \
           WSACleanup(); \
           return NULL; \
           } while(0)


#define CHECK_INVALID_SOCKET_RETURN_ERROR(socket) do {if ((socket) == INVALID_SOCKET) return ERROR;} while(0)

// ------------------------------ global variables -----------------------------

static SOCKET clientSocket = INVALID_SOCKET;
static SOCKET connectionSocket = INVALID_SOCKET;

// ------------------------------ private functions -----------------------------

static ReturnValue initWinSock() {
    WSADATA wsaData;

    // initialize Winsock version 2.2
    int returnValue = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (returnValue != 0)
        PRINT_ERROR_CALL_CLEANUP_RETURN_ERROR("Server", "WSAStartup()", returnValue);

    return SUCCESS;
}

static char *fromBufferToAllocatedMsg(char * buf) {
    CHECK_NULL_RETURN_NULL(buf);

    // allocate memory for the msg
    unsigned int msgLength = strlen(buf) + NULL_CHAR_SIZE;
    char *msg = (char *) malloc(sizeof(char) * msgLength);
    CHECK_NULL_RETURN_NULL(msg);

    // and copy buf into it
    strncpy(msg, buf, msgLength - NULL_CHAR_SIZE);
    msg[msgLength - 1] = NULL_CHAR;

    return msg;
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
    SOCKET listenSocket = socket(DEFAULT_ADDRESS_FAMILY, DEFAULT_SOCKET_TYPE, DEFAULT_PROTOCOL);
    if (listenSocket == INVALID_SOCKET)
        PRINT_ERROR_CALL_CLEANUP_RETURN_ERROR("Server", "bind()", WSAGetLastError());

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

    // No longer need server socket
    closesocket(listenSocket);

    return SUCCESS;

cleanup:
    closesocket(listenSocket);
    listenSocket = INVALID_SOCKET;
    PRINT_ERROR_CALL_CLEANUP_RETURN_ERROR("Server", "listen()", returnValue);
}

ReturnValue socketServerCloseConnection() {
    CHECK_INVALID_SOCKET_RETURN_ERROR(clientSocket);

    // Shutdown the send half of the connection since no more data will be sent
    int returnValue = shutdown(clientSocket, SD_SEND);
    if (returnValue == SOCKET_ERROR)
    {
        closesocket(clientSocket);
        PRINT_ERROR_CALL_CLEANUP_RETURN_ERROR("Server", "shutdown()", returnValue);
    }

    // cleanup
    closesocket(clientSocket);
    WSACleanup();

    return SUCCESS;
}

char *socketListen() {
    CHECK_INVALID_SOCKET_RETURN_ERROR(clientSocket);

    char buf[MAX_MSG_LENGTH] = {0};
    int returnValue = 0;


    returnValue = recv(clientSocket, buf, MAX_MSG_LENGTH, 0);
    if (returnValue == SOCKET_ERROR || returnValue == 0)
        goto cleanup;

    char *msg = fromBufferToAllocatedMsg(buf);
    CHECK_NULL_GOTO_CLEANUP(msg);
    return msg;

cleanup:
    closesocket(clientSocket);
    PRINT_ERROR_CALL_CLEANUP_RETURN_NULL("Server", "recv()", returnValue);
}

ReturnValue socketSend(const char *msg) {
    CHECK_NULL_RETURN_ERROR(msg);
    CHECK_INVALID_SOCKET_RETURN_ERROR(clientSocket);

    int returnValue = send(clientSocket, msg, (int)strlen(msg), 0);
    if (returnValue == SOCKET_ERROR)
        goto cleanup;

    return SUCCESS;

cleanup:
    closesocket(clientSocket);
    PRINT_ERROR_CALL_CLEANUP_RETURN_ERROR("Server", "send()", returnValue);
}

ReturnValue socketClientInitConnection() {
    ReturnValue result = initWinSock();
//    CHECK_ERROR_RETURN_ERROR(result);

    // Get host
    struct hostent *hp;
    hp = gethostbyname(DEFAULT_SERVER_NAME);
    if (hp == NULL)
        PRINT_ERROR_CALL_CLEANUP_RETURN_ERROR("Client", "gethostbyname()", WSAGetLastError());

    // Copy the resolved information into the sockaddr_in structure
    struct sockaddr_in server;
    memset(&server, 0, sizeof(server));
    memcpy(&(server.sin_addr), hp->h_addr, hp->h_length);
    server.sin_family = hp->h_addrtype;
    server.sin_port = htons(DEFAULT_PORT);

    // Create the connection socket
    connectionSocket = socket(DEFAULT_ADDRESS_FAMILY, DEFAULT_SOCKET_TYPE, DEFAULT_PROTOCOL);
    if (connectionSocket == INVALID_SOCKET )
        PRINT_ERROR_CALL_CLEANUP_RETURN_ERROR("Client", "socket()", WSAGetLastError());

    // Connect to server
    int connectionResult = connect(connectionSocket, (struct sockaddr*)&server, sizeof(server));
    if (connectionResult == SOCKET_ERROR)
        goto cleanup;

    return SUCCESS;

cleanup:
    closesocket(connectionSocket);
    PRINT_ERROR_CALL_CLEANUP_RETURN_ERROR("Client", "connect()", WSAGetLastError());
}

ReturnValue socketClientCloseConnection() {
    CHECK_INVALID_SOCKET_RETURN_ERROR(connectionSocket);
    closesocket(connectionSocket);
    WSACleanup();
    return SUCCESS;
}

char *socketClientSend(const char *msg)
{
    CHECK_NULL_RETURN_NULL(msg);
    CHECK_INVALID_SOCKET_RETURN_ERROR(connectionSocket);

    // Send msg to server
    int result = send(connectionSocket, msg, (int)strlen(msg), 0);
    if (result == SOCKET_ERROR)
        PRINT_ERROR_CALL_CLEANUP_RETURN_NULL("Client", "send()", WSAGetLastError());

    // Wait for a reply from the server
    char buf[MAX_MSG_LENGTH] = {0};
    result = recv(connectionSocket, buf, MAX_MSG_LENGTH, 0);
    if (result == SOCKET_ERROR || result == 0)
        goto cleanup;

    char *reply = fromBufferToAllocatedMsg(buf);
    CHECK_NULL_GOTO_CLEANUP(reply);
    return reply;

cleanup:
    closesocket(connectionSocket);
    PRINT_ERROR_CALL_CLEANUP_RETURN_NULL("Client", "recv()", WSAGetLastError());
}
