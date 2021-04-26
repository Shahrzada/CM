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
// CR: You should just feed the include directory for the compiler instead of giving a relative path like this
#include "../../Macros.h"

// -------------------------- const definitions -------------------------

/**
 * These are the server virtual functions every communication method must
 * implement.
 */
typedef ReturnValue (server_init_connection_function_t)();
typedef ReturnValue (server_close_connection_function_t)();
typedef char * (listen_function_t)();
typedef ReturnValue (server_send_function_t)(const char *);

/**
 * These are the client virtual functions every communication method must
 * implement.
 */
typedef ReturnValue (client_init_connection_function_t)();
typedef ReturnValue (client_close_connection_function_t)();
typedef char * (client_send_function_t)(const char *);
typedef char * (client_receive_function_t)();

/**
 * The ServerCommunicationMethod holds pointers to the relevant
 * communication method functions.
 */
typedef struct {
    server_init_connection_function_t *serverInitConnectionFunction;
    server_close_connection_function_t *serverCloseConnectionFunction;
    listen_function_t *listenFunction;
    server_send_function_t *sendFunction;
} ServerCommunicationMethod;
// CR: Try to stick to the naming convetion. typedef should be name snake_case and not CamelCase

/**
 * The ClientCommunicationMethod holds pointers to the relevant
 * communication method functions.
 */
typedef struct {
    client_init_connection_function_t *clientInitConnectionFunction;
    client_close_connection_function_t *clientCloseConnectionFunction;
    client_send_function_t *sendFunction;
    client_receive_function_t *clientReceiveFunction;
} ClientCommunicationMethod;
// CR: same as the above
// ------------------------------ functions -----------------------------

/**
 * @brief allocates and sets the ServerCommunicationMethod with the relevant
 * function pointers according the CommunicationMethodCode.
 *
 * @return the ServerCommunicationMethod if the cMethod is valid, NULL ow.
 */
ServerCommunicationMethod *serverCMethodSet(CommunicationMethodCode cMethod);

/**
 * @brief allocates and sets the ClientCommunicationMethod with the relevant
 * function pointers according the CommunicationMethodCode.
 *
 * @return the ClientCommunicationMethod if the cMethod is valid, NULL ow.
 */
ClientCommunicationMethod *clientCMethodSet(CommunicationMethodCode cMethod);
