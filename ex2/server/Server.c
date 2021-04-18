#include <unistd.h>
#include "Server.h"
#include "../communicationProtocol/include/MessageProtocol.h"

// -------------------------- const definitions -------------------------


// ------------------------------ private functions -----------------------------

static void serverRead()
{
    // Assuming we did some reading from somewhere

    ReturnValue result = SUCCESS;
    MPServerSendSuccessOrFailure(result);
}

static void serverWrite(char *msg)
{
    CHECK_NULL_RETURN(msg);

    // Assuming server wrote content to somewhere, and succeeded nice.

    ReturnValue result = SUCCESS;
    MPServerSendSuccessOrFailure(result);
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
        serverDisconnect();

    else
    {
        // todo: report error
        return;
    }
}

// ------------------------------ functions -----------------------------

ReturnValue serverInitialize(CommunicationMethodCode cMethod)
{
    return MPServerInitConnection(cMethod);
}

void serverListen()
{
    // todo: until client sends abort? yes!
    while (true)
    {
        char *incomingMsg = MPServerListen();
        if (!messageValidateFormat(incomingMsg)) {
            PRINT_ERROR_MSG_AND_FUNCTION_NAME("serverListen", "Bad msg format");
            return;
        }
        serverHandleMessage(incomingMsg);
        free(incomingMsg);
        sleep(1);
    }
}

ReturnValue serverDisconnect()
{
    return MPServerCloseConnection();
}