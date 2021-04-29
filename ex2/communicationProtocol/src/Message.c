
// ------------------------------ includes ------------------------------

#include <time.h>
#include "Message.h"

// -------------------------- macros -------------------------

#define SUCCESS_STR "Success"
#define FAILURE_STR "Failure"
#define SUCCESS_OR_FAILURE_STR_LENGTH 7

#define CHECK_MSG_NULL_RETURN_NULL(msg) do { \
           CHECK_NULL_RETURN_NULL(msg); \
           CHECK_NULL_RETURN_NULL((msg)->contents); \
           } while(0)

#define CHECK_MSG_NULL_RETURN_ZERO(msg) do { \
           CHECK_NULL_RETURN_ZERO(msg); \
           CHECK_NULL_RETURN_ZERO((msg)->contents); \
           } while(0)

// ------------------------------ private functions -----------------------------

static bool messageValidateSender(Sender sender)
{
    return (sender == SERVER || sender == CLIENT);
}

static bool messageValidateCommand(Command command)
{
    return (command == READ || command == WRITE
    || command == ABORT || command == REPLY || command == GET_FILE);
}

static bool messageValidateContentsLength(unsigned int contentsLength)
{
    return (contentsLength < MAX_MSG_LENGTH - sizeof(Message));
}

// ------------------------------ functions -----------------------------

Message *messageSet(Sender sender, Command command, unsigned int contentsLength, char *contents)
{
    CHECK_NULL_RETURN_NULL(contents);
    if (!messageValidateCommand(command)|| !messageValidateSender(sender)
        || !messageValidateContentsLength(contentsLength))
        return NULL;

    Message *msg = NULL;
    char *msgContents = NULL;

    msg = (Message *) malloc(sizeof(Message));
    CHECK_NULL_GOTO_CLEANUP(msg);

    msgContents = (char *) malloc(sizeof(char) * (contentsLength + NULL_CHAR_SIZE));
    CHECK_NULL_GOTO_CLEANUP(msgContents);

    memcpy(msgContents, contents, sizeof(char) * contentsLength);
    msgContents[contentsLength] = NULL_CHAR;

    msg->sender = sender;
    msg->command = command;
    msg->contentsLength = contentsLength;
    msg->contents = msgContents;

    return msg;

cleanup:
    free(msg);
    free(msgContents);
    return NULL;

}

void messageFree(Message *msg)
{
    if (msg == NULL)
        return;

    if (msg->contents != NULL)
        free(msg->contents);

    free(msg);
}

bool messageValidateFormat(Message *msg)
{
    CHECK_MSG_NULL_RETURN_NULL(msg);
    return (messageValidateSender(msg->sender)
            && messageValidateCommand(msg->command)
            && messageValidateContentsLength(msg->contentsLength));
}

char *messageToCString(Message *msg, unsigned int *msgStrLength)
{
    MSG_CHECK_VALID_RETURN_NULL(msg);
    CHECK_NULL_RETURN_NULL(msgStrLength);

    size_t msgLength = sizeof(Message) + msg->contentsLength;
    char *msgStr = (char *) malloc(sizeof(char) * (msgLength + NULL_CHAR_SIZE));
    CHECK_NULL_RETURN_ZERO(msgStr);

    memcpy(msgStr, msg, sizeof(Message));
    memcpy(msgStr + sizeof(Message), msg->contents, msg->contentsLength);
    msgStr[msgLength] = EOL_CHAR;

    *msgStrLength = msgLength;
    return msgStr;
}

Message *messageFromCString(const char *msgStr, unsigned int msgLength)
{
    CHECK_NULL_RETURN_NULL(msgStr);

    // Copy Message data
    Message *msg = (Message *) malloc(sizeof(Message));
    CHECK_NULL_RETURN_NULL(msg);
    memcpy(msg, msgStr, sizeof(Message));

    // Copy contents & update the pointer
    unsigned int contentsLength = msgLength - sizeof(Message);
    char *contents = (char *) malloc(contentsLength + NULL_CHAR_SIZE);
    if (contents == NULL)
    {
        free(msg);
        return NULL;
    }

    memcpy(contents, msgStr + sizeof(Message), contentsLength);
    contents[contentsLength] = NULL_CHAR;
    msg->contents = contents;

    // Validate
    if (!messageValidateFormat(msg))
    {
        free(msg);
        free(contents);
        return NULL;
    }

    return msg;
}

Sender messageGetSender(Message *msg)
{
    if (!messageValidateFormat(msg))
        return EMPTY_SENDER;
    return msg->sender;
}

Command messageGetCommand(Message *msg)
{
    if (!messageValidateFormat(msg))
        return EMPTY_COMMAND;
    return msg->command;
}

char *messageGetContents(Message *msg)
{
    MSG_CHECK_VALID_RETURN_NULL(msg);
    return msg->contents;
}

unsigned int messageGetContentsLength(Message *msg)
{
    MSG_CHECK_VALID_RETURN_ZERO(msg);
    return msg->contentsLength;
}

ReturnValue messageToPrintCString(Message *msg, char *buffer)
{
    MSG_CHECK_VALID_RETURN_ERROR(msg);
    CHECK_NULL_RETURN_ERROR(buffer);

    sprintf(buffer, "[TIME=%lu]:[SENDER=%d][COMMAND=%d][LENGTH=%d]"
                    "\n\t\t[MSG AS CSTRING=%s]\n",
//                    "\n\t\t[The msg as hex string]:\t[%s]\n",
                    (unsigned long)time(NULL), msg->sender, msg->command,
                    msg->contentsLength, msg->contents);

    return PROJECT_SUCCESS;
}

Message *messageSetSuccessOrFailure(Sender sender, Command command, bool isSuccess)
{
    if (!messageValidateSender(sender) || !messageValidateCommand(command))
        return NULL;

    if (isSuccess)
        return messageSet(sender, command, SUCCESS_OR_FAILURE_STR_LENGTH, SUCCESS_STR);

    return messageSet(sender, command, SUCCESS_OR_FAILURE_STR_LENGTH, FAILURE_STR);
}