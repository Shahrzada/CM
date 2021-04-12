#include <unistd.h>
#include "Server.h"
#include "../communicationProtocol/include/MessageProtocol.h"

// -------------------------- const definitions -------------------------


// ------------------------------ private functions -----------------------------

static void serverRead()
{
    // send client all of a file's contents.
}

static void serverWrite(Message * msg)
{
    CHECK_NULL_RETURN(msg);
    // write messageGetContents(msg) to file and answer with success/failure.
}

static void serverHandleMessage(Message * msg)
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
    return msgServerInitConnect(cMethod);
}

void serverListen()
{
    ReturnValue result = ERROR;
    // todo: until client sends abort?
    while (true)
    {
        Message * msg = messageAllocate();
        CHECK_NULL_RETURN(msg); // todo: print error
        result = msgServerReceive(msg);
        CHECK_ERROR_RETURN(result);
        serverHandleMessage(msg);
        messageFree(msg);
        sleep(1);
    }
}

ReturnValue serverDisconnect()
{
    return msgServerCloseConnection();
}