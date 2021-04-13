#pragma once

/********************************************************************************
 * @brief The socket communicationMethods method implementation.
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

#include "communicationProtocol/include/Message.h"

// -------------------------- const definitions -------------------------


// ------------------------------ functions -----------------------------

ReturnValue socketServerConnect();
ReturnValue socketServerClose();
ReturnValue socketClientConnect();
ReturnValue socketListen(char* msg);
ReturnValue socketSend(char *msg, char *reply);


