
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

char *messageSet(Sender sender, Command commandType, char * contents)
{
    CHECK_NULL_RETURN_NULL(contents);

    int contentsLength = (int)strlen(contents);
    int msgLength = contentsLength + MSG_FORMAT_LENGTH;

    char *msg = (char *) malloc(sizeof(char)*msgLength);
    CHECK_NULL_RETURN_NULL(msg);

    msg[0] = (char)(sender + '0');
    msg[1] = COMMA_CHAR;
    msg[2] = (char)(commandType + '0');
    msg[3] = COMMA_CHAR;

    char *pOutput = msg;
    pOutput += MSG_FORMAT_LENGTH;
    strncpy(pOutput, contents, contentsLength);

    msg[msgLength] = NULL_CHAR;
    return msg;
}

char *messageSetEmpty()
{
    return messageSet(EMPTY_SENDER, EMPTY_COMMAND, "");
}

bool messageValidateFormat(char *msg) {
    CHECK_NULL_RETURN_FALSE(msg);

    unsigned int msgLength = strlen(msg);
    if (msgLength < MSG_FORMAT_LENGTH)
        return false;

    if (!messageValidateSender(msg[MSG_FORMAT_SENDER_POSITION] - ZERO_CHAR))
        return false;

    if (!messageValidateCommand(msg[MSG_FORMAT_COMMAND_POSITION] - ZERO_CHAR))
        return false;

    return true;
}

Sender messageGetSender(char *msg)
{
    if (msg == NULL || !messageValidateFormat(msg))
        return EMPTY_SENDER;
    return msg[MSG_FORMAT_SENDER_POSITION] - ZERO_CHAR;
}

Command messageGetCommand(char *msg)
{
    if (msg == NULL || !messageValidateFormat(msg))
        return EMPTY_COMMAND;
    return msg[MSG_FORMAT_COMMAND_POSITION] - ZERO_CHAR;
}

char *messageGetContents(char *msg)
{
    char *pMsg = msg;
    return pMsg + MSG_FORMAT_LENGTH;
}