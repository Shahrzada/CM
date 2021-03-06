#pragma once

/********************************************************************************
 * @brief Creates and holds both communicationMethods methods for the
 *        client & the server. This header's c file should be the only file
 *        that will be edited for adding another communication method.
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

/**
 * These are the server virtual functions every communication method must
 * implement.
 */
typedef ReturnValue (server_init_connection_function_t)();
typedef ReturnValue (server_close_connection_function_t)();
typedef unsigned int (listen_function_t)(char *);
typedef ReturnValue (server_send_function_t)(const char *, unsigned int);

/**
 * These are the client virtual functions every communication method must
 * implement.
 */
typedef ReturnValue (client_init_connection_function_t)();
typedef ReturnValue (client_close_connection_function_t)();
typedef unsigned int (client_send_function_t)(const char *, unsigned int, char *);

/**
 * The server_communication_method holds pointers to the relevant
 * communication method functions.
 */
typedef struct {
    server_init_connection_function_t *serverInitConnectionFunction;
    server_close_connection_function_t *serverCloseConnectionFunction;
    listen_function_t *listenFunction;
    server_send_function_t *sendFunction;
} server_communication_method;

/**
 * The client_communication_method holds pointers to the relevant
 * communication method functions.
 */
typedef struct {
    client_init_connection_function_t *clientInitConnectionFunction;
    client_close_connection_function_t *clientCloseConnectionFunction;
    client_send_function_t *sendFunction;
} client_communication_method;

// ------------------------------ functions -----------------------------

/**
 * @brief allocates and sets the server_communication_method with the relevant
 * function pointers according the CommunicationMethodCode.
 *
 * @return the server_communication_method if the cMethod is valid, NULL ow.
 */
server_communication_method *serverCMethodSet(CommunicationMethodCode cMethod);

/**
 * @brief allocates and sets the client_communication_method with the relevant
 * function pointers according the CommunicationMethodCode.
 *
 * @return the client_communication_method if the cMethod is valid, NULL ow.
 */
client_communication_method *clientCMethodSet(CommunicationMethodCode cMethod);
