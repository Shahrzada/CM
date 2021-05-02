
// ------------------------------ includes ------------------------------

#include "SocketMethod.h"

#include <winsock2.h>
#include <unistd.h>
#include <config.h>

// -------------------------- const definitions -------------------------

#define DEFAULT_ADDRESS_FAMILY AF_INET // IPv4
#define DEFAULT_SOCKET_TYPE SOCK_STREAM
#define DEFAULT_PROTOCOL IPPROTO_TCP
#define MAXIMUM_ALLOWED_CONNECTIONS 1
#define NO_FLAGS 0

// -------------------------- macros -------------------------

#define PRINT_ERROR_CALL_WSACLEANUP(role, functionName, returnValue) do { \
           fprintf(stderr,"%s: %s failed with error %d\n", role, functionName, returnValue); \
           WSACleanup(); \
           } while(0)

#define PRINT_ERROR_CALL_WSACLEANUP_RETURN_ZERO(role, functionName, returnValue) do { \
           fprintf(stderr,"%s: %s failed with error %d\n", role, functionName, returnValue); \
           WSACleanup(); \
           return 0; \
           } while(0)

#define CLOSE_SOCKET(socket) do { \
           closesocket((socket)); \
           (socket) = INVALID_SOCKET; \
           } while(0)

#define SHUTDOWN_AND_CLOSE_SOCKET(socket) do { \
           shutdown((socket), SD_SEND); \
           CLOSE_SOCKET(socket); \
           } while(0)

#define CHECK_INVALID_SOCKET_RETURN_ERROR(socket) do {if ((socket) == INVALID_SOCKET) return PROJECT_ERROR;} while(0)
#define CHECK_INVALID_SOCKET_RETURN_ZERO(socket) do {if ((socket) == INVALID_SOCKET) return 0;} while(0)
#define CHECK_INVALID_SOCKET_GOTO_CLEANUP(socket) do {if ((socket) == INVALID_SOCKET) goto cleanup;} while(0)

#define CHECK_SOCKET_ERROR_GOTO_CLEANUP(result) do {if ((result) == SOCKET_ERROR) goto cleanup;} while(0)

// ------------------------------ global variables -----------------------------

static SOCKET clientSocket = INVALID_SOCKET;
static SOCKET connectionSocket = INVALID_SOCKET;

static char *serverName = NULL;
static int serverPort = 0;

static char *clientServerName = NULL;
static int clientServerPort = 0;

// ------------------------------ private functions -----------------------------

/* initialize Winsock version 2.2 */
static ReturnValue initWinSock()
{
    WSADATA wsaData;
    int returnValue = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (returnValue != 0)
    {
        PRINT_ERROR_CALL_WSACLEANUP("Socket init", "WSAStartup()", returnValue);
        return PROJECT_ERROR;
    }
    return PROJECT_SUCCESS;
}

static ReturnValue initServerNameAndPortForServer()
{
    serverName = getServerName();
    CHECK_NULL_RETURN_ERROR(serverName);

    serverPort = getServerPort();
    CHECK_ZERO_RETURN_ERROR(serverPort);

    return PROJECT_SUCCESS;
}

static ReturnValue initServerNameAndPortForClient()
{
    clientServerName = getClientServerName();
    CHECK_NULL_RETURN_ERROR(clientServerName);

    clientServerPort = getClientServerPort();
    CHECK_ZERO_RETURN_ERROR(clientServerPort);

    return PROJECT_SUCCESS;
}

static unsigned int socketReceive(SOCKET socket, char *buffer)
{
    CHECK_INVALID_SOCKET_RETURN_ZERO(socket);
    CHECK_NULL_RETURN_ZERO(buffer);

    // Notice: if the msg is larger than the buffer, the buffer is filled and recv generates an error
    unsigned int totalBytesReceived = recv(socket, buffer, MAX_MSG_LENGTH, NO_FLAGS);
    if (totalBytesReceived == SOCKET_ERROR || totalBytesReceived == 0)
    {
        SHUTDOWN_AND_CLOSE_SOCKET(socket);
        PRINT_ERROR_CALL_WSACLEANUP_RETURN_ZERO("Server/Client", "recv()", totalBytesReceived);
    }
    return totalBytesReceived;
}

