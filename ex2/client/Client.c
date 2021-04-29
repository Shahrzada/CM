
// ------------------------------ includes ------------------------------

#include "Client.h"
#include "MessageProtocol.h"
#include "utils/include/base64.h"

// ------------------------------ private functions -----------------------------

static ReturnValue clientHandleFileDataStream(FILE *pFile)
{
    CHECK_NULL_RETURN_ERROR(pFile);

    Message *currentFileDataMsg = NULL;
    void *fileData = NULL;
    unsigned int totalBytesToBeWritten = 0, totalBytesWritten = 0;
    ReturnValue result = PROJECT_ERROR;

    printf("Incoming file data:\n");

    while (true)
    {
        // get the msg
        currentFileDataMsg = MPClientReceive();
        MSG_CHECK_VALID_GOTO_CLEANUP(currentFileDataMsg);

        // make sure we are still getting file data
        if (messageGetCommand(currentFileDataMsg) != GET_FILE)
            break;

        // Write data to local file
        fileData = messageGetContents(currentFileDataMsg);
        totalBytesToBeWritten = messageGetContentsLength(currentFileDataMsg);
        if (totalBytesToBeWritten == 0 || fileData == NULL)
            goto cleanup;

        totalBytesWritten = fwrite(fileData, 1, totalBytesToBeWritten, pFile);
        if (totalBytesWritten != totalBytesToBeWritten)
            goto cleanup;

        // free for other possible incoming data
        messageFree(currentFileDataMsg);
        currentFileDataMsg = NULL;
    }

    result = PROJECT_SUCCESS;

cleanup:
    free(currentFileDataMsg);
    return result;
}

static ReturnValue clientGetFile(Message *reply)
{
    if (!messageValidateFormat(reply))
        return PROJECT_ERROR;

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

static ReturnValue handleReply(Message *reply)
{
    if (!messageValidateFormat(reply))
        return PROJECT_ERROR;

    // we do not validate the reply atm, only handle file downloading
    if (messageGetCommand(reply) == GET_FILE)
        clientGetFile(reply);

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

ReturnValue clientSendCommand(Command commandType, unsigned int contentsLength, char *contents)
{
    CHECK_NULL_RETURN_ERROR(contents);
    ReturnValue result = PROJECT_ERROR;

    Message *msg = messageSet(CLIENT, commandType, contentsLength, contents);
    if (!messageValidateFormat(msg))
        return PROJECT_ERROR;

    // send the message and wait to handle its reply
    Message *reply = MPClientSend(msg);
    CHECK_NULL_GOTO_CLEANUP(reply);
    result = handleReply(reply);

cleanup:
    messageFree(msg);
    messageFree(reply);
    return result;
}