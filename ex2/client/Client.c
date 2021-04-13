#include "Client.h"

#include "../communicationProtocol/include/MessageProtocol.h"

ReturnValue clientInitialize(CommunicationMethodCode cMethod)
{
    return MPClientInitConnection(cMethod);
}

ReturnValue clientSendCommand(char *msg)
{
    CHECK_NULL_RETURN_ERROR(msg);

    char *reply = MPSend(msg);
    CHECK_NULL_RETURN_ERROR(reply);

    //todo: do something with reply

    free(reply);
    return SUCCESS;
}