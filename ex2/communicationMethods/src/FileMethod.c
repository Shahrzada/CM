
// ------------------------------ includes ------------------------------
#include <unistd.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/FileMethod.h"

// -------------------------- const definitions -------------------------

#define COMMUNICATION_FILE_NAME "bebi.txt"
#define INIT_FILE_SUCCESS_MSG "[Server]: Created the file successfully.\n"
#define CLOSED_FILE_SUCCESS_MSG "[Server]: Closed the file successfully.\n"

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

// ------------------------------ static global variables -----------------------------

static bool isServer = true;
static int serverPreviousReadPosition = 0;
static int clientPreviousReadPosition = 0;

// ------------------------------ private functions -----------------------------


/* Returns total chars left from pFile position until EOF */
static int fileGetTotalCharsFromFilePointer(FILE *pFile)
{
    CHECK_NULL_RETURN_ZERO(pFile);

    int count = 0;
    while(fgetc(pFile) != EOF)
        count++;

    return count;
}

/* Returns total chars left from pFile position until next EOF or EOL */
static int fileGetTotalCharsUntilEOFOrEOL(FILE *pFile)
{
    CHECK_NULL_RETURN_ZERO(pFile);
    int count = 0, ch = 0;

    while((ch = fgetc(pFile)) != EOF) {
        if (ch == EOL_CHAR_ASCII_DEC_VALUE)
                break;
        count++;
    }

    return count;
}

static ReturnValue updateReadPosition()
{
    FILE *pFile = fopen(COMMUNICATION_FILE_NAME,FILE_READ_MODE);
    CHECK_NULL_RETURN_ERROR(pFile);

    int position = fileGetTotalCharsFromFilePointer(pFile);
    if (position == 0)
    {
        fclose(pFile);
        PRINT_ERROR_MSG_AND_FUNCTION_NAME("updateReadPosition", "Empty file");
        return ERROR;
    }

    if (isServer)
        serverPreviousReadPosition = position;
    else
        clientPreviousReadPosition = position;

    fclose(pFile);
    return SUCCESS;
}

static char *fileGetMessage(FILE *pFile)
{
    CHECK_NULL_RETURN_NULL(pFile);
    int previousReadPosition = (isServer) ? serverPreviousReadPosition : clientPreviousReadPosition;

    // moving file pointer to the first unread char
    int result = fseek(pFile, previousReadPosition + NEWLINE_CHAR_SIZE, SEEK_SET);
    CHECK_NON_ZERO_RETURN_NULL(result);

    // fetch the total length of chars until EOF or EOL
    int msgLength = fileGetTotalCharsUntilEOFOrEOL(pFile);
    char *msg = (char *) malloc(sizeof(char) * (msgLength + NULL_CHAR_SIZE));
    CHECK_NULL_RETURN_NULL(msg);

    // moving file pointer back to the first unread char
    result = fseek(pFile, previousReadPosition + NEWLINE_CHAR_SIZE, SEEK_SET);
    if (result != 0)
        goto cleanup;

    // get the msg
    size_t totalCopiedChars = fread(msg, sizeof(char), msgLength, pFile);
    if (totalCopiedChars != msgLength)
        goto cleanup;

    msg[msgLength] = NULL_CHAR;
    return msg;

cleanup:
    free(msg);
    return NULL;
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

ReturnValue fileServerInitConnect() {
    // create and open the file
    FILE *pFile = fopen(COMMUNICATION_FILE_NAME,FILE_WRITE_UPDATE_MODE);
    CHECK_FILE_NULL_PRINT_OPEN_ERROR_GOTO_CLEANUP(pFile);

    // write first success msg
    int result = fputs(INIT_FILE_SUCCESS_MSG, pFile);
    CHECK_NEGATIVE_PRINT_WRITE_ERROR_GOTO_CLEANUP(result);
    fclose(pFile);

    return updateReadPosition();

cleanup:
    fclose(pFile);
    return ERROR;
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
    return SUCCESS;

cleanup:
    fclose(pFile);
    return ERROR;
}

ReturnValue fileClientInitConnect() {
    isServer = false;

    // open the file for appending
    FILE * pFile;
    pFile = fopen(COMMUNICATION_FILE_NAME,FILE_APPEND_MODE);
    CHECK_FILE_NULL_PRINT_OPEN_ERROR_GOTO_CLEANUP(pFile);

    fclose(pFile);
    return SUCCESS;

cleanup:
    fclose(pFile);
    return ERROR;
}

ReturnValue fileClientCloseConnect() {
    // open the file for appending
    FILE * pFile;
    pFile = fopen(COMMUNICATION_FILE_NAME,FILE_APPEND_MODE);
    CHECK_FILE_NULL_PRINT_OPEN_ERROR_GOTO_CLEANUP(pFile);

    fclose(pFile);
    return SUCCESS;

cleanup:
    fclose(pFile);
    return ERROR;
}

char *fileListen() {
    // stat helps us checking for new msgs without opening it (using file size)
    struct stat st;
    stat(COMMUNICATION_FILE_NAME, &st);
    int fileSize = st.st_size;

    while (true)
    {
        stat(COMMUNICATION_FILE_NAME, &st);
        if (fileSize < st.st_size)
            return fileReceive();
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
    return updateReadPosition();

cleanup:
    fclose(pFile);
    return ERROR;
}

char *fileClientSend(const char *msg) {
    CHECK_NULL_RETURN_NULL(msg);
    ReturnValue result = fileSend(msg);
    CHECK_ERROR_RETURN_NULL(result);
    return fileListen();
}
