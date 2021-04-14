#include "Client.h"

#include "../communicationProtocol/include/MessageProtocol.h"


ReturnValue clientInitialize(CommunicationMethodCode cMethod)
{
    return MPClientInitConnection(cMethod);
}

ReturnValue clientClose()
{
    return MPClientCloseConnection();
}

static ReturnValue handleReply(char *reply)
{
    if (reply == NULL || !messageValidateFormat(reply))
        return ERROR;

    // there's nothing really I should check here atm, maybe will check
    // for success in the future.
    const char *pointerToMsg = messageGetContents(reply);
    printf("Client received: %s.\n", pointerToMsg);

    return SUCCESS;
}

ReturnValue clientSendCommand(const char *msg)
{
    if (msg == NULL || !messageValidateFormat(msg))
        return ERROR;

    // send the message and wait & get its reply
    char *reply = MPClientSend(msg);
    ReturnValue result = handleReply(reply);
    free(reply);
    return result;
}