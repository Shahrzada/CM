#include <unistd.h>
#include "Server.h"
#include "../communication/CommunicationMethods.h"

// -------------------------- const definitions -------------------------

struct _Server {
    ServerCommunicationMethod *serverCMethod;
    void *connectionData;
    CommunicationMethodCode cMethod;
    bool listen;
};

// ------------------------------ private functions -----------------------------

void serverRead()
{
    // send client all of the file's contents.
}

void serverWrite(Message * msg)
{
    CHECK_NULL_RETURN(msg);
    // write messageGetContents(msg) to file and answer with success/failure.
}


void serverHandleMessage(Server *server, Message * msg)
{
    CHECK_NULL_RETURN(server);
    CHECK_NULL_RETURN(msg);

    if (messageGetCommand(msg) == READ)
    {
        serverRead();
    }
    else if (messageGetCommand(msg) == WRITE)
    {
        serverWrite(msg);
    }
    else if (messageGetCommand(msg) == ABORT)
    {
        serverDisconnect(server);
    }
    else
    {
        // todo report error?
        return;
    }
}


// ------------------------------ functions -----------------------------

Server *serverInitialize(CommunicationMethodCode cMethod)
{
    ServerCommunicationMethod *serverCM = serverCMethodSet(FILE_METHOD);
    CHECK_NULL_RETURN_NULL(serverCM);

    Server *server = (Server *) malloc(sizeof(Server));
    CHECK_NULL_RETURN_NULL(serverCM);

    void *connectionData = serverCM->serverInitConnectionFunction();
    CHECK_NULL_RETURN_NULL(connectionData);

    server->serverCMethod = serverCM;
    server->connectionData = connectionData;
    server->cMethod = cMethod;
    server->listen = false;

    return server;
}

void serverListen(Server * server)
{
    CHECK_NULL_RETURN(server);
    server->listen = true;

    // todo: can't access server->listen like that
    while (server->listen)
    {
        Message * msg = messageAllocate();
        CHECK_NULL_RETURN(msg); // todo should print error
        server->serverCMethod->listenFunction(server->connectionData, msg);
        serverHandleMessage(server, msg);
        messageFree(msg);
        sleep(1);
    }
}

ReturnValue serverDisconnect(Server * server)
{
    CHECK_NULL_RETURN_ERROR(server);

    // todo: can't access server->listen like that
    server->listen = false;
    sleep(2);

    return server->serverCMethod->serverCloseConnectionFunction(server->connectionData);
}