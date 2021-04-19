
#include <unistd.h>
#include "Client.h"

ReturnValue sendSomeMsgs();

int main(int argc, char const *argv[])
{
    ReturnValue result = clientInitialize(FILE_METHOD);
    CHECK_ERROR_PRINT_AND_RETURN_ERROR(result, "clientInitialize");

    result = sendSomeMsgs();

    clientClose();
    return result;
}


ReturnValue sendSomeMsgs()
{
    char *msg = messageSet(CLIENT, READ, "Heyo");
    CHECK_NULL_PRINT_AND_RETURN_ERROR(msg, "messageSet");
    ReturnValue result = clientSendCommand(msg);
    free(msg);
    CHECK_ERROR_RETURN_ERROR(result);
    sleep(1);

    msg = messageSet(CLIENT, WRITE, "Heyo#2");
    CHECK_NULL_PRINT_AND_RETURN_ERROR(msg, "messageSet");
    result = clientSendCommand(msg);
    free(msg);
    CHECK_ERROR_RETURN_ERROR(result);
    sleep(1);

    msg = messageSet(CLIENT, ABORT, "Goodbye");
    CHECK_NULL_PRINT_AND_RETURN_ERROR(msg, "messageSet");
    result = clientSendCommand(msg);
    free(msg);

    return result;
}