
// ------------------------------ includes ------------------------------
#include <unistd.h>
#include <sys/stat.h>

#include "FileMethod.h"

// -------------------------- const definitions -------------------------
// CR: This and the entire configuration for the server be in a single file.
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
static unsigned int fileLatestCharPosition = 0;

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
// CR: naming is a bit weird in this case.
static FILE *getToLastKnownPosition(FILE *pFile) {
    CHECK_NULL_RETURN_NULL(pFile);

    if (fileLatestCharPosition == 0)
        return pFile;

    unsigned int counter = 0;
    // CR: there must be a better way to do this... use seek
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

    while (msgLength < MAX_MSG_LENGTH)
    {
        ch = fgetc(pFile);
        if (ch == EOF || ch == EOT_CHAR_ASCII_DEC_VALUE)
            return msgLength;
        buf[msgLength] = (char)ch;
        msgLength++;
    }
    return 0;
}

static char *fileGetMessage(FILE *pFile) {
    CHECK_NULL_RETURN_NULL(pFile);

    // get msg from previous position to first EOF/EOT
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

    fileLatestCharPosition += msgLength + EOT_CHAR_SIZE;
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
    {   // CR: not really liking the never ending file, its really nice for debugging
        //     but it keeps a log of the entire connection for what?
        // if the current mtime is later than a prior mtime, the file has been modified
        stat(COMMUNICATION_FILE_NAME, &fileStat);
        if (fileLatestModified < fileStat.st_mtime)
        {
            sleep(1);
            return fileReceive();
        }
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

    // add EOT to differentiate msgs
    // CR: this is kind of a problem and it shouldn't really be here
    //     each sending method should be agnostic to the data
    char buf[2]= {EOT_CHAR_ASCII_DEC_VALUE};
    writingResult = fputs(buf, pFile);
    CHECK_NEGATIVE_PRINT_WRITE_ERROR_GOTO_CLEANUP(writingResult);

    fclose(pFile);
    updateLatestModified();
    fileLatestCharPosition += strlen(msg) + EOT_CHAR_SIZE;
    return PROJECT_SUCCESS;

cleanup:
    fclose(pFile);
    return PROJECT_ERROR;
}

ReturnValue fileClientInitConnection() {
    // open the file for appending
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
    char *reply = fileListen();
    return reply;
}

char *fileClientReceive() { // CR: why does this function exist?
    return fileReceive();
}
