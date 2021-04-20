
// ------------------------------ includes ------------------------------
#include <unistd.h>
#include <sys/stat.h>

#include "../include/FileMethod.h"

// -------------------------- const definitions -------------------------

#define COMMUNICATION_FILE_NAME "bebi.txt"
#define INIT_FILE_SUCCESS_MSG "[Server]: Created the file successfully.\n"
#define CLOSED_FILE_SUCCESS_MSG "[Server]: Closed the file successfully.\n"
#define MAX_LINE_LENGTH 257

// -------------------------- macros -------------------------

#define CHECK_FILE_NULL_PRINT_OPEN_ERROR_GOTO_CLEANUP(pFile) do { \
           if ((pFile) == NULL) {                                 \
                printf("Failed to open the file.\n");             \
                goto cleanup; }                                   \
           } while(0)

#define CHECK_NEGATIVE_PRINT_WRITE_ERROR_GOTO_CLEANUP(fputsResult) do { \
           if ((fputsResult) < 0) {                              \
                printf("Couldn't write to file.\n");                       \
                goto cleanup; }                                            \
           } while(0)

// ------------------------------ private functions -----------------------------

static char *fileGetMessage(FILE *pFile)
{
    CHECK_NULL_RETURN_NULL(pFile);

    // get the last line in the file
    char buf[MAX_LINE_LENGTH];
    while(!feof(pFile))
        fgets(buf, MAX_LINE_LENGTH, pFile);

    // ignore the '\n'
    buf[strlen(buf) - EOL_CHAR_SIZE] = NULL_CHAR;

    // allocate memory for the msg
    unsigned int msgLength = strlen(buf) + NULL_CHAR_SIZE;
    char *msg = (char *) malloc(sizeof(char) * msgLength);
    CHECK_NULL_RETURN_NULL(msg);

    // and copy the last line into it
    strncpy(msg, buf, msgLength - NULL_CHAR_SIZE);
    msg[msgLength - 1] = NULL_CHAR;

    return msg;
}

static char *fileReceive()
{
    // open file for reading
    FILE * pFile = fopen(COMMUNICATION_FILE_NAME, FILE_READ_MODE);
    CHECK_FILE_NULL_PRINT_OPEN_ERROR_GOTO_CLEANUP(pFile);

    // get msg
    char *msgReceived = fileGetMessage(pFile);
    fclose(pFile);
    return msgReceived;

cleanup:
    fclose(pFile);
    return NULL;
}
// ------------------------------ functions -----------------------------

ReturnValue fileServerInitConnection() {
    // create and open the file for writing
    FILE *pFile = fopen(COMMUNICATION_FILE_NAME,FILE_WRITE_UPDATE_MODE);
    CHECK_FILE_NULL_PRINT_OPEN_ERROR_GOTO_CLEANUP(pFile);

    // write first success msg
    int result = fputs(INIT_FILE_SUCCESS_MSG, pFile);
    CHECK_NEGATIVE_PRINT_WRITE_ERROR_GOTO_CLEANUP(result);
    fclose(pFile);

    return PROJECT_SUCCESS;

cleanup:
    fclose(pFile);
    return PROJECT_ERROR;
}

ReturnValue fileServerCloseConnection() {
    // open the file for appending
    FILE * pFile;
    pFile = fopen(COMMUNICATION_FILE_NAME,FILE_APPEND_MODE);
    CHECK_FILE_NULL_PRINT_OPEN_ERROR_GOTO_CLEANUP(pFile);

    // write last closing success msg
    int result = fputs(CLOSED_FILE_SUCCESS_MSG, pFile);
    CHECK_NEGATIVE_PRINT_WRITE_ERROR_GOTO_CLEANUP(result);

    fclose(pFile);
    return PROJECT_SUCCESS;

cleanup:
    fclose(pFile);
    return PROJECT_ERROR;
}

char *fileListen() {
    // stat helps us checking for new msgs without opening it (using file size)
    struct stat st;
    stat(COMMUNICATION_FILE_NAME, &st);
    int fileSize = st.st_size;

    while (true)
    {
        // get file size
        stat(COMMUNICATION_FILE_NAME, &st);

        // larger size means new msgs
        if (fileSize < st.st_size)
            return fileReceive();

        // smaller size means the file was corrupted
        else if (st.st_size < fileSize)
            return NULL;

        sleep(1);
    }
}

ReturnValue fileSend(const char *msg) {
    CHECK_NULL_RETURN_ERROR(msg);

    // open file for appending
    FILE *pFile = fopen(COMMUNICATION_FILE_NAME,FILE_APPEND_MODE);
    CHECK_FILE_NULL_PRINT_OPEN_ERROR_GOTO_CLEANUP(pFile);

    // write given msg
    int writingResult = fputs(msg, pFile);
    CHECK_NEGATIVE_PRINT_WRITE_ERROR_GOTO_CLEANUP(writingResult);

    // add EOL to differentiate msgs
    writingResult = fputs(EOL_STRING, pFile);
    CHECK_NEGATIVE_PRINT_WRITE_ERROR_GOTO_CLEANUP(writingResult);

    fclose(pFile);
    return PROJECT_SUCCESS;

cleanup:
    fclose(pFile);
    return PROJECT_ERROR;
}

ReturnValue fileClientInitConnection() {
    // open the file for appending, just to check the file is okay really
    FILE * pFile;
    pFile = fopen(COMMUNICATION_FILE_NAME,FILE_APPEND_MODE);
    CHECK_FILE_NULL_PRINT_OPEN_ERROR_GOTO_CLEANUP(pFile);

    fclose(pFile);
    return PROJECT_SUCCESS;

cleanup:
    fclose(pFile);
    return PROJECT_ERROR;
}

ReturnValue fileClientCloseConnection() {
    return PROJECT_SUCCESS;
}

char *fileClientSend(const char *msg) {
    CHECK_NULL_RETURN_NULL(msg);

    // write the msg to the file
    ReturnValue result = fileSend(msg);
    CHECK_ERROR_RETURN_NULL(result);

    // wait for server reply
    char *reply = fileListen();
    sleep(1);
    return reply;
}
