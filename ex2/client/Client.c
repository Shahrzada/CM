
// ------------------------------ includes ------------------------------

#include "Client.h"
#include "../communicationProtocol/include/MessageProtocol.h"

// ------------------------------ private functions -----------------------------

static ReturnValue handleReply(char *reply)
{
    if (!messageValidateFormat(reply))
        return PROJECT_ERROR;

    // SHAH: there's nothing I check atm
    const char *pointerToMsg = messageGetContents(reply);
    printf("Client received reply: %s.\n", pointerToMsg);

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