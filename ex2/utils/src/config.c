
#include "config.h"
#include "nxjson.h"

#define MAX_JSON_FILE_SIZE 1024

/* for mains */
static CommunicationMethodCode communicationMethodCode = EMPTY_METHOD;
static char fileToTransferPath[MAX_JSON_VALUE_LENGTH] = {0};

/* For FileMethod */
static char serverCommunicationFilePath[MAX_JSON_VALUE_LENGTH] = {0};
static char serverTempCommunicationFilePath[MAX_JSON_VALUE_LENGTH] = {0};

static char clientCommunicationFilePath[MAX_JSON_VALUE_LENGTH] = {0};
static char clientTempCommunicationFilePath[MAX_JSON_VALUE_LENGTH] = {0};

static char serverLogCommunicationFilePath[MAX_JSON_VALUE_LENGTH] = {0};
static char clientLogCommunicationFilePath[MAX_JSON_VALUE_LENGTH] = {0};

/* For SocketMethod */
static char serverName[MAX_JSON_VALUE_LENGTH] = {0};
static int serverPort = 0;

static char clientServerName[MAX_JSON_VALUE_LENGTH] = {0};
static int clientServerPort = 0;


static char *loadJSONStringFromFile(const char *configFilePath)
{
    CHECK_NULL_RETURN_VALUE(configFilePath, NULL);

    FILE *pFile = fopen(configFilePath, FILE_READ_MODE);
    if (pFile == NULL)
        PRINT_ERROR_WITH_FUNCTION_AND_RETURN_NULL("loadJSONStringFromFile", "Failed to open the file");

    char buffer[MAX_JSON_FILE_SIZE] = {0};
    unsigned int counter = 0;
    int ch = 0;

    while (((ch = fgetc(pFile)) != EOF) && (counter < MAX_JSON_FILE_SIZE))
    {
        if ((char)ch != EOL_CHAR)
            buffer[counter++] = (char)ch;
    }

    fclose(pFile);

    char *JSONString = (char *) malloc(sizeof(char) * (counter + NULL_CHAR_SIZE));
    CHECK_NULL_RETURN_VALUE(JSONString, NULL);

    memcpy(JSONString, buffer, sizeof(char) * counter);
    JSONString[counter] = NULL_CHAR;

    return JSONString;
}

static ReturnValue copyStringIntoPathHolder(char *pathHolder, const nx_json *json, char *key)
{
    CHECK_NULL_RETURN_VALUE(pathHolder, PROJECT_ERROR);
    CHECK_NULL_RETURN_VALUE(json, PROJECT_ERROR);
    CHECK_NULL_RETURN_VALUE(key, PROJECT_ERROR);

    if (nx_json_get(json, key) == NULL)
        return PROJECT_ERROR;

    strncpy(pathHolder, nx_json_get(json, key)->text_value, MAX_JSON_VALUE_LENGTH);
    return PROJECT_SUCCESS;
}

static ReturnValue copyStringIntoPathHolders(const nx_json *json)
{
    CHECK_NULL_RETURN_VALUE(json, PROJECT_ERROR);
    ReturnValue result = PROJECT_ERROR;

    result = copyStringIntoPathHolder(fileToTransferPath, json, "fileToTransferPath");
    CHECK_ERROR_RETURN_VALUE(result, PROJECT_ERROR);

    result = copyStringIntoPathHolder(serverCommunicationFilePath, json, "serverCommunicationFilePath");
    CHECK_ERROR_RETURN_VALUE(result, PROJECT_ERROR);

    result = copyStringIntoPathHolder(serverTempCommunicationFilePath, json, "serverTempCommunicationFilePath");
    CHECK_ERROR_RETURN_VALUE(result, PROJECT_ERROR);

    result = copyStringIntoPathHolder(clientCommunicationFilePath, json, "clientCommunicationFilePath");
    CHECK_ERROR_RETURN_VALUE(result, PROJECT_ERROR);

    result = copyStringIntoPathHolder(clientTempCommunicationFilePath, json, "clientTempCommunicationFilePath");
    CHECK_ERROR_RETURN_VALUE(result, PROJECT_ERROR);

    result = copyStringIntoPathHolder(serverLogCommunicationFilePath, json, "serverLogCommunicationFilePath");
    CHECK_ERROR_RETURN_VALUE(result, PROJECT_ERROR);

    result = copyStringIntoPathHolder(clientLogCommunicationFilePath, json, "clientLogCommunicationFilePath");
    CHECK_ERROR_RETURN_VALUE(result, PROJECT_ERROR);

    result = copyStringIntoPathHolder(serverName, json, "serverName");
    CHECK_ERROR_RETURN_VALUE(result, PROJECT_ERROR);

    result = copyStringIntoPathHolder(clientServerName, json, "clientServerName");
    CHECK_ERROR_RETURN_VALUE(result, PROJECT_ERROR);

    return PROJECT_SUCCESS;
}

