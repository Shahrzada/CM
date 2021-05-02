
#include "Server.h"
#include "config.h"

#define DISPLAY_USAGE_RETURN_ERROR do {printf("\nUsage: server <config.json path> \n"); return PROJECT_ERROR;} while(0)

ReturnValue validateInput(int argc, char const *argv[]);

int main(int argc, char const *argv[])
{
    ReturnValue result = PROJECT_ERROR;

    result = validateInput(argc, argv);
    CHECK_ERROR_PRINT_AND_RETURN_ERROR(result, "validateInput");

    result = initConfigurations(argv[CONFIG_PATH]);
    CHECK_ERROR_PRINT_AND_RETURN_ERROR(result, "initConfigurations");

    result = serverInitialize(getCommunicationMethodCode());
    CHECK_ERROR_PRINT_AND_RETURN_ERROR(result, "serverInitialize");

    serverListen(); // No return
}

ReturnValue validateInput(int argc, char const *argv[])
{
    if (argc != TOTAL_ARGUMENTS)
        DISPLAY_USAGE_RETURN_ERROR;

    if (argv == NULL)
        DISPLAY_USAGE_RETURN_ERROR;

    return PROJECT_SUCCESS;
}
