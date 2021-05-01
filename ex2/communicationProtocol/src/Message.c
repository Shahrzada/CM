
// ------------------------------ includes ------------------------------

#include <time.h>
#include "Message.h"

// -------------------------- macros -------------------------

#define SUCCESS_STR "Success"
#define FAILURE_STR "Failure"
#define SUCCESS_OR_FAILURE_STR_LENGTH 7

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
    return (msg != NULL
            && messageValidateSender(msg->sender)
            && messageValidateCommand(msg->command)
            && messageValidateContentsLength(msg->contentsLength));
}

char *messageToCString(Message *msg, unsigned int *msgStrLength)
{
    MSG_CHECK_VALID_RETURN_NULL(msg);
    CHECK_NULL_RETURN_NULL(msgStrLength);

    size_t msgLength = sizeof(Message) + msg->contentsLength;
    char *msgStr = (char *) malloc(sizeof(char) * (msgLength + NULL_CHAR_SIZE));
    CHECK_NULL_RETURN_NULL(msgStr);

    // We first copy the Message object memory into the array
    memcpy(msgStr, msg, sizeof(Message));

    // The we attach a copy of the contents themselves
    memcpy(msgStr + sizeof(Message), msg->contents, msg->contentsLength);
    msgStr[msgLength] = EOL_CHAR;

    *msgStrLength = msgLength;
    return msgStr;
}

Message *messageFromCString(const char *msgStr, unsigned int msgLength)
{
    CHECK_NULL_RETURN_NULL(msgStr);
    CHECK_ZERO_RETURN_NULL(msgLength);
    Message *msg = NULL;
    char *contents = NULL;

    // Copy Message data
    msg = (Message *) malloc(sizeof(Message));
    CHECK_NULL_RETURN_NULL(msg);
    memcpy(msg, msgStr, sizeof(Message));

    // Copy contents & update the msgs' pointer
    unsigned int contentsLength = msgLength - sizeof(Message);
    contents = (char *) malloc(contentsLength + NULL_CHAR_SIZE);
    CHECK_NULL_GOTO_CLEANUP(contents);

    memcpy(contents, msgStr + sizeof(Message), contentsLength);
    contents[contentsLength] = NULL_CHAR;
    msg->contents = contents;

    // Validation is important
    if (!messageValidateFormat(msg))
        goto cleanup;

    return msg;

    cleanup:
    free(msg);
    free(contents);
    return NULL;
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

ReturnValue messageToPrintableCString(Message *msg, char *buffer)
{
    MSG_CHECK_VALID_RETURN_ERROR(msg);
    CHECK_NULL_RETURN_ERROR(buffer);

    sprintf(buffer, "[TIME=%lu]:[SENDER=%d][COMMAND=%d][LENGTH=%d]\n\t\t[MSG AS CSTRING=%s]\n",
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

char *messageIntoEncodedString(Message *msg, encoding_function *encodingFunction)
{
    MSG_CHECK_VALID_RETURN_NULL(msg);
    CHECK_NULL_RETURN_NULL(encodingFunction);

    // Get the msg as a cString
    unsigned int msgStrLength = 0;
    char *msgStr = messageToCString(msg, &msgStrLength);
    CHECK_NULL_RETURN_NULL(msgStr);

    // Encode le msg
    char *encodedMsg = (char *) malloc(MAX_MSG_LENGTH);
    CHECK_NULL_RETURN_NULL(encodedMsg);

    int encodedMsgLength = encodingFunction(encodedMsg, msgStr, (int)msgStrLength);
    free(msgStr);
    if (encodedMsgLength == 0)
    {
        free(encodedMsg);
        PRINT_ERROR_WITH_FUNCTION_AND_RETURN_NULL("messageIntoEncodedString", "Error with encoding");
    }

    encodedMsg = realloc(encodedMsg, encodedMsgLength);
    return encodedMsg;
}

Message *messageDecodeStringIntoMsg(char *encodedMsg, decoding_function *decodingFunction)
{
    CHECK_NULL_RETURN_NULL(encodedMsg);
    CHECK_NULL_RETURN_NULL(decodingFunction);
    Message *msg = NULL;

    // decode the data
    char decodedMsg[MAX_MSG_LENGTH];
    unsigned int msgLength = decodingFunction(decodedMsg, encodedMsg);
    CHECK_ZERO_RETURN_NULL(msgLength);

    // create the msg object
    return messageFromCString(decodedMsg, msgLength);
}
