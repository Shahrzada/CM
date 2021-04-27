#pragma once

/********************************************************************************
 * @brief The message protocol layer.
 *
 *
 * @section DESCRIPTION
 *
 * Error handling
 * ~~~~~~~~~~~~~~
 * Most functions may fail due to failure to allocate dynamic memory. When
 * this happens the functions will return an appropriate failure value. If this
 * happens, then the state of the other outputs of the function is undefined.
 ********************************************************************************/

// ------------------------------ includes ------------------------------

#include "Message.h"

// ------------------------------ functions -----------------------------
/**
 * @brief initializes the server connection according to the communication
 * method code, allocating/using the static server_communication_method object that holds
 * all relevant function pointers
 *
 * @return PROJECT_SUCCESS if connection was established, PROJECT_ERROR ow.
 */
ReturnValue MPServerInitConnection(CommunicationMethodCode cMethodCode);

/**
 * @brief closes the server connection using the static server_communication_method,
 * and frees it.
 *
 * @return PROJECT_SUCCESS if connection was closed, PROJECT_ERROR ow.
 */
ReturnValue MPServerCloseConnection();

/**
 * @brief listens to new msgs using the static server_communication_method object
 *
 * @return a new msg if valid, NULL ow.
 */
Message *MPServerListen();

/**
 * @brief send the msg using the static server_communication_method object
 *
 * @return PROJECT_SUCCESS if succeeded, PROJECT_ERROR ow.
 */
ReturnValue MPServerSend(Message *msg);

/**
 * @brief send the formatted PROJECT_SUCCESS or FAILURE server msg according
 * to the input result value.
 */
void MPServerSendSuccessOrFailure(ReturnValue result);

/**
 * @brief initializes the client connection according to the communication
 * method code, allocating/using the static client_communication_method object that holds
 * all relevant function pointers
 *
 * @return PROJECT_SUCCESS if connection was established, PROJECT_ERROR ow.
 */
ReturnValue MPClientInitConnection(CommunicationMethodCode cMethodCode);

/**
 * @brief closes the client connection using the static client_communication_method,
 * and frees it.
 *
 * @return PROJECT_SUCCESS if connection was closed, PROJECT_ERROR ow.
 */
ReturnValue MPClientCloseConnection(ReturnValue result);

/**
 * @brief send the msg using the static client_communication_method object
 *
 * @return the reply msg from the server if succeeded, NULL ow.
 */
Message *MPClientSend(Message *msg);

/**
 * @brief get the earliest unread msg from the server
 *
 * @return the msg from the server if succeeded, NULL ow.
 */
Message *MPClientReceive();
