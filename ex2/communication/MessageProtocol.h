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

#include "Message.h"

// -------------------------- const definitions -------------------------



// ------------------------------ functions -----------------------------

ReturnValue getServerCMethod(CommunicationMethodCode cMethod);
ReturnValue getClientCMethod(CommunicationMethodCode cMethod);

ReturnValue msgServerInitConnect(CommunicationMethodCode cMethod);
ReturnValue msgServerCloseConnection();

ReturnValue clientInitConnect(CommunicationMethodCode cMethod);
ReturnValue clientCloseConnect();

ReturnValue msgServerReceive(Message *msg);

ReturnValue send(Message *msg, Message *reply);
