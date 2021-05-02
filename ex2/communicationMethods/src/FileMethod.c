
// ------------------------------ includes ------------------------------
#include <unistd.h>
#include <sys/stat.h>

#include "FileMethod.h"
#include "config.h"

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

const char *fileServerCommunicationFilePath = NULL;
const char *fileServerTempCommunicationFilePath = NULL; 
const char *fileClientCommunicationFilePath = NULL;
const char *fileClientTempCommunicationFilePath = NULL;

// ------------------------------ private functions -----------------------------

static ReturnValue initFilePaths()
{
    fileServerCommunicationFilePath = getServerCommunicationFilePath();
    CHECK_NULL_RETURN_VALUE(fileServerCommunicationFilePath, PROJECT_ERROR);

    fileServerTempCommunicationFilePath = getServerTempCommunicationFilePath();
    CHECK_NULL_RETURN_VALUE(fileServerTempCommunicationFilePath, PROJECT_ERROR);

    fileClientCommunicationFilePath = getClientCommunicationFilePath();
    CHECK_NULL_RETURN_VALUE(fileClientCommunicationFilePath, PROJECT_ERROR);

    fileClientTempCommunicationFilePath = getClientTempCommunicationFilePath();
    CHECK_NULL_RETURN_VALUE(fileClientTempCommunicationFilePath, PROJECT_ERROR);

    return PROJECT_SUCCESS;
}

static ReturnValue fileRemoveMsgFromFile(const char *fileName)
{
    CHECK_NULL_RETURN_VALUE(fileName, PROJECT_ERROR);
    CHECK_NULL_RETURN_VALUE(fileServerTempCommunicationFilePath, PROJECT_ERROR);
    CHECK_NULL_RETURN_VALUE(fileClientTempCommunicationFilePath, PROJECT_ERROR);

    FILE *tempFile = NULL;

    FILE *srcFile  = fopen(fileName, FILE_READ_MODE);
    if (srcFile == NULL)
        return PROJECT_ERROR;

    tempFile = (isServer) ? fopen(fileServerTempCommunicationFilePath, FILE_WRITE_UPDATE_MODE)
                          : fopen(fileClientTempCommunicationFilePath, FILE_WRITE_UPDATE_MODE);

    if (tempFile == NULL)
    {
        fclose(srcFile);
        return PROJECT_ERROR;
    }

    // Ignore the first msg
    char buffer[MAX_MSG_LENGTH] = {0};
    (fgets(buffer, MAX_MSG_LENGTH, srcFile));

    // And copy all other msgs into the temp file
    while (fgets(buffer, MAX_MSG_LENGTH, srcFile) != NULL)
        fputs(buffer, tempFile);

    fclose(srcFile);
    fclose(tempFile);

    int result = remove(fileName);
    if (result != 0)
        return PROJECT_ERROR;

    result = (isServer) ? rename(fileServerTempCommunicationFilePath, fileName)
                        : rename(fileClientTempCommunicationFilePath, fileName);

    return (result != 0) ? PROJECT_ERROR : PROJECT_SUCCESS;
}

static unsigned int fileReceive(char *buffer, const char *fileName)
{
    CHECK_NULL_RETURN_VALUE(buffer, 0);
    CHECK_NULL_RETURN_VALUE(fileName, 0);

    FILE *pFile = fopen(fileName, FILE_READ_MODE);
    if (pFile == NULL)
        PRINT_ERROR_WITH_FUNCTION_AND_RETURN_ZERO("fileReceive", "Failed to open the file");

    if (fgets(buffer, MAX_MSG_LENGTH, pFile) == NULL)
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
    CHECK_NULL_RETURN_VALUE(buffer, 0);
    CHECK_NULL_RETURN_VALUE(fileName, 0);

    // stat helps us checking for new msgs without opening it (using file size)
    struct stat fileStat;
    int result = stat(fileName, &fileStat);
    if (result != 0)
        PRINT_ERROR_WITH_FUNCTION_AND_RETURN_ZERO("fileListen", "Couldn't check file stat");

    size_t fileSize = (isServer) ? serverFileEmptySize : clientFileEmptySize;
    while (true)
    {
        sleep(1);
        stat(fileName, &fileStat);
        if (fileSize < fileStat.st_size)
            return fileReceive(buffer, fileName);
    }
}

