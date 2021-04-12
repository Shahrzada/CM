#pragma once

/********************************************************************************
 * @brief The server class.
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

#include "../Macros.h"

// -------------------------- const definitions -------------------------


// ------------------------------ functions -----------------------------

/**
 * @brief initializes the server given the connection method
 *
 * RETURN VALUE:
 * @return a pointer to the new server, or NULL if the initialization failed.
 */
ReturnValue serverInitialize(CommunicationMethodCode cMethod);

void serverListen();

ReturnValue serverDisconnect();

/* These should be the only reachable functions, as the server works independently according
 * to client commands. */
