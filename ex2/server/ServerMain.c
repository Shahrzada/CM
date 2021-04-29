
#include "Server.h"
#include "config.h"

#define TOTAL_INPUT_PARAMETERS 2

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

    result = serverInitialize(getCommunicationMethodCode());
    CHECK_ERROR_PRINT_AND_RETURN_ERROR(result, "serverInitialize");
    serverListen();
}
