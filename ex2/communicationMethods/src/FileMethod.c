
// ------------------------------ includes ------------------------------
#include <unistd.h>
#include <sys/stat.h>

#include "../include/FileMethod.h"

// -------------------------- const definitions -------------------------

#define COMMUNICATION_FILE_NAME "bebi.txt"

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

static time_t fileLatestModified = 0;
unsigned int fileLatestCharPosition = 0;

// ------------------------------ private functions -----------------------------

static ReturnValue updateLatestModified()
{
    // stat helps us checking for new msgs without opening it (using file info)
    struct stat fileStat;
    int result = stat(COMMUNICATION_FILE_NAME, &fileStat);
    if (result != 0)
    {
        PRINT_ERROR_MSG_AND_FUNCTION_NAME("fileListen", "Couldn't check file stat");
        return PROJECT_ERROR;
    }
    fileLatestModified = fileStat.st_mtime;
    return PROJECT_SUCCESS;
}

static FILE *getToLastKnownPosition(FILE *pFile) {
    CHECK_NULL_RETURN_NULL(pFile);

    if (fileLatestCharPosition == 0)
        return pFile;

    unsigned int counter = 0;
    while (fgetc(pFile) != EOF)
    {
        if (counter == fileLatestCharPosition - 1)
            return pFile;
        counter++;
    }
    return NULL;
}

static unsigned int copyMsgIntoCStringAndReturnItsLength(FILE *pFile, char *buf) {
    CHECK_NULL_RETURN_ZERO(pFile);
    CHECK_NULL_RETURN_ZERO(buf);

    unsigned int msgLength = 0;
    int ch = 0;

    // TODO handle in msg EOLs
    while (msgLength < MAX_MSG_LENGTH)
    {
        ch = fgetc(pFile);
        if (ch == EOF || ch == EOL_CHAR)
            return msgLength;
        buf[msgLength] = (char)ch;
        msgLength++;
    }
    return 0;
}

static char *fileGetMessage(FILE *pFile) {
    // get msg from previous position to first EOF/EOL
    pFile = getToLastKnownPosition(pFile);
    char buf[MAX_MSG_LENGTH];
    unsigned int msgLength = copyMsgIntoCStringAndReturnItsLength(pFile, buf);
    CHECK_ZERO_RETURN_NULL(msgLength);

    // allocate memory for the msg
    char *msg = (char *) malloc(sizeof(char) * (msgLength + NULL_CHAR_SIZE));
    CHECK_NULL_RETURN_NULL(msg);

    // and copy the msg into it
    memcpy(msg, buf, sizeof(char) * msgLength);
    msg[msgLength] = NULL_CHAR;

    fileLatestCharPosition += msgLength + EOL_CHAR_SIZE;
    return msg;
}

static char *fileReceive()
{
    // open file for reading
    FILE * pFile = fopen(COMMUNICATION_FILE_NAME, FILE_READ_MODE);
    if (pFile == NULL)
    {
        PRINT_ERROR_MSG_AND_FUNCTION_NAME("fileReceive", "Failed to open the file");
        return NULL;
    }

    char *msgReceived = fileGetMessage(pFile);
    fclose(pFile);
    return msgReceived;
}
// ------------------------------ functions -----------------------------

ReturnValue fileServerInitConnection() {
    // create the file for communicating
    FILE *pFile = fopen(COMMUNICATION_FILE_NAME,FILE_WRITE_UPDATE_MODE);
    if (pFile == NULL)
    {
        PRINT_ERROR_MSG_AND_FUNCTION_NAME("fileServerInitConnection", "Failed to open the file");
        return PROJECT_ERROR;
    }
    fclose(pFile);
    updateLatestModified();
    return PROJECT_SUCCESS;
}

ReturnValue fileServerCloseConnection() {
    return PROJECT_SUCCESS;
}

char *fileListen() {
    // stat helps us checking for new msgs without opening it (using file info)
    struct stat fileStat;
    int result = stat(COMMUNICATION_FILE_NAME, &fileStat);
    if (result != 0)
    {
        PRINT_ERROR_MSG_AND_FUNCTION_NAME("fileListen", "Couldn't check file stat");
        return NULL;
    }

    while (true)
    {
        // if the current mtime is later than a prior mtime, the file has been modified
        stat(COMMUNICATION_FILE_NAME, &fileStat);
        if (fileLatestModified < fileStat.st_mtime)
            return fileReceive();
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

    updateLatestModified();
    fclose(pFile);
    fileLatestCharPosition += strlen(msg) + EOL_CHAR_SIZE;
    return PROJECT_SUCCESS;

cleanup:
    fclose(pFile);
    return PROJECT_ERROR;
}

ReturnValue fileClientInitConnection() {
    // open the file for appending, just to check the file is okay really
    FILE * pFile;
    pFile = fopen(COMMUNICATION_FILE_NAME,FILE_APPEND_MODE);
    if (pFile == NULL)
    {
        PRINT_ERROR_MSG_AND_FUNCTION_NAME("fileClientInitConnection", "Failed to open the file");
        return PROJECT_ERROR;
    }
    fclose(pFile);
    updateLatestModified();
    return PROJECT_SUCCESS;
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
    sleep(1);
    char *reply = fileListen();
    return reply;
}

char *fileClientReceive() {
    return fileReceive();
}
