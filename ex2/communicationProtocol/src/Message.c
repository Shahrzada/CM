
// ------------------------------ includes ------------------------------

#include "../include/Message.h"

// -------------------------- macros -------------------------

#define MSG_FORMAT_LENGTH 4
#define MSG_FORMAT_SENDER_POSITION 0
#define MSG_FORMAT_COMMAND_POSITION 2

// ------------------------------ private functions -----------------------------

static bool messageValidateSender(Sender sender)
{
    return (sender == SERVER || sender == CLIENT);
}

static bool messageValidateCommand(Command command)
{
    return (command == READ || command == WRITE
    || command == ABORT || command == REPLY);
}

// ------------------------------ functions -----------------------------

char *messageSet(Sender sender, Command commandType, char *contents)
{
    CHECK_NULL_RETURN_NULL(contents);

    int contentsLength = (int)strlen(contents);
    int msgLength = MSG_FORMAT_LENGTH + contentsLength + NULL_CHAR_SIZE;

    char *msg = (char *) malloc(sizeof(char) * msgLength);
    CHECK_NULL_RETURN_NULL(msg);

    // msg starts at [sender][,][command][,], for example "1,0," is for a client sending read command
    char msgPrefix[MSG_FORMAT_LENGTH] = {(char)(sender + ZERO_CHAR), COMMA_CHAR,
                                         (char)(commandType + ZERO_CHAR), COMMA_CHAR};
    strncpy(msg, msgPrefix, MSG_FORMAT_LENGTH);

    // adding the content
    char *pOutput = msg;
    pOutput += MSG_FORMAT_LENGTH;
    strncpy(pOutput, contents, contentsLength);
    free(contents);

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

    if (!messageValidateSender(msg[MSG_FORMAT_SENDER_POSITION] - ZERO_CHAR))
        return false;

    if (!messageValidateCommand(msg[MSG_FORMAT_COMMAND_POSITION] - ZERO_CHAR))
        return false;

    return true;
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

const char *messageGetContents(const char *msg)
{
    if (!messageValidateFormat(msg))
        return NULL;
    const char *pMsg = msg;
    return pMsg + MSG_FORMAT_LENGTH;
}

unsigned int messageGetContentsLength(const char *msg)
{
    if (!messageValidateFormat(msg))
        return 0;

    unsigned int length = strlen(msg) - MSG_FORMAT_LENGTH;

    if (length < 0)
        return 0;

    return length;
}