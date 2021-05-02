
#include "Client.h"
#include "config.h"

#define DISPLAY_USAGE_RETURN_ERROR do {printf("\nUsage: client <config.json path> \n"); return PROJECT_ERROR;} while(0)

ReturnValue sendSomeMsgs();
ReturnValue validateInput(int argc, char const *argv[]);

int main(int argc, char const *argv[])
{
    ReturnValue result = PROJECT_ERROR;

    result = validateInput(argc, argv);
    CHECK_ERROR_PRINT_AND_RETURN_ERROR(result, "validateInput");

    result = initConfigurations(argv[CONFIG_PATH]);
    CHECK_ERROR_PRINT_AND_RETURN_ERROR(result, "initConfigurations");

    result = clientInitialize(getCommunicationMethodCode());
    CHECK_ERROR_PRINT_AND_RETURN_ERROR(result, "clientInitialize");

    result = sendSomeMsgs();
    clientClose(result);
    return result;
}

ReturnValue validateInput(int argc, char const *argv[])
{
    if (argc != TOTAL_ARGUMENTS)
        DISPLAY_USAGE_RETURN_ERROR;

    if (argv == NULL)
        DISPLAY_USAGE_RETURN_ERROR;

    return PROJECT_SUCCESS;
}

ReturnValue sendSomeMsgs()
{
    ReturnValue result = PROJECT_ERROR;

    result = clientSendCommand(READ, 11, "Hey2222222o");
    CHECK_ERROR_RETURN_VALUE(result, PROJECT_ERROR);

    result = clientSendCommand(WRITE, 25, "BEBBBI   222         IIII");
    CHECK_ERROR_RETURN_VALUE(result, PROJECT_ERROR);

    result = clientSendCommand(READ, 26, "BEBBBI1135411222  \n  IIII");
    CHECK_ERROR_RETURN_VALUE(result, PROJECT_ERROR);

    char *filePath = getFileToTransferPath();
    CHECK_NULL_RETURN_VALUE(filePath, PROJECT_ERROR);

    unsigned int filePathLength = strnlen(filePath, MAX_JSON_VALUE_LENGTH);
    CHECK_ZERO_RETURN_VALUE(filePathLength, PROJECT_ERROR);

    result = clientSendCommand(GET_FILE, filePathLength, filePath);
    CHECK_ERROR_RETURN_VALUE(result, PROJECT_ERROR);

    result = clientSendCommand(ABORT, 7, "Goodbye");
    CHECK_ERROR_RETURN_VALUE(result, PROJECT_ERROR);

    return result;
}
