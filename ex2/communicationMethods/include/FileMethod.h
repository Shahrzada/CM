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

#include "../../Macros.h"

// ------------------------------ functions -----------------------------

ReturnValue fileServerInitConnection();
ReturnValue fileServerCloseConnection();
char *fileListen();
ReturnValue fileSend(const char *msg);

ReturnValue fileClientInitConnection();
ReturnValue fileClientCloseConnection();
char *fileClientSend(const char *msg);