static ReturnValue socketSend(const char *msg, unsigned int msgLength, SOCKET socket)
{
    CHECK_NULL_RETURN_ERROR(msg);
    CHECK_INVALID_SOCKET_RETURN_ERROR(socket);

    int returnValue = send(socket, msg, (int)msgLength, 0);
    if (returnValue == SOCKET_ERROR)
    {
        SHUTDOWN_AND_CLOSE_SOCKET(clientSocket);
        PRINT_ERROR_CALL_WSACLEANUP("Server/Client", "send()", returnValue);
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

ReturnValue socketServerInitConnection()
{
    ReturnValue result = initWinSock();
    CHECK_ERROR_RETURN_ERROR(result);

    result = initServerNameAndPortForServer();
    CHECK_ERROR_RETURN_ERROR(result);

    struct sockaddr_in local;
    local.sin_family = DEFAULT_ADDRESS_FAMILY;
    local.sin_addr.s_addr = INADDR_ANY;
    local.sin_port = htons(serverPort);

    SOCKET listenSocket = socket(DEFAULT_ADDRESS_FAMILY, DEFAULT_SOCKET_TYPE, DEFAULT_PROTOCOL);
    if (listenSocket == INVALID_SOCKET)
    {
        PRINT_ERROR_CALL_WSACLEANUP("Server", "bind()", WSAGetLastError());
        return PROJECT_ERROR;
    }

    int returnValue = bind(listenSocket, (struct sockaddr*)&local, sizeof(local));
    CHECK_SOCKET_ERROR_GOTO_CLEANUP(returnValue);

    returnValue = listen(listenSocket, MAXIMUM_ALLOWED_CONNECTIONS);
    CHECK_SOCKET_ERROR_GOTO_CLEANUP(returnValue);

    clientSocket = accept(listenSocket, NULL, NULL);
    CHECK_INVALID_SOCKET_GOTO_CLEANUP(clientSocket);

    // No longer need server socket, because we have 1 client
    SHUTDOWN_AND_CLOSE_SOCKET(listenSocket);
    return PROJECT_SUCCESS;

cleanup:
    SHUTDOWN_AND_CLOSE_SOCKET(listenSocket);
    PRINT_ERROR_CALL_WSACLEANUP("Server", "socketServerInitConnection()", returnValue);
    return PROJECT_ERROR;
}

ReturnValue socketServerCloseConnection()
{
    CHECK_INVALID_SOCKET_RETURN_ERROR(clientSocket);
    return socketCloseConnection(clientSocket);
}

unsigned int socketServerListen(char *buffer)
{
    CHECK_INVALID_SOCKET_RETURN_ZERO(clientSocket);
    CHECK_NULL_RETURN_ZERO(buffer);
    return socketReceive(clientSocket, buffer);
}

ReturnValue socketServerSend(const char *msg, unsigned int msgLength)
{
    CHECK_NULL_RETURN_ERROR(msg);
    CHECK_INVALID_SOCKET_RETURN_ERROR(clientSocket);
    return socketSend(msg, msgLength, clientSocket);
}

ReturnValue socketClientInitConnection() {
    ReturnValue result = initWinSock();
    CHECK_ERROR_RETURN_ERROR(result);

    result = initServerNameAndPortForClient();
    CHECK_ERROR_RETURN_ERROR(result);

    struct hostent *hp;
    hp = gethostbyname(clientServerName);
    if (hp == NULL)
    {
        PRINT_ERROR_CALL_WSACLEANUP("Client", "gethostbyname()", WSAGetLastError());
        return PROJECT_ERROR;
    }

    struct sockaddr_in server;
    memset(&server, 0, sizeof(server));
    memcpy(&(server.sin_addr), hp->h_addr, hp->h_length);
    server.sin_family = hp->h_addrtype;
    server.sin_port = htons(clientServerPort);

    connectionSocket = socket(DEFAULT_ADDRESS_FAMILY, DEFAULT_SOCKET_TYPE, DEFAULT_PROTOCOL);
    if (connectionSocket == INVALID_SOCKET)
    {
        PRINT_ERROR_CALL_WSACLEANUP("Client", "socket()", WSAGetLastError());
        return PROJECT_ERROR;
    }

    int connectionResult = connect(connectionSocket, (struct sockaddr*)&server, sizeof(server));
    if (connectionResult == SOCKET_ERROR)
    {
        SHUTDOWN_AND_CLOSE_SOCKET(connectionSocket);
        PRINT_ERROR_CALL_WSACLEANUP("Client", "connect()", WSAGetLastError());
        return PROJECT_ERROR;
    }

    return PROJECT_SUCCESS;
}

ReturnValue socketClientCloseConnection()
{
    CHECK_INVALID_SOCKET_RETURN_ERROR(connectionSocket);
    return socketCloseConnection(connectionSocket);
}

unsigned int socketClientSend(const char *msg, unsigned int msgLength, char *buffer)
{
    CHECK_INVALID_SOCKET_RETURN_ZERO(connectionSocket);
    CHECK_NULL_RETURN_ZERO(msg);
    CHECK_NULL_RETURN_ZERO(buffer);

    ReturnValue result = socketSend(msg, msgLength, connectionSocket);
    if (result == PROJECT_ERROR)
    {
        SHUTDOWN_AND_CLOSE_SOCKET(connectionSocket);
        PRINT_ERROR_CALL_WSACLEANUP_RETURN_ZERO("Client", "send()", WSAGetLastError());
    }

    return socketReceive(connectionSocket, buffer);
}
