
// ------------------------------ includes ------------------------------

#include <unistd.h>

#include "Server.h"
#include "../communicationProtocol/include/MessageProtocol.h"

// ------------------------------ private functions -----------------------------

static void serverRead()
{
    // Assuming we did some reading from somewhere
    MPServerSendSuccessOrFailure(SUCCESS);
}

static void serverWrite(const char *msg)
{
    CHECK_NULL_RETURN(msg);

    // Assuming server wrote content to somewhere, and succeeded.
    MPServerSendSuccessOrFailure(SUCCESS);
}

static void serverAbort(char *msg)
{
    CHECK_NULL_RETURN(msg);
    free(msg);
    MPServerSendSuccessOrFailure(SUCCESS);
    sleep(1);
    serverClose();
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
            printf("Server got: %s\n", incomingMsg);
        }
        serverHandleMessage(incomingMsg);
        free(incomingMsg);
        sleep(1);
    }
}

void serverClose()
{
    MPServerCloseConnection();
    printf("Server got ABORT command, closing...");
    exit(SUCCESS);
}