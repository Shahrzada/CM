
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
#define INIT_FILE_CLIENT_SUCCESS_MSG "[Client]: Connected successfully.\n"
#define CLOSED_FILE_SUCCESS_MSG "[Server]: Closed the file successfully.\n"
#define CLOSED_FILE_CLIENT_SUCCESS_MSG "[Client]: Closed the file successfully.\n"

#define SERVER_READ_STARTING_POSITION 41

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

static bool isServer = true;
static int serverPreviousReadPosition = 0;
static int clientPreviousReadPosition = 0;

/* Returns total chars left from pFile position until EOF */
static int fileGetTotalCharsFromFilePointer(FILE *pFile)
{
    CHECK_NULL_RETURN_ZERO(pFile);

    int count = 0;
    while(fgetc(pFile) != EOF)
        count++;

    return count;
}

/* Returns total chars left from pFile position until next either EOL or EOF */
static int fileGetTotalCharsUntilEOFOrNextEOL(FILE *pFile)
{
    CHECK_NULL_RETURN_ZERO(pFile);
    int count = 0, ch = 0;

    while((ch = fgetc(pFile)) != EOF) {
        // we ignore EOL that are part of the client's msg, we consider it part of
        // the client's msg if it is inside quotation marks or apostrophes.

        if (ch == EOL_CHAR_ASCII_DEC_VALUE)
        {
            if (count == 0)
                continue;
            else
                break;
        }

        count++;
    }
    return count;
}

static char *fileGetMessage(FILE *pFile)
{
    CHECK_NULL_RETURN_NULL(pFile);

    int previousReadPosition = clientPreviousReadPosition;
    if (isServer)
        previousReadPosition = serverPreviousReadPosition;

    // moving file pointer to the first unread char
    int result = fseek(pFile, previousReadPosition + NEWLINE_CHAR_SIZE + NEWLINE_CHAR_SIZE, SEEK_SET);
    CHECK_NON_ZERO_RETURN_NULL(result);

    // fetch the total length of chars until EOF or EOL
    int msgLength = fileGetTotalCharsUntilEOFOrNextEOL(pFile);
    char *msg = (char *) malloc(sizeof(char) * (msgLength + NULL_CHAR_SIZE));
    CHECK_NULL_RETURN_NULL(msg);

    // moving file pointer back again to the first unread char
    result = fseek(pFile, previousReadPosition + NEWLINE_CHAR_SIZE + NEWLINE_CHAR_SIZE, SEEK_SET);
    if (result != 0)
        goto cleanup;

    // copying the command to our cString
    size_t totalCopiedChars = fread(msg, sizeof(char), msgLength, pFile);
    if (totalCopiedChars != msgLength)
        goto cleanup;

    // making sure it is indeed a valid cString
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
    FILE * pFile = fopen(COMMUNICATION_FILE_NAME,FILE_WRITE_UPDATE_MODE);
    CHECK_FILE_NULL_PRINT_OPEN_ERROR_GOTO_CLEANUP(pFile);

    // write first success msg
    int result = fputs(INIT_FILE_SUCCESS_MSG, pFile);
    CHECK_NEGATIVE_PRINT_WRITE_ERROR_GOTO_CLEANUP(result);
    fclose(pFile);

    serverPreviousReadPosition = SERVER_READ_STARTING_POSITION;
    return SUCCESS;

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
    // open the file for appending
    FILE * pFile;
    pFile = fopen(COMMUNICATION_FILE_NAME,FILE_APPEND_MODE);
    CHECK_FILE_NULL_PRINT_OPEN_ERROR_GOTO_CLEANUP(pFile);

    // write init connection success msg
    int result = fputs(INIT_FILE_CLIENT_SUCCESS_MSG, pFile);
    CHECK_NEGATIVE_PRINT_WRITE_ERROR_GOTO_CLEANUP(result);

    fclose(pFile);
    isServer = false;
    return SUCCESS;

cleanup:
    fclose(pFile);
    return ERROR;
}

char *fileListen() {
    // stat helps us checking for new msgs without opening it (by the file's size)
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

char *fileSend(char *msg) {
    // open the file for appending
    FILE *pFile = fopen(COMMUNICATION_FILE_NAME,FILE_APPEND_MODE);
    CHECK_FILE_NULL_PRINT_OPEN_ERROR_GOTO_CLEANUP(pFile);

    // write wanted msg
    int writingResult = fputs(msg, pFile);
    CHECK_NEGATIVE_PRINT_WRITE_ERROR_GOTO_CLEANUP(writingResult);

    writingResult = fputs("\n", pFile);
    CHECK_NEGATIVE_PRINT_WRITE_ERROR_GOTO_CLEANUP(writingResult);
    fclose(pFile);

    // update position for future listening/reading
    pFile = fopen(COMMUNICATION_FILE_NAME,FILE_READ_MODE);
    CHECK_FILE_NULL_PRINT_OPEN_ERROR_GOTO_CLEANUP(pFile);

    int position = fileGetTotalCharsFromFilePointer(pFile);
    if (position == 0)
        goto cleanup;

    if (isServer)
        serverPreviousReadPosition = position;
    else
        clientPreviousReadPosition = position;

    fclose(pFile);

    // wait until reply
    return fileListen();

cleanup:
    fclose(pFile);
    return NULL;
}

ReturnValue fileClientCloseConnect() {
    // open the file for appending
    FILE * pFile;
    pFile = fopen(COMMUNICATION_FILE_NAME,FILE_APPEND_MODE);
    CHECK_FILE_NULL_PRINT_OPEN_ERROR_GOTO_CLEANUP(pFile);

    // write last closing success msg
    int result = fputs(CLOSED_FILE_CLIENT_SUCCESS_MSG, pFile);
    CHECK_NEGATIVE_PRINT_WRITE_ERROR_GOTO_CLEANUP(result);

    fclose(pFile);
    return SUCCESS;

cleanup:
    fclose(pFile);
    return ERROR;
}
