
// ------------------------------ includes ------------------------------
#include <unistd.h>
#include <sys/stat.h>

#include "FileMethod.h"

// -------------------------- const definitions -------------------------
// CR: This and the entire configuration for the server be in a single file.
#define SERVER_COMMUNICATION_FILE_NAME "server_bebi.txt"
#define CLIENT_COMMUNICATION_FILE_NAME "client_bebi.txt"
#define SERVER_TEMP_COMMUNICATION_FILE_NAME "server_bebi_temp.txt"
#define CLIENT_TEMP_COMMUNICATION_FILE_NAME "client_bebi_temp.txt"
#define LOG_FILE_NAME "bebi_log.txt"

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

// ------------------------------ global variables -----------------------------

static bool isServer = false;

static size_t serverFileEmptySize = 0;
static size_t clientFileEmptySize = 0;

// ------------------------------ private functions -----------------------------

//static ReturnValue updateLatestModified()
//{
//    // stat helps us checking for new msgs without opening it (using file info)
//    struct stat fileStat;
//    int result = 0;
//
//    if (isServer)
//        result = stat(SERVER_COMMUNICATION_FILE_NAME, &fileStat);
//    else
//        result = stat(CLIENT_COMMUNICATION_FILE_NAME, &fileStat);
//
//    if (result != 0)
//    {
//        PRINT_ERROR_MSG_AND_FUNCTION_NAME("fileListen", "Couldn't check file stat");
//        return PROJECT_ERROR;
//    }
//
//    if (isServer)
//        serverFileLatestModified = fileStat.st_mtime;
//    else
//        clientFileLatestModified = fileStat.st_mtime;
//
//    return PROJECT_SUCCESS;
//}

static ReturnValue fileRemoveMsgFromFile(const char *fileName)
{
    CHECK_NULL_RETURN_ERROR(fileName);
    FILE *tempFile = NULL;

    FILE *srcFile  = fopen(fileName, FILE_READ_MODE);
    if (srcFile == NULL)
        return PROJECT_ERROR;

    if (isServer)
        tempFile = fopen(SERVER_TEMP_COMMUNICATION_FILE_NAME, FILE_WRITE_UPDATE_MODE);
    else
        tempFile = fopen(CLIENT_TEMP_COMMUNICATION_FILE_NAME, FILE_WRITE_UPDATE_MODE);

    if (tempFile == NULL)
    {
        fclose(srcFile);
        return PROJECT_ERROR;
    }

    // Ignore the first msg
    char buffer[MAX_MSG_LENGTH] = {0};
    char *line = (fgets(buffer, MAX_MSG_LENGTH, srcFile));

    // And copy all other msgs
    while (true)
    {
        line = (fgets(buffer, MAX_MSG_LENGTH, srcFile));
        if (line == NULL)
            break;
        fputs(buffer, tempFile);
    }

    fclose(srcFile);
    fclose(tempFile);

    // Delete src file
    int result = remove(fileName);
    if (result != 0)
        return PROJECT_ERROR;

    // And rename temp file as src
    if (isServer)
        result = rename(SERVER_TEMP_COMMUNICATION_FILE_NAME, fileName);
    else
        result = rename(CLIENT_TEMP_COMMUNICATION_FILE_NAME, fileName);

    return (result != 0) ? PROJECT_ERROR : PROJECT_SUCCESS;
}

static unsigned int fileReceive(char *buffer, const char *fileName)
{
    CHECK_NULL_RETURN_ZERO(buffer);
    CHECK_NULL_RETURN_ZERO(fileName);

    // open file for reading
    FILE *pFile = fopen(fileName, FILE_READ_MODE);
    if (pFile == NULL)
    {
        PRINT_ERROR_MSG_AND_FUNCTION_NAME("fileReceive", "Failed to open the file");
        return 0;
    }

    // get first msg
    if (fgets (buffer, MAX_MSG_LENGTH, pFile) == NULL)
    {
        fclose(pFile);
        return 0;
    }

    fclose(pFile);

    fileRemoveMsgFromFile(fileName);
    return strnlen(buffer, MAX_MSG_LENGTH);
}

