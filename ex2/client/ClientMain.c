
#include <unistd.h>
#include "Client.h"

int main(int argc, char const *argv[])
{
    ReturnValue result = clientInitialize(FILE_METHOD);
    CHECK_ERROR_PRINT_AND_RETURN_ERROR(result, "clientInitialize");

    char *msg = messageSet(CLIENT, READ, "Heyo");
    CHECK_NULL_PRINT_AND_RETURN_ERROR(msg, "messageSet");

    result = clientSendCommand(msg);
    free(msg);

    clientClose();
    return result;
}