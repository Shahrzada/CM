
#include <libgen.h>
#include "fileUtils.h"

char *loadFileAsString(const char *configFilePath, unsigned int maxBufferSize)
{
    CHECK_NULL_RETURN_NULL(configFilePath);

    // open file for reading
    FILE *pFile = fopen(configFilePath, FILE_READ_MODE);
    if (pFile == NULL)
    {
        PRINT_ERROR_MSG_AND_FUNCTION_NAME("loadFileAsString", "Failed to open the file");
        return NULL;
    }

    // get file as a string
    char buffer[maxBufferSize];
    memset(buffer, NULL_CHAR, sizeof(char) * maxBufferSize);
    unsigned int counter = 0;
    int ch = 0;

    while (((ch = fgetc(pFile)) != EOF) && (counter < maxBufferSize))
    {
        if ((char)ch != EOL_CHAR)
            buffer[counter++] = (char)ch;
    }

    fclose(pFile);

    // copy to allocated memory
    char *fileString = (char *) malloc(sizeof(char) * (counter + NULL_CHAR_SIZE));
    CHECK_NULL_RETURN_NULL(fileString);

    memcpy(fileString, buffer, sizeof(char) * counter);
    fileString[counter] = NULL_CHAR;

    return fileString;
}
