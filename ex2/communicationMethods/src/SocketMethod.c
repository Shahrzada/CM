
// ------------------------------ includes ------------------------------

#include "../include/SocketMethod.h"

#include <winsock2.h>
#include <unistd.h>

// -------------------------- const definitions -------------------------

#define DEFAULT_SERVER_NAME "127.0.0.1" // CR: Client and server should have different defines?
#define DEFAULT_PORT 5656
#define DEFAULT_ADDRESS_FAMILY AF_INET // IPv4
#define DEFAULT_SOCKET_TYPE SOCK_STREAM
#define DEFAULT_PROTOCOL IPPROTO_TCP
#define MAXIMUM_ALLOWED_CONNECTIONS 1

// -------------------------- macros -------------------------

#define PRINT_ERROR_CALL_CLEANUP(role, functionName, returnValue) do { \
           fprintf(stderr,"%s: %s failed with error %d\n", role, functionName, returnValue); \
           WSACleanup(); \
           } while(0)

#define PRINT_ERROR_CALL_CLEANUP_RETURN_NULL(role, functionName, returnValue) do { \
           fprintf(stderr,"%s: %s failed with error %d\n", role, functionName, returnValue); \
           WSACleanup(); \
           return NULL; \
           } while(0)
// CR: When closing a socket you should call shutdown
#define CLOSE_SOCKET(socket) do { \
           closesocket((socket)); \
           (socket) = INVALID_SOCKET; \
           } while(0)

// ------------------------------ global variables -----------------------------

static SOCKET clientSocket = INVALID_SOCKET;
static SOCKET connectionSocket = INVALID_SOCKET;

// ------------------------------ private functions -----------------------------

/* initialize Winsock version 2.2 */
static ReturnValue initWinSock() {
    WSADATA wsaData;
    int returnValue = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (returnValue != 0)
    {
        PRINT_ERROR_CALL_CLEANUP("Socket init", "WSAStartup()", returnValue);
        return PROJECT_ERROR;
    }
    return PROJECT_SUCCESS;
}

static char *fromBufferToAllocatedMsg(char *buf) {
    CHECK_NULL_RETURN_NULL(buf);

    // allocate memory for the msg
    // CR: same as messageSet
    unsigned int msgLength = strlen(buf) + NULL_CHAR_SIZE;
    char *msg = (char *) malloc(sizeof(char) * msgLength);
    CHECK_NULL_RETURN_NULL(msg);

    // and copy buf into it
    strncpy(msg, buf, msgLength - NULL_CHAR_SIZE);
    msg[msgLength - 1] = NULL_CHAR;

    return msg;
}

static char *socketListenGivenSocket(SOCKET socket) {
    if (socket == INVALID_SOCKET)
        return NULL;
    char buf[MAX_MSG_LENGTH] = {0};

    // Receive msg
    // CR: a bit code duplication, what about socketClientSend?
    int returnValue = recv(socket, buf, MAX_MSG_LENGTH - NULL_CHAR_SIZE, 0);
    if (returnValue == SOCKET_ERROR || returnValue == 0)
    {
        CLOSE_SOCKET(socket);
        PRINT_ERROR_CALL_CLEANUP_RETURN_NULL("Server", "recv()", returnValue);
    }

    return fromBufferToAllocatedMsg(buf);
}

// ------------------------------ functions -----------------------------

ReturnValue socketServerInitConnection() {
    ReturnValue result = initWinSock();
    CHECK_ERROR_RETURN_ERROR(result);

    // Initialize the socket object for the server + Resolve the local address and port
    struct sockaddr_in local;
    local.sin_family = DEFAULT_ADDRESS_FAMILY;
    local.sin_addr.s_addr = INADDR_ANY;
    local.sin_port = htons(DEFAULT_PORT);

    // Create a SOCKET for the server to listen for client connections
    SOCKET listenSocket = socket(DEFAULT_ADDRESS_FAMILY, DEFAULT_SOCKET_TYPE, DEFAULT_PROTOCOL);
    if (listenSocket == INVALID_SOCKET)
    {
        PRINT_ERROR_CALL_CLEANUP("Server", "bind()", WSAGetLastError());
        return PROJECT_ERROR;
    }

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

    // No longer need server socket, because we have 1 client
    CLOSE_SOCKET(listenSocket);

    return PROJECT_SUCCESS;

cleanup:
    CLOSE_SOCKET(listenSocket);
    PRINT_ERROR_CALL_CLEANUP("Server", "listen()", returnValue);
    return PROJECT_ERROR;
}

