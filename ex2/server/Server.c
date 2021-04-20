
// ------------------------------ includes ------------------------------

#include <unistd.h>

#include "Server.h"
#include "../communicationProtocol/include/MessageProtocol.h"

// ------------------------------ private functions -----------------------------

static void serverClose(bool errorExitFlag)
{
    MPServerCloseConnection();

    if (errorExitFlag)
        printf("An error occurred, closing...");
    else
        printf("Server got ABORT command, closing...");

    exit(PROJECT_SUCCESS);
}

static void serverRead()
{
    // Assuming we did some reading from somewhere
    MPServerSendSuccessOrFailure(PROJECT_SUCCESS);
}

static void serverWrite(const char *msg)
{
    CHECK_NULL_RETURN(msg);

    // Assuming server wrote content to somewhere, and succeeded.
    MPServerSendSuccessOrFailure(PROJECT_SUCCESS);
}

static void serverAbort(char *msg)
{
    int errorExitFlag = true;
    if (msg != NULL)
    {
        free(msg);
        errorExitFlag = false;
    }
    MPServerSendSuccessOrFailure(PROJECT_SUCCESS);
    sleep(1);
    serverClose(errorExitFlag);
}

static void serverHandleMessage(char *msg)
{
    CHECK_NULL_RETURN(msg);
    Command currentCommand = messageGetCommand(msg);

    if (currentCommand == READ)
        serverRead();

    else if (currentCommand == WRITE)
        serverWrite(msg);

    else if (currentCommand == ABORT)
        serverAbort(msg);

    else
        PRINT_ERROR_MSG_AND_FUNCTION_NAME("serverHandleMessage", "Bad COMMAND");
}

// ------------------------------ functions -----------------------------

ReturnValue serverInitialize(CommunicationMethodCode cMethod)
{
    return MPServerInitConnection(cMethod);
}

_Noreturn void serverListen()
{
    while (true)
    {
        char *incomingMsg = MPServerListen();
        if (!messageValidateFormat(incomingMsg)) {
            PRINT_ERROR_MSG_AND_FUNCTION_NAME("serverListen", "Bad msg format");

            if (incomingMsg == NULL)
                serverAbort(incomingMsg);

            printf("Server got: %s\n", incomingMsg);
        }

        serverHandleMessage(incomingMsg);
        free(incomingMsg);
        sleep(1);
    }
}
