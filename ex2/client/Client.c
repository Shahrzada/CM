
// ------------------------------ includes ------------------------------

#include "Client.h"
#include "../communicationProtocol/include/MessageProtocol.h"

// ------------------------------ private functions -----------------------------

static ReturnValue clientGetFile(char *reply)
{
    // TODO create a file instead
    printf("Client is receiving a file:\n");

    char *followingReply = reply;
    Command currentCommand = GET_FILE;
    int counter = 0;
    while (currentCommand == GET_FILE)
    {
        counter++;
        // TODO write to a file instead
        printf("[%d]: %s\n", counter, followingReply);
        free(followingReply);
        followingReply = MPClientReceive();
        //todo maybe check validity?
        currentCommand = messageGetCommand(followingReply);
    }

    printf("Client finished receiving a file of %d msgs with msg:\n", counter);
    printf("%s\n", followingReply);
    return PROJECT_SUCCESS;
}


static ReturnValue handleReply(char *reply)
{
    if (!messageValidateFormat(reply))
        return PROJECT_ERROR;

    Command currentCommand = messageGetCommand(reply);

    // SHAH: there's nothing much I check atm
    if (currentCommand == GET_FILE)
        clientGetFile(reply);
    else
    {
        const char *pointerToMsg = messageGetContents(reply);
        printf("Client received reply: %s.\n", pointerToMsg);
    }
    return PROJECT_SUCCESS;
}

// ------------------------------ functions -----------------------------

ReturnValue clientInitialize(CommunicationMethodCode cMethod)
{
    return MPClientInitConnection(cMethod);
}

ReturnValue clientClose()
{
    return MPClientCloseConnection();
}

ReturnValue clientSendCommand(Command commandType, char *contents)
{
    CHECK_NULL_RETURN_ERROR(contents);
    char *msg = messageSet(CLIENT, commandType, contents);
    if (!messageValidateFormat(msg))
        return PROJECT_ERROR;

    // send the message and wait to handle its reply
    char *reply = MPClientSend(msg);
    ReturnValue result = handleReply(reply);

    free(msg);
    free(reply);
    return result;
}