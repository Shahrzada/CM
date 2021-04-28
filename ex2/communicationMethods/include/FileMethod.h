#pragma once

/********************************************************************************
 * @brief The file communicationMethods method implementation.
 *
 *
 * @section DESCRIPTION
 *
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

ReturnValue fileServerInitConnection();
ReturnValue fileServerCloseConnection();
unsigned int fileServerListen(char *buffer);
ReturnValue fileServerSend(const char *msg, unsigned int msgLength);

ReturnValue fileClientInitConnection();
ReturnValue fileClientCloseConnection();
unsigned int fileClientSend(const char *msg, unsigned int msgLength, char *buffer);
unsigned int fileClientReceive(char *buffer);
