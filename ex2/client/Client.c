
// ------------------------------ includes ------------------------------

#include <unistd.h>
#include <utils/base64.h>
#include "Client.h"
#include "../communicationProtocol/include/MessageProtocol.h"

// -------------------------- macros -------------------------

#define CLIENT_CONST_FILE_NAME "test"

// ------------------------------ private functions -----------------------------

static ReturnValue clientGetFile(char *reply)
{
    CHECK_NULL_RETURN_ERROR(reply);
    // TODO create a file instead
    printf("Client is receiving a file:\n");

    // first msg is always the file's title
    char *fileTitle = messageGetContents(reply);
    CHECK_NULL_RETURN_ERROR(fileTitle);
    printf("%s\n", fileTitle);

    FILE *pFile = fopen(fileTitle, FILE_WRITE_BINARY_MODE);
    if (pFile == NULL)
    {
        PRINT_ERROR_MSG_AND_FUNCTION_NAME("clientGetFile", "Couldn't open the file");
        return PROJECT_ERROR;
    }

    char *followingReply = MPClientReceive();
    CHECK_NULL_RETURN_ERROR(followingReply);
    printf("Got the msg: %s\n", followingReply);

    char *contents = NULL;
    Command currentCommand = messageGetCommand(followingReply);
    unsigned int totalBytesToBeWritten = 0, totalBytesWritten = 0;
    while (currentCommand == GET_FILE)
    {
        contents = messageGetContents(followingReply);
        if (contents == NULL || strlen(contents) == 0)
        {
            printf("[CLIENT-FILE]: Error with contents...\n");
            free(followingReply);
            fclose(pFile);
            return PROJECT_ERROR;
        }

        char* decodedMsg = (char *) malloc(MAX_MSG_LENGTH);
        //TODO CHECK NON NULL

        totalBytesToBeWritten = Base64decode(decodedMsg, contents);
        free(followingReply);
        if (totalBytesToBeWritten == 0)
        {
            printf("[CLIENT-FILE]: Error with decoding...\n");
            free(decodedMsg);
            fclose(pFile);
            return PROJECT_ERROR;
        }

        totalBytesWritten = fwrite(decodedMsg, 1, totalBytesToBeWritten, pFile);
        free(decodedMsg);
        if (totalBytesWritten != totalBytesToBeWritten)
        {
            printf("[CLIENT-FILE]: Error with writing to file\n");
            fclose(pFile);
            return PROJECT_ERROR;
        }

        // get next msg and its command
        followingReply = MPClientReceive();
        currentCommand = messageGetCommand(followingReply);
    }

    fclose(pFile);

    if (followingReply == NULL)
    {
        PRINT_ERROR_MSG_AND_FUNCTION_NAME("clientGetFile", "Something went wrong with receiving the file");
        return PROJECT_ERROR;
    }

    return PROJECT_SUCCESS;
}


static ReturnValue handleReply(char *reply)
{
    if (!messageValidateFormat(reply))
        return PROJECT_ERROR;

    Command currentCommand = messageGetCommand(reply);

    // SHAH: there's nothing much I check atm
    if (currentCommand == GET_FILE)
        clientGetFile(reply);
    else
    {
        const char *pointerToMsg = messageGetContents(reply);
        printf("Client received reply: %s.\n", pointerToMsg);
    }
    return PROJECT_SUCCESS;
}

// ------------------------------ functions -----------------------------

ReturnValue clientInitialize(CommunicationMethodCode cMethod)
{
    return MPClientInitConnection(cMethod);
}

ReturnValue clientClose(ReturnValue result)
{
    bool errorFlag = (result == PROJECT_ERROR) ? true:false;
    return MPClientCloseConnection(errorFlag);
}

ReturnValue clientSendCommand(Command commandType, char *contents)
{
    CHECK_NULL_RETURN_ERROR(contents);
    char *msg = messageSet(CLIENT, commandType, contents);
    if (!messageValidateFormat(msg))
        return PROJECT_ERROR;

    // send the message and wait to handle its reply
    char *reply = MPClientSend(msg);
    ReturnValue result = handleReply(reply);

    free(msg);
    free(reply);
    return result;
}