
#include <unistd.h>
#include "Client.h"

ReturnValue sendAMsg(Command commandType, char *contents);
ReturnValue sendSomeMsgs();

int main(int argc, char const *argv[])
{
    ReturnValue result = clientInitialize(SOCKET_METHOD);
    CHECK_ERROR_PRINT_AND_RETURN_ERROR(result, "clientInitialize");

    result = sendSomeMsgs();

    clientClose(result);
    return result;
}

ReturnValue sendSomeMsgs()
{
    ReturnValue result = PROJECT_ERROR;

    result = sendAMsg(READ, "Hey2222222o");
    CHECK_ERROR_RETURN_ERROR(result);

    result = sendAMsg(WRITE, "BEBBBI   222         IIII");
    CHECK_ERROR_RETURN_ERROR(result);

    result = sendAMsg(WRITE, "BEBBB   IIIII");
    CHECK_ERROR_RETURN_ERROR(result);

    result = sendAMsg(READ, "BEBBBI1135411222  \n  IIII");
    CHECK_ERROR_RETURN_ERROR(result);

    result = sendAMsg(GET_FILE, "schnitzel/pitzel.exe");
    CHECK_ERROR_RETURN_ERROR(result);

    result = sendAMsg(ABORT, "Goodbye");
    CHECK_ERROR_RETURN_ERROR(result);

    return result;
}

ReturnValue sendAMsg(Command commandType, char *contents)
{
    CHECK_NULL_PRINT_AND_RETURN_ERROR(contents, "messageSet");
    printf("Client sent: CLIENT,%d,%s.\n", commandType, contents);
    ReturnValue result = clientSendCommand(commandType, contents);
    return result;
}
