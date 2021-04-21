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

#include "Macros.h"

// ------------------------------ functions -----------------------------

/**
 * @brief initializes the server given the connection method
 *
 * @return PROJECT_SUCCESS if the connection was established, PROJECT_ERROR ow.
 */
ReturnValue serverInitialize(CommunicationMethodCode cMethod);

/**
 * @brief listens to the connection until interrupted or receives ABORT.
 */
_Noreturn void serverListen();
