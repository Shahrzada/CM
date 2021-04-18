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
    if (!messageValidateFormat(reply))
        return ERROR;

    // TODO: there's nothing really I check here atm, maybe will check for success in the future.
    const char *pointerToMsg = messageGetContents(reply);
    printf("Client received: %s.\n", pointerToMsg);

    return SUCCESS;
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