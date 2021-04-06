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

#include "FileMethod.h"
#include "SocketMethod.h"
/* This should be the only file (besides main due to input) that will
 * be edited after adding another communication method. */

// -------------------------- const definitions -------------------------

/* These are the "virtual functions" the communication methods must implement */
typedef ReturnValue (server_connect_function_t)();
typedef ReturnValue (server_close_function_t)();
typedef ReturnValue (client_connect_function_t)();
typedef ReturnValue (client_close_function_t)();
typedef ReturnValue (listen_function_t)(Message msg);
typedef ReturnValue (send_function_t)(Message msg, Message reply);

/* The program's server communication method struct */
typedef struct {
    server_connect_function_t *serverConnectFunction;
    server_close_function_t *serverCloseFunction;
    listen_function_t *listenFunction;
    send_function_t *sendFunction;
} ServerCommunicationMethod;

/* The program's client communication method struct */
typedef struct {
    client_connect_function_t *clientConnectFunction;
    client_close_function_t *clientCloseFunction;
    send_function_t *sendFunction;
} ClientCommunicationMethod;

/* It is static because it should only be initialized once */
static ServerCommunicationMethod serverCMethod;
static ClientCommunicationMethod clientCMethod;

// ------------------------------ functions -----------------------------

ServerCommunicationMethod *serverCMethodSet(CommunicationMethodCode cMethod);
// if serverCMethod is null then allocate and set it, ow return the existing one

ClientCommunicationMethod *clientCMethodSet(CommunicationMethodCode cMethod);
// if clientCMethod is null then allocate and set it, ow return the existing one