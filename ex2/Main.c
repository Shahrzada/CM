
//#include "server/server.h"
//#include "client/client.h"

#include "communication/FileMethod.h"

#define CHECK_ERROR_PRINT_AND_RETURN_ERROR(result, function) do { \
           if ((result) == ERROR) {                                 \
                printf("Error with %s.\n", (function));             \
                 return ERROR; }                                   \
           } while(0)

#define CHECK_NULL_PRINT_AND_RETURN_ERROR(result, function) do { \
           if ((result) == NULL) {                                 \
                printf("Error with %s.\n", (function));             \
                 return ERROR; }                                   \
           } while(0)

int main(int argc, char const *argv[])
{
    FileData *fileData = fileServerInitConnect();
    CHECK_NULL_PRINT_AND_RETURN_ERROR(fileData, "fileServerInitConnect");

    ReturnValue result = fileClientInitConnect();
    CHECK_ERROR_PRINT_AND_RETURN_ERROR(result, "fileClientInitConnect");

    Message * msg = messageAllocate();
    CHECK_NULL_RETURN_ERROR(msg);

    fileListen(fileData, msg);
    messageFree(msg);

    result = fileServerCloseConnection(fileData);
    CHECK_ERROR_PRINT_AND_RETURN_ERROR(result, "fileServerCloseConnection");


    /* Ask user for desired communication method (present available, pick by number)
     * e.g. "For sockets, enter 0.\nFor file, enter 1.\n" */

    /* init the server via another thread */

    /* init the client */

    /* While alive: Ask user for client commands (present available, pick by number)
    * e.g. "For READ, enter 0.\nFor WRITE, enter 1.\n"
    * e.g. if picked write, prompt for length of msg then the msg itself
    * Print the result of the sending the command (= the server's reply) */

    return 0;
}