
// ------------------------------ includes ------------------------------

#include "Client.h"
#include "MessageProtocol.h"
#include "base64.h"

// ------------------------------ private functions -----------------------------

static ReturnValue clientHandleFileDataStream(FILE *pFile)
{
    CHECK_NULL_RETURN_VALUE(pFile, PROJECT_ERROR);

    Message *currentFileDataMsg = NULL;
    void *fileData = NULL;
    unsigned int totalBytesToBeWritten = 0, totalBytesWritten = 0;
    ReturnValue result = PROJECT_ERROR;

    Message *msgSuccess = messageSetSuccessOrFailure(CLIENT, GET_FILE, true);
    MSG_CHECK_VALID_GOTO_CLEANUP(msgSuccess);

    currentFileDataMsg = MPClientSend(msgSuccess);
    MSG_CHECK_VALID_GOTO_CLEANUP(currentFileDataMsg);

    while (messageGetCommand(currentFileDataMsg) == GET_FILE)
    {
        fileData = messageGetContents(currentFileDataMsg);
        CHECK_NULL_GOTO_CLEANUP(fileData);

        totalBytesToBeWritten = messageGetContentsLength(currentFileDataMsg);
        CHECK_ZERO_GOTO_CLEANUP(totalBytesToBeWritten);

        totalBytesWritten = fwrite(fileData, 1, totalBytesToBeWritten, pFile);
        if (totalBytesWritten != totalBytesToBeWritten)
            goto cleanup;

        messageFree(currentFileDataMsg);
        currentFileDataMsg = MPClientSend(msgSuccess);
        MSG_CHECK_VALID_GOTO_CLEANUP(currentFileDataMsg);
    }

    result = PROJECT_SUCCESS;

cleanup:
    messageFree(currentFileDataMsg);
    messageFree(msgSuccess);
    return result;
}

static ReturnValue clientGetFile(Message *reply)
{
    MSG_CHECK_VALID_RETURN_VALUE(reply, PROJECT_ERROR);

    char *fileTitle = messageGetContents(reply);
    CHECK_NULL_RETURN_VALUE(fileTitle, PROJECT_ERROR);

    FILE *pFile = fopen(fileTitle, FILE_WRITE_BINARY_MODE);
    if (pFile == NULL)
        PRINT_ERROR_WITH_FUNCTION_AND_RETURN_ERROR("clientGetFile", "Couldn't open the file");

    ReturnValue result = clientHandleFileDataStream(pFile);

    fclose(pFile);
    return result;
}

static ReturnValue handleReply(Message *reply)
{
    MSG_CHECK_VALID_RETURN_VALUE(reply, PROJECT_ERROR);

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
    CHECK_NULL_RETURN_VALUE(contents, PROJECT_ERROR);
    ReturnValue result = PROJECT_ERROR;

    Message *msg = messageSet(CLIENT, commandType, contentsLength, contents);
    MSG_CHECK_VALID_RETURN_VALUE(msg, PROJECT_ERROR);

    Message *reply = MPClientSend(msg);
    MSG_CHECK_VALID_GOTO_CLEANUP(reply);

    result = handleReply(reply);

cleanup:
    messageFree(msg);
    messageFree(reply);
    return result;
}
