
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

static int serverPreviousReadPosition = 0;

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
static int fileGetTotalCharsUntilEOFOrNextCommand(FILE * pFile)
{
    CHECK_NULL_RETURN_ZERO(pFile);

    bool inQuotationMarks = false, inApostrophes = false;
    int count = 0, ch = 0;

    while((ch = fgetc(pFile)) != EOF) {
        // we ignore EOL that are part of the client's msg, we consider it part of
        // the client's msg if it is inside quotation marks or apostrophes.
        if (ch == QUOTATION_MARK_CHAR_ASCII_DEC_VALUE)
            inQuotationMarks = (inQuotationMarks == false) ? true : false;

        else if (ch == APOSTROPHE_CHAR_ASCII_DEC_VALUE)
            inApostrophes = (inApostrophes == false) ? true : false;

        if (ch == EOL_CHAR_ASCII_DEC_VALUE && !inQuotationMarks && !inApostrophes)
            break;

        count++;
    }
    return count;
}

static ReturnValue fileGetMessage(FILE *pFile, char *clientMsg)
{
    CHECK_NULL_RETURN_ERROR(pFile);
    CHECK_NULL_RETURN_ERROR(clientMsg);

    // moving file pointer to the first unread char (that's why there's a +1)
    int result = fseek(pFile, serverPreviousReadPosition + NEWLINE_CHAR_SIZE, SEEK_SET);
    CHECK_NON_ZERO_RETURN_ERROR(result);

    // fetch the total length of one client command
    int msgsLength = fileGetTotalCharsUntilEOFOrNextCommand(pFile);
    clientMsg = (char *) realloc(clientMsg, sizeof(char) * (msgsLength + NULL_CHAR_SIZE));
    CHECK_NULL_RETURN_ERROR(clientMsg);

    // moving file pointer back again to the first unread char (it moved because of the counting)
    result = fseek(pFile, serverPreviousReadPosition + NEWLINE_CHAR_SIZE, SEEK_SET);
    CHECK_NON_ZERO_RETURN_ERROR(result);

    // copying the command to our cString
    size_t totalCopiedChars = fread(clientMsg, sizeof(char), msgsLength, pFile);
    if (totalCopiedChars != msgsLength)
        return ERROR;

    // making sure it is indeed a valid cString and updating the read position for next time
    clientMsg[msgsLength] = NULL_CHAR;
    serverPreviousReadPosition += msgsLength;
    return SUCCESS;
}

static ReturnValue fileReceive(char *msg)
{
    CHECK_NULL_RETURN_ERROR(msg);

    // open file for reading
    FILE * pFile = fopen(COMMUNICATION_FILE_NAME, FILE_READ_MODE);
    CHECK_FILE_NULL_PRINT_OPEN_ERROR_GOTO_CLEANUP(pFile);

    ReturnValue result = fileGetMessage(pFile, msg);
    fclose(pFile);
    return result;

cleanup:
    fclose(pFile);
    return ERROR;
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

    // never forget to close the file before continuing
    fclose(pFile);
    return SUCCESS;

cleanup:
    fclose(pFile);
    return ERROR;
}

ReturnValue fileListen(char *msg) {
    CHECK_NULL_RETURN_ERROR(msg);

    // stat helps us checking for new msgs without opening it (by the file's size)
    struct stat st;
    stat(COMMUNICATION_FILE_NAME, &st);
    int fileSize = st.st_size;

    // important: in the future the condition will be different according to when we kill
    //              the server from main. same regarding sleep.
    while (true)
    {
        stat(COMMUNICATION_FILE_NAME, &st);
        if (fileSize < st.st_size)
            return fileReceive(msg);
        sleep(1);
    }
}

ReturnValue fileSend(char *msg, char *reply) {
    return ERROR;
}

ReturnValue fileClientCloseConnect() {
    return ERROR;
}
