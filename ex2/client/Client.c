#include "Client.h"

struct _Client {
    ClientCommunicationMethod *clientCMethod;
    CommunicationMethodCode cMethod;
};

Client *clientInitialize(CommunicationMethodCode cMethod)
{
//    Client * client;
    /* allocate memory for Client and initiate its params */
//    ClientCommunicationMethod *clientCMethodSet(CommunicationMethodCode cMethod);
    /* connect it via the communication method */
//    ReturnValue result = client->clientCMethod->clientInitConnectionFunction();
//    return client;
    return NULL;
}

ReturnValue clientSendCommand(Client * client, Command commandType, char * msg)
{
//    Message *message = clientCreateMessageFromCommand(commandType, msg);
//    Message *reply;
//    ReturnValue result = client->clientCMethod->sendFunction(message, reply);
//    clientGenerateReplyToUser(msg, reply);
    return SUCCESS;
}