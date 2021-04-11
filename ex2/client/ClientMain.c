
#include "Client.h"

int main(int argc, char const *argv[])
{
    ReturnValue result = clientInitialize(FILE_METHOD);
    CHECK_ERROR_PRINT_AND_RETURN_ERROR(result, "clientInitialize");


    Message *msg = messageAllocate();
    CHECK_NULL_PRINT_AND_RETURN_ERROR(msg, "messageAllocate");

    clientSendCommand(msg);
    free(msg);

    result = clientCloseConnect();
    CHECK_ERROR_PRINT_AND_RETURN_ERROR(result, "clientInitialize");

    return SUCCESS;
}