static ReturnValue copyNumbersIntoGlobals(const nx_json *json)
{
    CHECK_NULL_RETURN_VALUE(json, PROJECT_ERROR);

    CHECK_NULL_RETURN_VALUE(nx_json_get(json, "communicationMethodCode"), PROJECT_ERROR);
    communicationMethodCode = nx_json_get(json, "communicationMethodCode")->num.u_value;

    CHECK_NULL_RETURN_VALUE(nx_json_get(json, "serverPort"), PROJECT_ERROR);
    serverPort = (int) nx_json_get(json, "serverPort")->num.u_value;

    CHECK_NULL_RETURN_VALUE(nx_json_get(json, "clientServerPort"), PROJECT_ERROR);
    clientServerPort = (int) nx_json_get(json, "clientServerPort")->num.u_value;

    return PROJECT_SUCCESS;
}

ReturnValue initConfigurations(const char *configFilePath)
{
    CHECK_NULL_RETURN_VALUE(configFilePath, PROJECT_ERROR);
    ReturnValue result = PROJECT_ERROR;

    // Get JSON as string
    char *JSONString = loadJSONStringFromFile(configFilePath);
    CHECK_NULL_RETURN_VALUE(JSONString, PROJECT_ERROR);

    // Parse the expected arguments
    const nx_json *json = nx_json_parse(JSONString, 0);

    // Copy int globals
    result = copyNumbersIntoGlobals(json);
    CHECK_ERROR_GOTO_CLEANUP(result);

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

char *getFileToTransferPath()
{
    if (strnlen(fileToTransferPath, MAX_JSON_VALUE_LENGTH) == 0)
        return NULL;
    return fileToTransferPath;
}

char *getServerCommunicationFilePath()
{
    if (strnlen(serverCommunicationFilePath, MAX_JSON_VALUE_LENGTH) == 0)
        return NULL;
    return serverCommunicationFilePath;
}

char *getServerTempCommunicationFilePath()
{
    if (strnlen(serverTempCommunicationFilePath, MAX_JSON_VALUE_LENGTH) == 0)
        return NULL;
    return serverTempCommunicationFilePath;
}

char *getClientCommunicationFilePath()
{
    if (strnlen(clientCommunicationFilePath, MAX_JSON_VALUE_LENGTH) == 0)
        return NULL;
    return clientCommunicationFilePath;
}

char *getClientTempCommunicationFilePath()
{
    if (strnlen(clientTempCommunicationFilePath, MAX_JSON_VALUE_LENGTH) == 0)
        return NULL;
    return clientTempCommunicationFilePath;
}

char *getServerLogCommunicationFilePath()
{
    if (strnlen(serverLogCommunicationFilePath, MAX_JSON_VALUE_LENGTH) == 0)
        return NULL;
    return serverLogCommunicationFilePath;
}

char *getClientLogCommunicationFilePath()
{
    if (strnlen(clientLogCommunicationFilePath, MAX_JSON_VALUE_LENGTH) == 0)
        return NULL;
    return clientLogCommunicationFilePath;
}

char *getServerName()
{
    if (strnlen(serverName, MAX_JSON_VALUE_LENGTH) == 0)
        return NULL;
    return serverName;
}

int getServerPort()
{
    return serverPort;
}

char *getClientServerName()
{
    if (strnlen(clientServerName, MAX_JSON_VALUE_LENGTH) == 0)
        return NULL;
    return clientServerName;
}

int getClientServerPort()
{
    return clientServerPort;
}
