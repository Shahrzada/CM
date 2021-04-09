#pragma once

/********************************************************************************
 * @brief Creates and holds both communication methods for the client & the server.
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

/* These are the "virtual functions" the communication methods must implement */
typedef void *(server_init_connection_function_t)();
typedef ReturnValue (server_close_connection_function_t)(void *);
typedef ReturnValue (client_init_connection_function_t)();
typedef ReturnValue (client_close_connection_function_t)();
typedef ReturnValue (listen_function_t)(void*, Message *);
typedef ReturnValue (send_function_t)(Message *, Message *);

/* The program's server communication method struct */
typedef struct {
    server_init_connection_function_t *serverInitConnectionFunction;
    server_close_connection_function_t *serverCloseConnectionFunction;
    listen_function_t *listenFunction;
    send_function_t *sendFunction;
} ServerCommunicationMethod;

/* The program's client communication method struct */
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