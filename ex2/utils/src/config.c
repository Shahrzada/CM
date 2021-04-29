
#include "config.h"
#include "nxjson.h"

#define MAX_JSON_FILE_SIZE 1024

/* for mains */
static CommunicationMethodCode communicationMethodCode = EMPTY_METHOD;

/* For Server.c */
static char fileToTransferPath[MAX_JSON_VALUE_LENGTH] = {0};

/* For FileMethod.c */
static char serverCommunicationFilePath[MAX_JSON_VALUE_LENGTH] = {0};
static char serverTempCommunicationFilePath[MAX_JSON_VALUE_LENGTH] = {0};

static char clientCommunicationFilePath[MAX_JSON_VALUE_LENGTH] = {0};
static char clientTempCommunicationFilePath[MAX_JSON_VALUE_LENGTH] = {0};

static char *loadJSONStringFromFile(const char *configFilePath)
{
    CHECK_NULL_RETURN_NULL(configFilePath);

    // open file for reading
    FILE *pFile = fopen(configFilePath, FILE_READ_MODE);
    if (pFile == NULL)
    {
        PRINT_ERROR_MSG_AND_FUNCTION_NAME("loadJSONStringFromFile", "Failed to open the file");
        return NULL;
    }

    // get JSON as string
    char buffer[MAX_JSON_FILE_SIZE] = {0};
    unsigned int counter = 0;
    int ch = 0;

    while (((ch = fgetc(pFile)) != EOF) && (counter < MAX_JSON_FILE_SIZE))
    {
        if ((char)ch != EOL_CHAR)
            buffer[counter++] = (char)ch;
    }

    fclose(pFile);

    // copy to allocated memory
    char *JSONString = (char *) malloc(sizeof(char) * (counter + NULL_CHAR_SIZE));
    CHECK_NULL_RETURN_NULL(JSONString);

    memcpy(JSONString, buffer, sizeof(char) * counter);
    JSONString[counter] = NULL_CHAR;

    return JSONString;
}

static ReturnValue copyStringIntoPathHolder(char *pathHolder, const nx_json *json, char *key)
{
    CHECK_NULL_RETURN_ERROR(pathHolder);
    CHECK_NULL_RETURN_ERROR(json);
    CHECK_NULL_RETURN_ERROR(key);

    if (nx_json_get(json, key) == NULL)
        return PROJECT_ERROR;

    strncpy(pathHolder, nx_json_get(json, key)->text_value, MAX_JSON_VALUE_LENGTH);
    return PROJECT_SUCCESS;
}

static ReturnValue copyStringIntoPathHolders(const nx_json *json)
{
    CHECK_NULL_RETURN_ERROR(json);
    ReturnValue result = PROJECT_ERROR;

    result = copyStringIntoPathHolder(fileToTransferPath, json, "fileToTransferPath");
    CHECK_ERROR_RETURN_ERROR(result);

    result = copyStringIntoPathHolder(serverCommunicationFilePath, json, "serverCommunicationFilePath");
    CHECK_ERROR_RETURN_ERROR(result);

    result = copyStringIntoPathHolder(serverTempCommunicationFilePath, json, "serverTempCommunicationFilePath");
    CHECK_ERROR_RETURN_ERROR(result);

    result = copyStringIntoPathHolder(clientCommunicationFilePath, json, "clientCommunicationFilePath");
    CHECK_ERROR_RETURN_ERROR(result);

    result = copyStringIntoPathHolder(clientTempCommunicationFilePath, json, "clientTempCommunicationFilePath");
    CHECK_ERROR_RETURN_ERROR(result);

    return PROJECT_SUCCESS;
}

ReturnValue initConfigurations(const char *configFilePath)
{
    CHECK_NULL_RETURN_ERROR(configFilePath);
    ReturnValue result = PROJECT_ERROR;

    // Get JSON as string
    char *JSONString = loadJSONStringFromFile(configFilePath);
    CHECK_NULL_RETURN_ERROR(JSONString);

    // Parse the expected arguments
    const nx_json *json = nx_json_parse(JSONString, 0);

    // Get the communication method first
    if (nx_json_get(json, "communicationMethodCode") == NULL)
        goto cleanup;
    communicationMethodCode = nx_json_get(json, "communicationMethodCode")->num.u_value;

    // Then copy paths into the global path holders
    result = copyStringIntoPathHolders(json);
    CHECK_ERROR_GOTO_CLEANUP(result);

    nx_json_free(json);
    free(JSONString);
    return PROJECT_SUCCESS;

    cleanup:
    nx_json_free(json);
    free(JSONString);
    return PROJECT_ERROR;
}

CommunicationMethodCode getCommunicationMethodCode()
{
    return communicationMethodCode;
}

const char *getFileToTransferPath()
{
    if (strnlen(fileToTransferPath, MAX_JSON_VALUE_LENGTH) == 0)
        return NULL;
    return fileToTransferPath;
}

const char *getServerCommunicationFilePath()
{
    if (strnlen(serverCommunicationFilePath, MAX_JSON_VALUE_LENGTH) == 0)
        return NULL;
    return serverCommunicationFilePath;
}

const char *getServerTempCommunicationFilePath()
{
    if (strnlen(serverTempCommunicationFilePath, MAX_JSON_VALUE_LENGTH) == 0)
        return NULL;
    return serverTempCommunicationFilePath;
}

const char *getClientCommunicationFilePath()
{
    if (strnlen(clientCommunicationFilePath, MAX_JSON_VALUE_LENGTH) == 0)
        return NULL;
    return clientCommunicationFilePath;
}

const char *getClientTempCommunicationFilePath()
{
    if (strnlen(clientTempCommunicationFilePath, MAX_JSON_VALUE_LENGTH) == 0)
        return NULL;
    return clientTempCommunicationFilePath;
}
