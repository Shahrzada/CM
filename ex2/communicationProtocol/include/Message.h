#pragma once

/********************************************************************************
 * @brief A message formatter.
 *
 *
 * @section DESCRIPTION
 *
 * Error handling
 * ~~~~~~~~~~~~~~
 * Most functions may fail due to failure to allocate dynamic memory. When
 * this happens the functions will return an appropriate failure value. If this
 * happens, then the state of the other outputs of the function is undefined.
 ********************************************************************************/

// ------------------------------ includes ------------------------------

#include "Macros.h"

// -------------------------- const definitions -------------------------

struct _Message {
    Sender sender;
    Command command;
    unsigned int contentsLength;
    char contents[];
};
typedef struct _Message Message;

typedef int (encoding_function)(char *, const char *, int);
typedef int (decoding_function)(char *, const char *);

// -------------------------- macros -------------------------

#define MSG_FORMAT_LENGTH 4
#define MSG_PRINT_FORMAT_LENGTH 100

#define MSG_CHECK_VALID_RETURN_VALUE(msg, value) do {if (!messageValidateFormat(msg)) return (value);} while(0)
#define MSG_CHECK_VALID_RETURN(msg) do {if (!messageValidateFormat(msg)) return;} while(0)
#define MSG_CHECK_VALID_GOTO_CLEANUP(msg) do {if (!messageValidateFormat(msg)) goto cleanup;} while(0)

#define PRINT_MSG(msg) do {printf("[%d][%d][%d]:%s\n", (msg)->sender, (msg)->command, (msg)->contentsLength, (msg)->contents);} while(0)

// ------------------------------ functions -----------------------------

/**
 * @brief allocates and creates a formatted msg for the protocol given its params.
 * The caller of this function is responsible to freeing the memory of the msg.
 *
 * @return a Message object according to the input if succeeded, NULL ow.
 */
Message *messageSet(Sender sender, Command command, unsigned int contentsLength, char *contents);

void messageFree(Message *msg);

/** Verifies the sender, command and the the msg length is withing the MAX_MSG_LENGTH */
bool messageValidateFormat(Message *msg);

/** If succeeded, returns the char * and updates msgStrLength accordingly, returns NULL ow. */
char *messageToCString(Message *msg, unsigned int *msgStrLength);
Message *messageFromCString(const char *msgStr, unsigned int msgLength);

/**
 * @brief extracts the sender from the msg according the format
 *
 * @return the relevant sender if msg is valid, EMPTY_SENDER ow.
 */
Sender messageGetSender(Message *msg);

/**
 * @brief extracts the command from the msg according the format
 *
 * @return the relevant command if msg is valid, EMPTY_COMMAND ow.
 */
Command messageGetCommand(Message *msg);

/**
 * @brief extracts the contents from the msg according the format
 *
 * @return the relevant contents if msg is valid, NULL ow.
 */
char *messageGetContents(Message *msg);

/** Returns the length of the contents of the msg */
unsigned int messageGetContentsLength(Message *msg);

/** Unloads the msg object into a readable cstring for human eyes
 * (mainly for logs). We assume the buffer is large enough. **/
ReturnValue messageToPrintableCString(Message *msg, char *buffer);

/**
 * Creates a simple [][][]Success or [][][]Failure msg object.
 * The caller of this function is responsible to freeing the memory of the msg.
 */
Message *messageSetSuccessOrFailure(Sender sender, Command command, bool isSuccess);

char *messageIntoEncodedString(Message *msg, encoding_function *encodingFunction);
Message *messageDecodeStringIntoMsg(char *encodedMsg, decoding_function *decodingFunction);
