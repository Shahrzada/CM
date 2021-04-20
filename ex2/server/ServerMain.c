
#include "Server.h"

int main(int argc, char const *argv[])
{
    ReturnValue result = serverInitialize(FILE_METHOD);
    CHECK_ERROR_PRINT_AND_RETURN_ERROR(result, "serverInitialize");
    serverListen();
}