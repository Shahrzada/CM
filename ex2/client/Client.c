
// ------------------------------ includes ------------------------------

#include "Client.h"
#include "MessageProtocol.h"
#include "base64.h"

// ------------------------------ private functions -----------------------------

static ReturnValue clientHandleFileDataStream(FILE *pFile)
{
    CHECK_NULL_RETURN_ERROR(pFile);

    char *currentFileData = NULL, *encodedData = NULL;
    void *decodedData = NULL;
    Command currentCommand = EMPTY_COMMAND;
    unsigned int totalBytesToBeWritten = 0, totalBytesWritten = 0;
    ReturnValue result = PROJECT_ERROR;

    printf("Incoming file data:\n");

    while (true)
    {
        // get the msg
        currentFileData = MPClientReceive();
        CHECK_NULL_RETURN_ERROR(currentFileData);

        // make sure we are still getting file data
        currentCommand = messageGetCommand(currentFileData);
        if (currentCommand != GET_FILE)
            break;

        // extract the encoded data
        encodedData = messageGetContents(currentFileData);
        CHECK_NULL_GOTO_CLEANUP(encodedData);

        // decode the data
        decodedData = (char *) malloc(sizeof(char) * MAX_MSG_LENGTH);
        CHECK_NULL_GOTO_CLEANUP(decodedData);
        totalBytesToBeWritten = Base64decode(decodedData, encodedData);
        CHECK_ZERO_GOTO_CLEANUP(totalBytesToBeWritten);

        // Write data to local file
        totalBytesWritten = fwrite(decodedData, 1, totalBytesToBeWritten, pFile);
        if (totalBytesWritten != totalBytesToBeWritten)
            goto cleanup;

        // free for other possible incoming data
        free(currentFileData);
        currentFileData = NULL;
        free(decodedData);
        decodedData = NULL;
    }

    result = PROJECT_SUCCESS;

cleanup:
    free(currentFileData);
    free(decodedData);
    return result;
}

static ReturnValue clientGetFile(char *reply)
{
    CHECK_NULL_RETURN_ERROR(reply);

    // first msg is always the file's title
    char *fileTitle = messageGetContents(reply);
    CHECK_NULL_RETURN_ERROR(fileTitle);
    printf("Client is receiving a file with title: %s\n", fileTitle);

    // Create and open the file
    FILE *pFile = fopen(fileTitle, FILE_WRITE_BINARY_MODE);
    if (pFile == NULL)
    {
        PRINT_ERROR_MSG_AND_FUNCTION_NAME("clientGetFile", "Couldn't open the file");
        return PROJECT_ERROR;
    }

    // Write the incoming file data
    ReturnValue result = clientHandleFileDataStream(pFile);

    fclose(pFile);
    return result;
}

static ReturnValue handleReply(char *reply)
{
    if (!messageValidateFormat(reply))
        return PROJECT_ERROR;

    // TODO we do not validate the reply atm, only handle file downloading
    Command currentCommand = messageGetCommand(reply);
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
    return MPClientCloseConnection(result);
}

ReturnValue clientSendCommand(Command commandType, char *contents)
{
    CHECK_NULL_RETURN_ERROR(contents);
    ReturnValue result = PROJECT_ERROR;

    char *msg = messageSet(CLIENT, commandType, contents);
    if (!messageValidateFormat(msg))
        return PROJECT_ERROR;

    // send the message and wait to handle its reply
    char *reply = MPClientSend(msg);
    CHECK_NULL_GOTO_CLEANUP(reply);
    result = handleReply(reply);

cleanup:
    free(msg);
    free(reply);
    return result;
}