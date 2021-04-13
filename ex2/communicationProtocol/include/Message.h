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

// -------------------------- const definitions -------------------------


// ------------------------------ functions -----------------------------


// setters
char * messageSet(Sender sender, Command commandType, char *contents);
char *messageSetEmpty();
bool messageValidateFormat(char *msg);

// getters
Command messageGetCommand(char *msg);
Sender messageGetSender(char *msg);
unsigned int messageGetLength(char *msg);
char *messageGetContents(char *msg);
