
#include "Client.h"
#include "config.h"

#define TOTAL_INPUT_PARAMETERS 2

ReturnValue sendSomeMsgs();

ReturnValue validateInput(int argc, char const *argv[])
{
    if (argc != TOTAL_INPUT_PARAMETERS)
        return PROJECT_ERROR;

    if (argv == NULL)
        return PROJECT_ERROR;

    if (argv[1] == NULL)
        return PROJECT_ERROR;

    return PROJECT_SUCCESS;
}

int main(int argc, char const *argv[])
{
    if (validateInput(argc, argv) == PROJECT_ERROR)
        return PROJECT_ERROR;

    ReturnValue result = initConfigurations(argv[1]);
    CHECK_ERROR_RETURN_ERROR(result);

    result = clientInitialize(getCommunicationMethodCode());
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

    // send a file
    char *filePath = getFileToTransferPath();
    CHECK_NULL_RETURN_ERROR(filePath);

    unsigned int filePathLength = strnlen(filePath, MAX_JSON_VALUE_LENGTH);
    CHECK_ZERO_RETURN_ERROR(filePathLength);

    result = clientSendCommand(GET_FILE, filePathLength, filePath);
    CHECK_ERROR_RETURN_ERROR(result);

    // send abort
    result = clientSendCommand(ABORT, 7, "Goodbye");
    CHECK_ERROR_RETURN_ERROR(result);

    return result;
}
