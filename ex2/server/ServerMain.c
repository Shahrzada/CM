
#include <Message.h>

#include "Server.h"


int main(int argc, char const *argv[])
{
    ReturnValue result = PROJECT_ERROR;


    Message *msg = messageSetT(SERVER, READ, 15, "ABC\nfffffagagva");

    unsigned int msgStrLength = 0;
    char *msgStr = messageToCString(msg, &msgStrLength);

    Message *msgNew = messageFromCString(msgStr, msgStrLength);

    free(msgStr);
    messageFree(msg);
    messageFree(msgNew);


    result = serverInitialize(SOCKET_METHOD);
    CHECK_ERROR_PRINT_AND_RETURN_ERROR(result, "serverInitialize");
    serverListen();
}
