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
 * RETURN VALUE:
 * @return a pointer to the new client, or NULL if the initialization failed.
 */
ReturnValue clientInitialize(CommunicationMethodCode cMethod);
ReturnValue clientKill();

/* Request to send a command to the server, should return the server's reply
 * e.g. if sent 'read' then reply == whatever it read and msg SUCCESS.
 * e.g. if sent 'write' then msg will contain the msg to be sent, (it
 * shouldn't change) return SUCCESS. */
ReturnValue clientSendCommand(Message *msg);
