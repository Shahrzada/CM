#include "Message.h"
#include "CommunicationMethods.h"

#define CHECK_MESSAGE_NULL_RETURN_NULL(msg) do { \
           CHECK_NULL_RETURN_NULL(msg);\
           CHECK_NULL_RETURN_NULL(msg->contents);\
           } while(0)

#define CHECK_MESSAGE_NULL_RETURN_ERROR(msg) do { \
           CHECK_NULL_RETURN_ERROR(msg);\
           CHECK_NULL_RETURN_ERROR(msg->contents);\
           } while(0)


struct _Message {
    Command commandType;
    Sender sender;
    unsigned int length;
    char * contents;
    /* time stamp? */
};

static ServerCommunicationMethod *serverCMethod = NULL;
static ClientCommunicationMethod *clientCMethod = NULL;

Message *messageAllocate()
{
    Message *msg = (Message *) malloc(sizeof(Message));
    CHECK_NULL_RETURN_NULL(msg);
    msg->contents = (char *) malloc(sizeof(char));
    CHECK_NULL_GOTO_CLEANUP(msg->contents);
    msg->commandType = EMPTY_COMMAND;
    msg->sender = EMPTY_SENDER;
    msg->length = 0;
    return msg;

cleanup:
    free(msg);
    return NULL;
}

void messageFree(Message * msg)
{
    CHECK_NULL_RETURN(msg);
    if (msg->contents != NULL)
        free(msg->contents);
    free(msg);
}

ReturnValue messageSet(Message * msg, Command commandType, Sender sender,
                        unsigned int length, char * contents)
{
    CHECK_MESSAGE_NULL_RETURN_ERROR(msg);
    CHECK_NULL_RETURN_ERROR(contents);
    free(msg->contents);

    msg->commandType = commandType;
    msg->sender = sender;
    msg->length = length;
    msg->contents = contents;
    return SUCCESS;
}

ReturnValue messageFromCString(Message * msg, const char * cStr)
{
    CHECK_MESSAGE_NULL_RETURN_ERROR(msg);
    CHECK_NULL_RETURN_ERROR(cStr);

    unsigned int contentsLength = strlen(cStr) + NULL_CHAR_SIZE;
    char * contents = (char *) malloc(sizeof(char)*contentsLength);
    CHECK_NULL_RETURN_ERROR(contents);
    return SUCCESS;
}

char *messageToCString(Message * msg)
{
    CHECK_NULL_RETURN_NULL(msg);
    return "";
}

ReturnValue getServerCMethod(CommunicationMethodCode cMethod)
{
    serverCMethod = serverCMethodSet(cMethod);
    CHECK_NULL_RETURN_ERROR(serverCMethod);
    return SUCCESS;
}

ReturnValue getClientCMethodSet(CommunicationMethodCode cMethod)
{
    clientCMethod = clientCMethodSet(cMethod);
    CHECK_NULL_RETURN_ERROR(clientCMethod);
    return SUCCESS;
}

Command messageGetCommand(Message * msg)
{
    if (msg == NULL)
    {
        return EMPTY_COMMAND;
    }
    return msg->commandType;
}

ReturnValue msgServerInitConnect(CommunicationMethodCode cMethod)
{
    getServerCMethod(cMethod);
    CHECK_NULL_RETURN_ERROR(serverCMethod);
    return serverCMethod->serverInitConnectionFunction();
}

ReturnValue msgServerCloseConnection()
{
    return serverCMethod->serverCloseConnectionFunction();
}

ReturnValue clientInitConnect(CommunicationMethodCode cMethod)
{
    return ERROR;
}

ReturnValue clientCloseConnect()
{
    return ERROR;
}


ReturnValue msgServerReceive(Message *msg)
{
    CHECK_NULL_RETURN_ERROR(msg);

    char * incomingMsg = (char *) malloc(sizeof(char));
    CHECK_NULL_RETURN_ERROR(incomingMsg);

    ReturnValue result = serverCMethod->receiveFunction(incomingMsg);
    CHECK_ERROR_GOTO_CLEANUP(result);

    result = messageFromCString(msg, incomingMsg);
    free(incomingMsg);
    return result;

cleanup:
    free(incomingMsg);
    return ERROR;
}


ReturnValue send(Message *msg, Message *reply)
{
    return ERROR;
}

