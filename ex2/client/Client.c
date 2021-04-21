
// ------------------------------ includes ------------------------------

#include "Client.h"
#include "../communicationProtocol/include/MessageProtocol.h"

// -------------------------- macros -------------------------

#define CLIENT_CONST_FILE_NAME "pitz"

// ------------------------------ private functions -----------------------------

static ReturnValue clientGetFile(char *reply)
{
    // TODO create a file instead
    printf("Client is receiving a file:\n");

    // first msg is always the file's title
    char *fileTitle = messageGetContents(reply);
    FILE *pFile = fopen(fileTitle, FILE_WRITE_BINARY_MODE);
    if (pFile == NULL)
    {
        PRINT_ERROR_MSG_AND_FUNCTION_NAME("clientGetFile", "Couldn't open the file");
        return PROJECT_ERROR;
    }

    char *followingReply = MPClientReceive();
    char *contents = NULL;
    Command currentCommand = messageGetCommand(followingReply);
    unsigned int counter = 0, totalBytesToBeWritten = 0, totalBytesWritten = 0;
    while (currentCommand == GET_FILE)
    {
        counter++;

        // TODO write to a file instead
        printf("[%d]: %s\n", counter, followingReply);
        contents = messageGetContents(followingReply);
        if (contents == NULL)
        {
            fclose(pFile);
            return PROJECT_ERROR;
        }
        totalBytesToBeWritten = (unsigned int)strlen(contents);
        totalBytesWritten = fwrite(contents, 1, totalBytesToBeWritten, pFile);
        free(followingReply);
        if (totalBytesWritten != totalBytesToBeWritten)
        {
            fclose(pFile);
            return PROJECT_ERROR;
        }

        // get next msg and its command
        followingReply = MPClientReceive();
        currentCommand = messageGetCommand(followingReply);
    }

    fclose(pFile);

    // TODO: remove this
    printf("Client finished receiving a file of %d msgs with msg:\n", counter);
    printf("%s\n", followingReply);
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

ReturnValue clientClose()
{
    return MPClientCloseConnection();
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