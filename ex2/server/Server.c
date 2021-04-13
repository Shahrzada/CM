#include <unistd.h>
#include "Server.h"
#include "../communicationProtocol/include/MessageProtocol.h"

// -------------------------- const definitions -------------------------


// ------------------------------ private functions -----------------------------

static void serverRead()
{
    // send client all of a file's contents.
}

static void serverWrite(char *msg)
{
    CHECK_NULL_RETURN(msg);
    // write messageGetContents(msg) to file and answer with success/failure.
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
    // todo: until client sends abort?
    while (true)
    {
        char *incomingMsg = MPServerReceive();
        CHECK_NULL_RETURN(incomingMsg);
        serverHandleMessage(incomingMsg);
        free(incomingMsg);
        sleep(1);
    }
}

ReturnValue serverDisconnect()
{
    return MPServerCloseConnection();
}