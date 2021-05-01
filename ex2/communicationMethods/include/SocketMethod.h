#pragma once

/********************************************************************************
 * @brief The socket communicationMethods method implementation.
 *
 *
 * @section created by using microsoft docs (with c related corrections):
 * https://docs.microsoft.com/en-us/windows/win32/winsock
 *
 *
 *
 * Error handling
 * ~~~~~~~~~~~~~~
 * Most functions may fail due to failure to allocate dynamic memory. When
 * this happens the functions will return an appropriate failure value. If this
 * happens, then the state of the other outputs of the function is undefined.
 ********************************************************************************/

// ------------------------------ includes ------------------------------

#include "Macros.h"

// ------------------------------ functions -----------------------------

ReturnValue socketServerInitConnection();
ReturnValue socketServerCloseConnection();
unsigned int socketServerListen(char *buffer);
ReturnValue socketServerSend(const char *msg, unsigned int msgLength);

ReturnValue socketClientInitConnection();
ReturnValue socketClientCloseConnection();
unsigned int socketClientSend(const char *msg, unsigned int msgLength, char *buffer);
