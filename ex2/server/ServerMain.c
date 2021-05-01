
#include "Server.h"
#include "config.h"

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
    ReturnValue result = PROJECT_ERROR;

    result = validateInput(argc, argv);
    CHECK_ERROR_PRINT_AND_RETURN_ERROR(result, "validateInput");

    result = initConfigurations(argv[1]);
    CHECK_ERROR_PRINT_AND_RETURN_ERROR(result, "initConfigurations");

    result = serverInitialize(getCommunicationMethodCode());
    CHECK_ERROR_PRINT_AND_RETURN_ERROR(result, "serverInitialize");

    serverListen(); // No return
}
