
#include <unistd.h>
#include "Client.h"

ReturnValue sendAMsg(Command commandType, char *contents);
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
    ReturnValue result = sendAMsg(READ, "Heyo");
    CHECK_ERROR_RETURN_ERROR(result);

    result = sendAMsg(WRITE, "BEBBBI            IIII");
    CHECK_ERROR_RETURN_ERROR(result);

    result = sendAMsg(WRITE, "BEBBB   IIIII");
    CHECK_ERROR_RETURN_ERROR(result);

    result = sendAMsg(READ, "BEBBBI1135411    IIII");
    CHECK_ERROR_RETURN_ERROR(result);

    result = sendAMsg(WRITE, "BEB1111111111111111111111111111111111111111111111111BBIIIII");
    CHECK_ERROR_RETURN_ERROR(result);

    result = sendAMsg(ABORT, "Goodbye");
    CHECK_ERROR_RETURN_ERROR(result);

    return result;
}

ReturnValue sendAMsg(Command commandType, char *contents)
{
    CHECK_NULL_PRINT_AND_RETURN_ERROR(contents, "messageSet");
    printf("Client sent: [%d]-%s.\n", commandType, contents);
    ReturnValue result = clientSendCommand(commandType, contents);
    return result;
}