static unsigned int fileListen(char *buffer, const char *fileName)
{
    CHECK_NULL_RETURN_ZERO(buffer);
    CHECK_NULL_RETURN_ZERO(fileName);

    // stat helps us checking for new msgs without opening it (using file info)
    struct stat fileStat;
    int result = stat(fileName, &fileStat);
    if (result != 0)
    {
        PRINT_ERROR_MSG_AND_FUNCTION_NAME("fileListen", "Couldn't check file stat");
        return 0;
    }

    while (true)
    {
        sleep(1);
        stat(fileName, &fileStat);
        if (isServer)
        {
            if (serverFileEmptySize < fileStat.st_size)
                return fileReceive(buffer, SERVER_COMMUNICATION_FILE_NAME);
        }
        else
        {
            if (clientFileEmptySize < fileStat.st_size)
                return fileReceive(buffer, CLIENT_COMMUNICATION_FILE_NAME);
        }
    }
}

static ReturnValue fileSend(const char *msg, const char *fileName)
{
    CHECK_NULL_RETURN_ERROR(msg);
    CHECK_NULL_RETURN_ERROR(fileName);

    // open file for appending
    FILE *pFile = fopen(fileName, FILE_APPEND_MODE);
    CHECK_FILE_NULL_PRINT_OPEN_ERROR_GOTO_CLEANUP(pFile);

    // write given msg
    int writingResult = fputs(msg, pFile);
    CHECK_NEGATIVE_PRINT_WRITE_ERROR_GOTO_CLEANUP(writingResult);
    fclose(pFile);

    return PROJECT_SUCCESS;

    cleanup:
    fclose(pFile);
    return PROJECT_ERROR;
}

static ReturnValue fileSetEmptyFileSize()
{
    struct stat fileStat;
    int result = -1;

    if (isServer)
        result = stat(SERVER_COMMUNICATION_FILE_NAME, &fileStat);
    else
        result = stat(CLIENT_COMMUNICATION_FILE_NAME, &fileStat);

    if (result != 0)
    {
        PRINT_ERROR_MSG_AND_FUNCTION_NAME("fileSetEmptyFileSize", "Couldn't check file stat");
        return PROJECT_ERROR;
    }

    if (isServer)
        serverFileEmptySize = fileStat.st_size;
    else
        clientFileEmptySize = fileStat.st_size;

    return PROJECT_SUCCESS;
}
// ------------------------------ functions -----------------------------

ReturnValue fileServerInitConnection()
{
    isServer = true;

    // create the file for communicating
    FILE *pFile = fopen(SERVER_COMMUNICATION_FILE_NAME,FILE_WRITE_UPDATE_MODE);
    if (pFile == NULL)
    {
        PRINT_ERROR_MSG_AND_FUNCTION_NAME("fileServerInitConnection", "Failed to open the file");
        return PROJECT_ERROR;
    }

    fclose(pFile);
    return fileSetEmptyFileSize();
}

ReturnValue fileServerCloseConnection()
{
    return PROJECT_SUCCESS;
}

unsigned int fileServerListen(char *buffer)
{
    CHECK_NULL_RETURN_ZERO(buffer);
    return fileListen(buffer, SERVER_COMMUNICATION_FILE_NAME);
}

ReturnValue fileServerSend(const char *msg, unsigned int msgLength)
{
    CHECK_NULL_RETURN_ERROR(msg);
    return fileSend(msg, CLIENT_COMMUNICATION_FILE_NAME);
}

ReturnValue fileClientInitConnection()
{
    FILE *pFile;
    pFile = fopen(CLIENT_COMMUNICATION_FILE_NAME, FILE_WRITE_UPDATE_MODE);
    if (pFile == NULL)
    {
        PRINT_ERROR_MSG_AND_FUNCTION_NAME("fileClientInitConnection", "Failed to open the file");
        return PROJECT_ERROR;
    }

    fclose(pFile);
    return fileSetEmptyFileSize();
}

ReturnValue fileClientCloseConnection()
{
    return PROJECT_SUCCESS;
}

unsigned int fileClientSend(const char *msg, unsigned int msgLength, char *buffer)
{
    CHECK_NULL_RETURN_ZERO(msg);
    CHECK_NULL_RETURN_ZERO(buffer);

    // send the msg
    ReturnValue result = fileSend(msg, SERVER_COMMUNICATION_FILE_NAME);
    CHECK_ERROR_RETURN_ZERO(result);

    // wait for server reply
    return fileListen(buffer, CLIENT_COMMUNICATION_FILE_NAME);
}

unsigned int fileClientReceive(char *buffer)
{
    return fileReceive(buffer, CLIENT_COMMUNICATION_FILE_NAME);
}
