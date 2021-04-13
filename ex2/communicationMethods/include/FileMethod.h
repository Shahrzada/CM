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

// -------------------------- const definitions -------------------------



// ------------------------------ functions -----------------------------

ReturnValue fileServerInitConnect();
ReturnValue fileServerCloseConnection();
ReturnValue fileClientInitConnect();
ReturnValue fileClientCloseConnect();
char *fileListen();
ReturnValue fileServerSend(char *msg);
char *fileClientSend(char *msg);