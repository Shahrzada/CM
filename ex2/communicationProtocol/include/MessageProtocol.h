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
 * method code, allocating/using the static ServerCommunicationMethod object that holds
 * all relevant function pointers
 *
 * @return PROJECT_SUCCESS if connection was established, PROJECT_ERROR ow.
 */
ReturnValue MPServerInitConnection(CommunicationMethodCode cMethodCode);

/**
 * @brief closes the server connection using the static ServerCommunicationMethod,
 * and frees it.
 *
 * @return PROJECT_SUCCESS if connection was closed, PROJECT_ERROR ow.
 */
ReturnValue MPServerCloseConnection();

/**
 * @brief initializes the client connection according to the communication
 * method code, allocating/using the static ClientCommunicationMethod object that holds
 * all relevant function pointers
 *
 * @return PROJECT_SUCCESS if connection was established, PROJECT_ERROR ow.
 */
ReturnValue MPClientInitConnection(CommunicationMethodCode cMethodCode);

/**
 * @brief closes the client connection using the static ClientCommunicationMethod,
 * and frees it.
 *
 * @return PROJECT_SUCCESS if connection was closed, PROJECT_ERROR ow.
 */
ReturnValue MPClientCloseConnection();

/**
 * @brief listens to new msgs using the static ServerCommunicationMethod object
 *
 * @return a new msg if valid, NULL ow.
 */
char *MPServerListen();

/**
 * @brief send the msg using the static ServerCommunicationMethod object
 *
 * @return PROJECT_SUCCESS if succeeded, PROJECT_ERROR ow.
 */
ReturnValue MPServerSend(char *msg);

/**
 * @brief send the formatted PROJECT_SUCCESS or FAILURE server msg according
 * to the input result value.
 */
void MPServerSendSuccessOrFailure(ReturnValue result);

/**
 * @brief send the msg using the static ClientCommunicationMethod object
 *
 * @return the reply msg from the server if succeeded, NULL ow.
 */
char *MPClientSend(const char *msg);

char *MPClientReceive();
