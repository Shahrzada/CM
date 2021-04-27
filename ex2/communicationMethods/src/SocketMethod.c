
// ------------------------------ includes ------------------------------

#include "SocketMethod.h"

#include <winsock2.h>
#include <unistd.h>

// -------------------------- const definitions -------------------------

#define SERVER_DEFAULT_SERVER_NAME "127.0.0.1"
#define CLIENT_DEFAULT_SERVER_NAME "127.0.0.1"
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

#define CLOSE_SOCKET(socket) do { \
           closesocket((socket)); \
           (socket) = INVALID_SOCKET; \
           } while(0)

#define SHUTDOWN_AND_CLOSE_SOCKET(socket) do { \
           shutdown((socket), SD_SEND); \
           CLOSE_SOCKET(socket); \
           } while(0)

#define CHECK_INVALID_SOCKET_RETURN_NULL(socket) do {if ((socket) == INVALID_SOCKET) return NULL;} while(0)
#define CHECK_INVALID_SOCKET_RETURN_ERROR(socket) do {if ((socket) == INVALID_SOCKET) return PROJECT_ERROR;} while(0)
#define CHECK_INVALID_SOCKET_GOTO_CLEANUP(socket) do {if ((socket) == INVALID_SOCKET) goto cleanup;} while(0)
#define CHECK_SOCKET_ERROR_GOTO_CLEANUP(result) do {if ((result) == SOCKET_ERROR) goto cleanup;} while(0)

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
    unsigned int msgLength = strlen(buf) + NULL_CHAR_SIZE;
    char *msg = (char *) malloc(sizeof(char) * msgLength);
    CHECK_NULL_RETURN_NULL(msg);

    // and copy buf into it
    strncpy(msg, buf, msgLength - NULL_CHAR_SIZE);
    msg[msgLength - 1] = NULL_CHAR;

    return msg;
}

static char *socketReceive(SOCKET socket) {
    CHECK_INVALID_SOCKET_RETURN_NULL(socket);
    char buf[MAX_MSG_LENGTH] = {0};

    // TODO make sure to validate the length of the incoming msg
    int returnValue = recv(socket, buf, MAX_MSG_LENGTH - NULL_CHAR_SIZE, 0);
    if (returnValue == SOCKET_ERROR || returnValue == 0)
    {
        SHUTDOWN_AND_CLOSE_SOCKET(socket);
        PRINT_ERROR_CALL_CLEANUP_RETURN_NULL("Server", "recv()", returnValue);
    }

    return fromBufferToAllocatedMsg(buf);
}

static ReturnValue socketSend(const char *msg, SOCKET socket)
{
    CHECK_NULL_RETURN_ERROR(msg);
    CHECK_INVALID_SOCKET_RETURN_ERROR(socket);

    int returnValue = send(socket, msg, (int)strlen(msg), 0);
    if (returnValue == SOCKET_ERROR)
    {
        SHUTDOWN_AND_CLOSE_SOCKET(clientSocket);
        PRINT_ERROR_CALL_CLEANUP("Server/Client", "send()", returnValue);
        return PROJECT_ERROR;
    }

    return PROJECT_SUCCESS;
}

static ReturnValue socketCloseConnection(SOCKET socket)
{
    CHECK_INVALID_SOCKET_RETURN_ERROR(socket);
    SHUTDOWN_AND_CLOSE_SOCKET(socket);
    WSACleanup();
    return PROJECT_SUCCESS;
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
    CHECK_SOCKET_ERROR_GOTO_CLEANUP(returnValue);

    // Start listening
    returnValue = listen(listenSocket, MAXIMUM_ALLOWED_CONNECTIONS);
    CHECK_SOCKET_ERROR_GOTO_CLEANUP(returnValue);

    // Accepting a single Connection
    clientSocket = accept(listenSocket, NULL, NULL);
    CHECK_INVALID_SOCKET_GOTO_CLEANUP(clientSocket);

    // No longer need server socket, because we have 1 client
    SHUTDOWN_AND_CLOSE_SOCKET(listenSocket);
    return PROJECT_SUCCESS;

cleanup:
    SHUTDOWN_AND_CLOSE_SOCKET(listenSocket);
    PRINT_ERROR_CALL_CLEANUP("Server", "socketServerInitConnection()", returnValue);
    return PROJECT_ERROR;
}

ReturnValue socketServerCloseConnection()
{
    CHECK_INVALID_SOCKET_RETURN_ERROR(clientSocket);
    return socketCloseConnection(clientSocket);
}

char *socketListen()
{
    CHECK_INVALID_SOCKET_RETURN_NULL(clientSocket);
    return socketReceive(clientSocket);
}

ReturnValue socketServerSend(const char *msg)
{
    CHECK_NULL_RETURN_ERROR(msg);
    CHECK_INVALID_SOCKET_RETURN_ERROR(clientSocket);

    sleep(1); // TODO REMOVE THIS

    return socketSend(msg, clientSocket);
}

ReturnValue socketClientInitConnection() {
    ReturnValue result = initWinSock();
    CHECK_ERROR_RETURN_ERROR(result);

    // Get host
    struct hostent *hp;
    hp = gethostbyname(CLIENT_DEFAULT_SERVER_NAME);
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
        SHUTDOWN_AND_CLOSE_SOCKET(connectionSocket);
        PRINT_ERROR_CALL_CLEANUP("Client", "connect()", WSAGetLastError());
        return PROJECT_ERROR;
    }

    return PROJECT_SUCCESS;
}

ReturnValue socketClientCloseConnection()
{
    CHECK_INVALID_SOCKET_RETURN_ERROR(connectionSocket);
    return socketCloseConnection(connectionSocket);
}

char *socketClientSend(const char *msg)
{
    CHECK_NULL_RETURN_NULL(msg);
    CHECK_INVALID_SOCKET_RETURN_NULL(connectionSocket);

    // Send msg to server
    ReturnValue result = socketSend(msg, connectionSocket);
    if (result == PROJECT_ERROR)
    {
        SHUTDOWN_AND_CLOSE_SOCKET(connectionSocket);
        PRINT_ERROR_CALL_CLEANUP_RETURN_NULL("Client", "send()", WSAGetLastError());
    }

    // wait for a reply
    return socketReceive(connectionSocket);
}

char *socketClientReceive()
{
    CHECK_INVALID_SOCKET_RETURN_NULL(connectionSocket);
    return socketReceive(connectionSocket);
}
