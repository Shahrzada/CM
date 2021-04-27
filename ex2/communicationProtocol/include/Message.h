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
    char * contents;
};
typedef struct _Message Message;

// -------------------------- macros -------------------------

#define MSG_FORMAT_LENGTH 4

#define SERVER_SUCCESS_MSG "0,3,PROJECT_SUCCESS"
#define SERVER_FAILURE_MSG "0,3,FAILURE"

#define PRINT_MSG(msg) do {printf("[%d][%d][%d]:%s\n", (msg)->sender, (msg)->command, (msg)->contentsLength, (msg)->contents);} while(0)
#define MSG_CHECK_VALID_RETURN_NULL(msg) do {if (!messageValidateFormat(msg)) return NULL;} while(0)
#define MSG_CHECK_VALID_RETURN_ERROR(msg) do {if (!messageValidateFormat(msg)) return PROJECT_ERROR;} while(0)
#define MSG_CHECK_VALID_GOTO_CLEANUP(msg) do {if (!messageValidateFormat(msg)) goto cleanup;} while(0)

// ------------------------------ functions -----------------------------

/**
 * @brief allocates and creates a formatted msg for the protocol given its params.
 * The caller of this function is responsible to freeing the memory of the msg.
 *
 * @return the formatted msg as a char* if succeeded, NULL ow.
 */

Message *messageSet(Sender sender, Command command, unsigned int contentsLength, char *contents);
void messageFree(Message *msg);
bool messageValidateFormat(Message *msg);
char *messageToCString(Message *msg, unsigned int *msgStrLength);
Message *messageFromCString(const char *msgStr, unsigned int msgLength);

/**
 * @brief checks for NULL and validates the prefix is according to the format
 *
 * @return true if the msg is formatted correctly, false ow.
 */

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

unsigned int messageGetContentsLength(Message *msg);


