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
#define SERVER_SUCCESS_MSG "0,3,SUCCESS"
#define SERVER_FAILURE_MSG "0,3,FAILURE"

// ------------------------------ functions -----------------------------


// setters
/* Whoever calls this function must free the allocated string */
char * messageSet(Sender sender, Command commandType, char *contents);
char *messageSetEmpty();
bool messageValidateFormat(const char *msg);

// getters
Command messageGetCommand(const char *msg);
Sender messageGetSender(const char *msg);
unsigned int messageGetLength(const char *msg);
const char *messageGetContents(const char *msg);
