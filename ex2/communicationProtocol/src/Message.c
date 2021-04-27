
// ------------------------------ includes ------------------------------

#include "Message.h"

// -------------------------- macros -------------------------

#define MSG_FORMAT_SENDER_POSITION 0
#define MSG_FORMAT_COMMAND_POSITION 2

// *3 for: sender, command and length
#define GET_SIZE_OF_MSG_FORMAT(size) do {(size) = sizeof(unsigned int)*3 + NULL_CHAR_SIZE;} while(0)

#define CHECK_MSG_NULL_RETURN_NULL(msg) do { \
           CHECK_NULL_RETURN_NULL(msg); \
           CHECK_NULL_RETURN_NULL((msg)->contents); \
           } while(0)

#define CHECK_MSG_NULL_RETURN_ERROR(msg) do { \
           CHECK_NULL_RETURN_ERROR(msg); \
           CHECK_NULL_RETURN_ERROR((msg)->contents); \
           } while(0)

#define CHECK_MSG_NULL_RETURN_ZERO(msg) do { \
           CHECK_NULL_RETURN_ZERO(msg); \
           CHECK_NULL_RETURN_ZERO((msg)->contents); \
           } while(0)

// -------------------------- const definitions -------------------------

struct _Message {
    Sender sender;
    Command command;
    unsigned int contentsLength;
    char * contents;
};

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

// this is done dynamically because of sizeof(unsigned int)
static bool messageValidateContentsLength(unsigned int contentsLength)
{
    size_t msgFormatSize = 0;
    GET_SIZE_OF_MSG_FORMAT(msgFormatSize);
    return (contentsLength < MAX_MSG_LENGTH - msgFormatSize);
}

// ------------------------------ functions -----------------------------


Message *messageSetT(Sender sender, Command command, unsigned int contentsLength, char *contents)
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
    strncpy(msgContents, contents, contentsLength);
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

bool messageValidateFormatT(Message *msg)
{
    CHECK_MSG_NULL_RETURN_NULL(msg);
    return (messageValidateSender(msg->sender)
            && messageValidateCommand(msg->command)
            && messageValidateContentsLength(msg->contentsLength));
}

char *messageToCString(Message *msg, unsigned int *msgStrLength)
{
    CHECK_MSG_NULL_RETURN_ZERO(msg);
    if (!messageValidateFormatT(msg))
        return 0;

    size_t msgLength = sizeof(Message) + msg->contentsLength;
    char *msgStr = (char *) malloc(sizeof(char) * (msgLength + NULL_CHAR_SIZE));
    CHECK_NULL_RETURN_ZERO(msgStr);

    memcpy(msgStr, msg, sizeof(Message));
    memcpy(msgStr + sizeof(Message), msg->contents, msg->contentsLength);
    // TODO add a validating element - compare

    Message *msg1 = (Message *) malloc(msgLength);
    memcpy(msg1, msgStr, msgLength);

    msgStr[msgLength] = NULL_CHAR;
    *msgStrLength = msgLength;
    return msgStr;
}

Message *messageFromCString(const char *msgStr, unsigned int msgLength)
{
    CHECK_NULL_RETURN_NULL(msgStr);

    // copy data
    Message *msg = (Message *) malloc(sizeof(Message));
    CHECK_NULL_RETURN_NULL(msg);
    memcpy(msg, msgStr, sizeof(Message));

    // update contents pointer
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

    if (!messageValidateFormatT(msg))
    {
        free(msg);
        free(contents);
        return NULL;
    }
    return msg;
}

char *messageSet(Sender sender, Command commandType, char *contents)
{
    CHECK_NULL_RETURN_NULL(contents);

    // CR: never use str function with giving it a limit
    // CR: why not just use size_t?
    int contentsLength = (int)strlen(contents);
    int msgLength = MSG_FORMAT_LENGTH + contentsLength + NULL_CHAR_SIZE;

    char *msg = (char *) malloc(sizeof(char) * msgLength);
    CHECK_NULL_RETURN_NULL(msg);

    // CR: this is why you should use structs, the message format is hidden inside messageSet
    // msg starts at [sender][,][command][,], for example "1,0," is for a client sending read command
    // CR: sender + ZERO_CHAR you be a macro
    // CR: Someone one day will curse you if there will be more than 10 commands
    char msgPrefix[MSG_FORMAT_LENGTH] = {(char)(sender + ZERO_CHAR), COMMA_CHAR,
                                         (char)(commandType + ZERO_CHAR), COMMA_CHAR};
    strncpy(msg, msgPrefix, MSG_FORMAT_LENGTH);

    // adding the content
    // CR: why the two lines?
    char *pOutput = msg;
    pOutput += MSG_FORMAT_LENGTH;
    strncpy(pOutput, contents, contentsLength);

    msg[msgLength - 1] = NULL_CHAR;
    return msg;
}

char *messageSetEmpty()
{
    return messageSet(EMPTY_SENDER, EMPTY_COMMAND, "");
}

bool messageValidateFormat(const char *msg) {
    CHECK_NULL_RETURN_FALSE(msg);

    // msg length must be at least equal to MSG_FORMAT_LENGTH
    unsigned int msgLength = strlen(msg);
    if (msgLength < MSG_FORMAT_LENGTH)
        return false;
    // CR: you should use structs to do this kind of parsing
    if (!messageValidateSender(msg[MSG_FORMAT_SENDER_POSITION] - ZERO_CHAR))
        return false;

    if (!messageValidateCommand(msg[MSG_FORMAT_COMMAND_POSITION] - ZERO_CHAR))
        return false;

    return true;
}

Sender messageGetSender(Message *msg)
{
    if (!messageValidateFormatT(msg))
        return EMPTY_SENDER;
    return msg->sender;
}

Command messageGetCommand(Message *msg)
{
    if (!messageValidateFormatT(msg))
        return EMPTY_COMMAND;
    return msg->command;
}

char *messageGetContents(Message *msg)
{
    if (!messageValidateFormatT(msg))
        return NULL;
    return msg->contents;
}

Sender messageGetSender(const char *msg)
{
    if (!messageValidateFormat(msg))
        return EMPTY_SENDER;
    return msg[MSG_FORMAT_SENDER_POSITION] - ZERO_CHAR;
}

Command messageGetCommand(const char *msg)
{
    if (!messageValidateFormat(msg))
        return EMPTY_COMMAND;
    return msg[MSG_FORMAT_COMMAND_POSITION] - ZERO_CHAR;
}

char *messageGetContents(char *msg)
{
    if (!messageValidateFormat(msg))
        return NULL;
    char *pMsg = msg; // cr: Why does this variable exist?
    return pMsg + MSG_FORMAT_LENGTH;
}
