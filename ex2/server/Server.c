#include "Server.h"

struct _Server {
    ServerCommunicationMethod *serverCMethod;
    CommunicationMethodCode cMethod;
};

Server *serverInitialize(CommunicationMethodCode cMethod)
{
//    Server * server;
    /* allocate memory for Server and call the following*/
//    ServerCommunicationMethod *serverCMethodSet(CommunicationMethodCode cMethod);
    /* to initiate its params */
//    serverConnect(server);
//    serverListen(server);
}

ReturnValue serverListen(Server *server)
{
    ReturnValue result = 0;
//    do {
//        Message *msg;
//        result = server->serverCMethod->listenFunction(msg);
//        serverHandleMessage(server, msg);
//    } while(result == SUCCESS)
    return SUCCESS;
}