static ReturnValue fileSend(const char *msg, const char *fileName)
{
    CHECK_NULL_RETURN_VALUE(msg, PROJECT_ERROR);
    CHECK_NULL_RETURN_VALUE(fileName, PROJECT_ERROR);

    FILE *pFile = fopen(fileName, FILE_APPEND_MODE);
    CHECK_FILE_NULL_PRINT_OPEN_ERROR_GOTO_CLEANUP(pFile);

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
    CHECK_NULL_RETURN_VALUE(fileServerCommunicationFilePath, PROJECT_ERROR);
    CHECK_NULL_RETURN_VALUE(fileClientCommunicationFilePath, PROJECT_ERROR);
    struct stat fileStat;
    int result = -1;

    result = (isServer) ? stat(fileServerCommunicationFilePath, &fileStat)
                        : stat(fileClientCommunicationFilePath, &fileStat);
    if (result != 0)
        PRINT_ERROR_WITH_FUNCTION_AND_RETURN_ERROR("fileSetEmptyFileSize", "Couldn't check file stat");

    (isServer) ? (serverFileEmptySize = fileStat.st_size) : (clientFileEmptySize = fileStat.st_size);
    return PROJECT_SUCCESS;
}

// ------------------------------ functions -----------------------------

ReturnValue fileServerInitConnection()
{
    isServer = true;
    ReturnValue result = initFilePaths();
    CHECK_ERROR_RETURN_VALUE(result, PROJECT_ERROR);

    CHECK_NULL_RETURN_VALUE(fileServerCommunicationFilePath, PROJECT_ERROR);
    FILE *pFile = fopen(fileServerCommunicationFilePath, FILE_WRITE_UPDATE_MODE);
    if (pFile == NULL)
        PRINT_ERROR_WITH_FUNCTION_AND_RETURN_ERROR("fileServerInitConnection", "Failed to open the file");

    fclose(pFile);
    return fileSetEmptyFileSize();
}

ReturnValue fileServerCloseConnection()
{
    return PROJECT_SUCCESS;
}

unsigned int fileServerListen(char *buffer)
{
    CHECK_NULL_RETURN_VALUE(buffer, 0);
    CHECK_NULL_RETURN_VALUE(fileServerCommunicationFilePath, PROJECT_ERROR);
    return fileListen(buffer, fileServerCommunicationFilePath);
}

ReturnValue fileServerSend(const char *msg, unsigned int msgLength)
{
    CHECK_NULL_RETURN_VALUE(msg, PROJECT_ERROR);
    CHECK_NULL_RETURN_VALUE(fileClientCommunicationFilePath, PROJECT_ERROR);
    return fileSend(msg, fileClientCommunicationFilePath);
}

ReturnValue fileClientInitConnection()
{
    ReturnValue result = initFilePaths();
    CHECK_ERROR_RETURN_VALUE(result, PROJECT_ERROR);

    FILE *pFile;
    CHECK_NULL_RETURN_VALUE(fileClientCommunicationFilePath, PROJECT_ERROR);
    pFile = fopen(fileClientCommunicationFilePath, FILE_WRITE_UPDATE_MODE);
    if (pFile == NULL)
        PRINT_ERROR_WITH_FUNCTION_AND_RETURN_ERROR("fileClientInitConnection", "Failed to open the file");

    fclose(pFile);
    return fileSetEmptyFileSize();
}

ReturnValue fileClientCloseConnection()
{
    return PROJECT_SUCCESS;
}

unsigned int fileClientSend(const char *msg, unsigned int msgLength, char *buffer)
{
    CHECK_NULL_RETURN_VALUE(msg, 0);
    CHECK_NULL_RETURN_VALUE(buffer, 0);

    CHECK_NULL_RETURN_VALUE(fileServerCommunicationFilePath, PROJECT_ERROR);
    ReturnValue result = fileSend(msg, fileServerCommunicationFilePath);
    CHECK_ERROR_RETURN_VALUE(result, 0);

    CHECK_NULL_RETURN_VALUE(fileClientCommunicationFilePath, PROJECT_ERROR);
    return fileListen(buffer, fileClientCommunicationFilePath);
}
