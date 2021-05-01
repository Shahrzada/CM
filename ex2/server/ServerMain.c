
#include "Server.h"
#include "config.h"

ReturnValue validateInput(int argc, char const *argv[])
{ 
    // CR: what happend to the enum?
    if (argc != TOTAL_INPUT_PARAMETERS)
        return PROJECT_ERROR;

    if (argv == NULL)
        return PROJECT_ERROR;

    if (argv[1] == NULL) // CR: Why would it be NULL if argc == TOTAL_INPUT_PARAMETERS?
        return PROJECT_ERROR;

    return PROJECT_SUCCESS;
}

int main(int argc, char const *argv[])
{
    ReturnValue result = PROJECT_ERROR;

    result = validateInput(argc, argv);
    CHECK_ERROR_PRINT_AND_RETURN_ERROR(result, "validateInput");

    result = initConfigurations(argv[1]); // CR: so argv[1] is...? the 1 is for what?
    CHECK_ERROR_PRINT_AND_RETURN_ERROR(result, "initConfigurations");

    result = serverInitialize(getCommunicationMethodCode());
    CHECK_ERROR_PRINT_AND_RETURN_ERROR(result, "serverInitialize");

    serverListen(); // No return
}
