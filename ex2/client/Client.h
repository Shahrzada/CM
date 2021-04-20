#pragma once

/********************************************************************************
 * @brief The client class.
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

#include "../communicationProtocol/include/Message.h"

// -------------------------- const definitions -------------------------


// ------------------------------ functions -----------------------------

/**
 * @brief initializes the client given the connection method
 *
 * @return PROJECT_SUCCESS if connection was established, PROJECT_ERROR ow.
 */
ReturnValue clientInitialize(CommunicationMethodCode cMethod);

/**
 * @brief closes the client connection
 *
 * @return PROJECT_SUCCESS if connection was closed, PROJECT_ERROR ow.
 */
ReturnValue clientClose();


/**
 * @brief given a string (already in a valid msg format), sends it to the server.
 *
 * @return PROJECT_SUCCESS if the msg was sent and the reply was PROJECT_SUCCESS, PROJECT_ERROR ow.
 */
ReturnValue clientSendCommand(const char *msg);
