#pragma once

/********************************************************************************
 * @brief The file communication method implementation.
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

struct _FileData;
typedef struct _FileData FileData;


// ------------------------------ functions -----------------------------

FileData *fileServerInitConnect();
ReturnValue fileServerCloseConnection(FileData * fileData);
ReturnValue fileClientInitConnect();
ReturnValue fileListen(FileData  *fileData, Message *msg);
ReturnValue fileSend(Message * msg, Message * reply);