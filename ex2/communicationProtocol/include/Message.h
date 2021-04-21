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

#include "../../Macros.h"

// -------------------------- macros -------------------------

#define MSG_FORMAT_LENGTH 4

#define SERVER_SUCCESS_MSG "0,3,PROJECT_SUCCESS"
#define SERVER_FAILURE_MSG "0,3,FAILURE"

// ------------------------------ functions -----------------------------

/**
 * @brief allocates and creates a formatted msg for the protocol given its params.
 * The caller of this function is responsible to freeing the memory of the msg.
 *
 * @return the formatted msg as a char* if succeeded, NULL ow.
 */
char * messageSet(Sender sender, Command commandType, char *contents);

/**
 * @brief calls the messageSet with the empty sender, command and string.
 *
 * @return the formatted msg as a char* if succeeded, NULL ow.
 */
char *messageSetEmpty();

/**
 * @brief checks for NULL and validates the prefix is according to the format
 *
 * @return true if the msg is formatted correctly, false ow.
 */
bool messageValidateFormat(const char *msg);

/**
 * @brief extracts the sender from the msg according the format
 *
 * @return the relevant sender if msg is valid, EMPTY_SENDER ow.
 */
Sender messageGetSender(const char *msg);

/**
 * @brief extracts the command from the msg according the format
 *
 * @return the relevant command if msg is valid, EMPTY_COMMAND ow.
 */
Command messageGetCommand(const char *msg);

/**
 * @brief extracts the contents from the msg according the format
 *
 * @return the relevant contents if msg is valid, NULL ow.
 */
char *messageGetContents(char *msg);

/**
 * @brief returns the total chars in contents
 */
unsigned int messageGetContentsLength(const char *msg);
