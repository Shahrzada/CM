
// ------------------------------ includes ------------------------------

#include "Client.h"
#include "../communicationProtocol/include/MessageProtocol.h"

// ------------------------------ private functions -----------------------------

static ReturnValue handleReply(char *reply)
{
    if (!messageValidateFormat(reply))
        return ERROR;

    // SHAH: there's nothing I check atm
    const char *pointerToMsg = messageGetContents(reply);
    printf("Client received reply: %s.\n", pointerToMsg);

    return SUCCESS;
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

ReturnValue clientSendCommand(const char *msg)
{
    if (!messageValidateFormat(msg))
        return ERROR;

    // send the message and wait to handle its reply
    char *reply = MPClientSend(msg);
    ReturnValue result = handleReply(reply);

    free(reply);
    return result;
}