ReturnValue socketServerCloseConnection() {
    if (clientSocket == INVALID_SOCKET)
        return PROJECT_ERROR;

    // Shutdown the send half of the connection since no more data will be sent
    int returnValue = shutdown(clientSocket, SD_SEND);
    CLOSE_SOCKET(clientSocket);
    if (returnValue == SOCKET_ERROR)
    {
        PRINT_ERROR_CALL_CLEANUP("Server", "shutdown()", returnValue);
        return PROJECT_ERROR;
    }

    WSACleanup();
    return PROJECT_SUCCESS;
}

char *socketListen()
{
    return socketListenGivenSocket(clientSocket);
}

ReturnValue socketSend(const char *msg)
{
    CHECK_NULL_RETURN_ERROR(msg);
    if (clientSocket == INVALID_SOCKET)
        return PROJECT_ERROR;

    sleep(1);
    // CR: ???????????
    // CR: same as socketClientSend...
    int returnValue = send(clientSocket, msg, (int)strlen(msg), 0);
    if (returnValue == SOCKET_ERROR)
    {
        CLOSE_SOCKET(clientSocket);
        PRINT_ERROR_CALL_CLEANUP("Server", "send()", returnValue);
        return PROJECT_ERROR;
    }

    return PROJECT_SUCCESS;
}

ReturnValue socketClientInitConnection() {
    ReturnValue result = initWinSock();
    CHECK_ERROR_RETURN_ERROR(result);

    // Get host
    struct hostent *hp;
    hp = gethostbyname(DEFAULT_SERVER_NAME);
    if (hp == NULL)
    {
        PRINT_ERROR_CALL_CLEANUP("Client", "gethostbyname()", WSAGetLastError());
        return PROJECT_ERROR;
    }

    // Copy the resolved information into the sockaddr_in structure
    struct sockaddr_in server;
    memset(&server, 0, sizeof(server));
    memcpy(&(server.sin_addr), hp->h_addr, hp->h_length);
    server.sin_family = hp->h_addrtype;
    server.sin_port = htons(DEFAULT_PORT);

    // Create the connection socket
    connectionSocket = socket(DEFAULT_ADDRESS_FAMILY, DEFAULT_SOCKET_TYPE, DEFAULT_PROTOCOL);
    if (connectionSocket == INVALID_SOCKET)
    {
        PRINT_ERROR_CALL_CLEANUP("Client", "socket()", WSAGetLastError());
        return PROJECT_ERROR;
    }

    // Connect to server
    int connectionResult = connect(connectionSocket, (struct sockaddr*)&server, sizeof(server));
    if (connectionResult == SOCKET_ERROR)
    {
        CLOSE_SOCKET(connectionSocket);
        PRINT_ERROR_CALL_CLEANUP("Client", "connect()", WSAGetLastError());
        return PROJECT_ERROR;
    }

    return PROJECT_SUCCESS;
}

ReturnValue socketClientCloseConnection() {
    if (connectionSocket == INVALID_SOCKET)
        return PROJECT_ERROR;
    CLOSE_SOCKET(connectionSocket);
    WSACleanup();
    return PROJECT_SUCCESS;
}

char *socketClientSend(const char *msg) {
    CHECK_NULL_RETURN_NULL(msg);
    if (connectionSocket == INVALID_SOCKET)
        return NULL;

    // Send msg to server
    // CR: same as recv below
    int result = send(connectionSocket, msg, (int) strlen(msg), 0);
    if (result == SOCKET_ERROR)
    {
        CLOSE_SOCKET(connectionSocket);
        PRINT_ERROR_CALL_CLEANUP_RETURN_NULL("Client", "send()", WSAGetLastError());
    }

    // Wait for a reply from the server
    char buf[MAX_MSG_LENGTH] = {0};
    // CR: did you read recv return value?
    // CR: flags magic
    result = recv(connectionSocket, buf, MAX_MSG_LENGTH - NULL_CHAR_SIZE, 0);
    if (result == SOCKET_ERROR || result == 0)
    {
        CLOSE_SOCKET(connectionSocket);
        PRINT_ERROR_CALL_CLEANUP_RETURN_NULL("Client", "recv()", WSAGetLastError());
    }

    return fromBufferToAllocatedMsg(buf);
}

char *socketClientReceive()
{
    return socketListenGivenSocket(connectionSocket);
}
