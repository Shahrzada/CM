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

ReturnValue MPServerInitConnection(CommunicationMethodCode cMethodCode);
ReturnValue MPServerCloseConnection();

ReturnValue MPClientInitConnection(CommunicationMethodCode cMethodCode);
ReturnValue MPClientCloseConnection();

char *MPServerReceive();
ReturnValue MPServerSend(char *msg);
void MPServerSendSuccessOrFailure(ReturnValue result);

char *MPClientSend(const char *msg);
