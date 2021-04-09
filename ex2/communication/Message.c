#include "Message.h"

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
}

Command messageGetCommand(Message * msg)
{
    if (msg == NULL)
    {
        return EMPTY_COMMAND;
    }
    return msg->commandType;
}
