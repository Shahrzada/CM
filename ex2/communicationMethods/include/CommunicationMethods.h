#pragma once

/********************************************************************************
 * @brief Creates and holds both communicationMethods methods for the client & the server.
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

// -------------------------- const definitions -------------------------

/* These are the "virtual functions" the communicationMethods methods must implement */
typedef ReturnValue (server_init_connection_function_t)();
typedef ReturnValue (server_close_connection_function_t)();
typedef ReturnValue (client_init_connection_function_t)();
typedef ReturnValue (client_close_connection_function_t)();
typedef ReturnValue (receive_function_t)(char *);
typedef ReturnValue (send_function_t)(char *, char *);

/* The program's server communicationMethods method struct */
typedef struct {
    server_init_connection_function_t *serverInitConnectionFunction;
    server_close_connection_function_t *serverCloseConnectionFunction;
    receive_function_t *receiveFunction;
    send_function_t *sendFunction;
} ServerCommunicationMethod;

/* The program's client communicationMethods method struct */
typedef struct {
    client_init_connection_function_t *clientInitConnectionFunction;
    client_close_connection_function_t *clientCloseConnectionFunction;
    send_function_t *sendFunction;
} ClientCommunicationMethod;

// ------------------------------ functions -----------------------------

ServerCommunicationMethod *serverCMethodSet(CommunicationMethodCode cMethod);
// if serverCMethod is null then allocate and set it, ow return the existing one

ClientCommunicationMethod *clientCMethodSet(CommunicationMethodCode cMethod);
// if clientCMethod is null then allocate and set it, ow return the existing one