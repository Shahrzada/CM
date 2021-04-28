
#include "Client.h"

ReturnValue sendSomeMsgs();

int main(int argc, char const *argv[])
{
    ReturnValue result = clientInitialize(FILE_METHOD);
    CHECK_ERROR_PRINT_AND_RETURN_ERROR(result, "clientInitialize");

    result = sendSomeMsgs();
    clientClose(result);

    return result;
}

ReturnValue sendSomeMsgs()
{
    ReturnValue result = PROJECT_ERROR;

    result = clientSendCommand(READ, 11, "Hey2222222o");
    CHECK_ERROR_RETURN_ERROR(result);

    result = clientSendCommand(WRITE, 25, "BEBBBI   222         IIII");
    CHECK_ERROR_RETURN_ERROR(result);

    result = clientSendCommand(READ, 26, "BEBBBI1135411222  \n  IIII");
    CHECK_ERROR_RETURN_ERROR(result);

//    result = clientSendCommand(GET_FILE, 20, "schnitzel/pitzel.exe");
//    CHECK_ERROR_RETURN_ERROR(result);

    result = clientSendCommand(ABORT, 7, "Goodbye");
    CHECK_ERROR_RETURN_ERROR(result);

    return result;
}
