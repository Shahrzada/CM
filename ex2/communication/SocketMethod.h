#pragma once

/********************************************************************************
 * @brief The socket communication method implementation.
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

#include "Message.h"

// -------------------------- const definitions -------------------------


// ------------------------------ functions -----------------------------

ReturnValue socketServerConnect();
ReturnValue socketServerClose();
ReturnValue socketClientConnect();
ReturnValue socketListen(Message * msg);
ReturnValue socketSend(Message * msg, Message * reply);


