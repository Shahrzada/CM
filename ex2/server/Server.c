
// ------------------------------ includes ------------------------------

#include "Server.h"
#include "MessageProtocol.h"
#include "base64.h"

#include <unistd.h>
#include <config.h>
#include <libgen.h>

// ------------------------------ private functions -----------------------------

static int serverLoadFileIntoBuffer(char *buf, int bufLength, FILE *pFile) {
    CHECK_NULL_RETURN_VALUE(buf, 0);
    CHECK_NULL_RETURN_VALUE(pFile, 0);

    int ch = 0, counter = 0;
    while (counter < bufLength) {
        ch = fgetc(pFile);
        if (ch == EOF)
            break;
        buf[counter++] = (char) ch;
    }
    return counter;
}

static Message *serverLoadFileIntoMsgFormat(FILE *pFile) {
    CHECK_NULL_RETURN_VALUE(pFile, NULL);
    Message *msg = (Message *) malloc(sizeof(Message));
    CHECK_NULL_RETURN_VALUE(msg, NULL);

    int maxMsgLength = MAX_FILE_MSG_LENGTH - MSG_FORMAT_LENGTH + NULL_CHAR_SIZE;
    char buffer[maxMsgLength];
    int counter = serverLoadFileIntoBuffer(buffer, maxMsgLength, pFile);

    if (counter == 0)
        goto cleanup;

    msg = messageSet(SERVER, GET_FILE, counter, buffer);
    CHECK_NULL_GOTO_CLEANUP(msg);

    return msg;

    cleanup:
    free(msg);
    return NULL;
}

_Noreturn static void serverClose(bool errorExitFlag)
{
    MPServerCloseConnection();
    (errorExitFlag) ? printf("\nAn error occurred, closing...\n") : printf("\nServer got ABORT command, closing...\n");
    exit(PROJECT_SUCCESS);
}

static ReturnValue serverRead()
{
    return MPServerSendSuccessOrFailure(PROJECT_SUCCESS);
}

static ReturnValue serverWrite(Message *msg)
{
    CHECK_NULL_RETURN_VALUE(msg, PROJECT_ERROR);
    return MPServerSendSuccessOrFailure(PROJECT_SUCCESS);
}

static ReturnValue sendFileTitle(Message *msg)
{
    if (!messageValidateFormat(msg))
        PRINT_ERROR_WITH_FUNCTION_AND_RETURN_ERROR("sendFileTitle", "Bad msg format");

    char *fileTitle = basename(msg->contents);
    CHECK_NULL_RETURN_VALUE(fileTitle, PROJECT_ERROR);
    unsigned int fileTitleLength = strnlen(fileTitle, MAX_JSON_VALUE_LENGTH);
    CHECK_ZERO_RETURN_VALUE(fileTitleLength, PROJECT_ERROR);

    Message *fileTitleMsg = messageSet(SERVER, GET_FILE, fileTitleLength, fileTitle);
    CHECK_NULL_RETURN_VALUE(fileTitleMsg, PROJECT_ERROR);

    ReturnValue result = MPServerSend(fileTitleMsg);
    free(fileTitleMsg);
    CHECK_ERROR_RETURN_VALUE(result, PROJECT_ERROR);

    Message *clientReply = MPServerListen();
    MSG_CHECK_VALID_GOTO_CLEANUP(clientReply);
    Command replyCommand = clientReply->command;
    messageFree(clientReply);
    if (replyCommand != GET_FILE)
        goto cleanup;

    return result;

    cleanup:
    messageFree(clientReply);
    return PROJECT_ERROR;
}

static ReturnValue serverStreamFile(FILE *pFile)
{
    CHECK_NULL_RETURN_VALUE(pFile, PROJECT_ERROR);
    Message *fileMsg = NULL;
    ReturnValue result = PROJECT_ERROR;

    while ((fileMsg = serverLoadFileIntoMsgFormat(pFile)) != NULL)
    {
        result = MPServerSend(fileMsg);
        messageFree(fileMsg);
        fileMsg = NULL;
        CHECK_ERROR_GOTO_CLEANUP(result);

        Message *clientReply = MPServerListen();
        MSG_CHECK_VALID_GOTO_CLEANUP(clientReply);

        Command replyCommand = clientReply->command;
        messageFree(clientReply);
        if (replyCommand != GET_FILE)
            goto cleanup;
    }
    return PROJECT_SUCCESS;

    cleanup:
    return PROJECT_ERROR;
}

static ReturnValue serverSendFile(Message *msg)
{
    if (!messageValidateFormat(msg))
        PRINT_ERROR_WITH_FUNCTION_AND_RETURN_ERROR("serverSendFile", "Bad msg format");
    char *filePath = msg->contents;

    FILE *pFile = NULL;
    ReturnValue result = PROJECT_ERROR;

    pFile = fopen(filePath, FILE_READ_BINARY_MODE);
    CHECK_NULL_GOTO_CLEANUP(pFile);

    // First msg to send is always the file's title
    result = sendFileTitle(msg);
    CHECK_ERROR_GOTO_CLEANUP(result);

    result = serverStreamFile(pFile);

    cleanup:
    fclose(pFile);
    MPServerSendSuccessOrFailure(result);
    return result;
}

static void serverAbort(Message *msg)
{
    bool errorExitFlag = (msg == NULL);
    messageFree(msg);

    MPServerSendSuccessOrFailure((errorExitFlag) ? PROJECT_ERROR : PROJECT_SUCCESS);
    serverClose(errorExitFlag);
}

static void serverHandleMessage(Message *msg)
{
    if (!messageValidateFormat(msg))
        PRINT_ERROR_WITH_FUNCTION_AND_RETURN("serverHandleMessage", "Bad msg format");

    Command currentCommand = messageGetCommand(msg);
    ReturnValue result = PROJECT_ERROR;
    switch (currentCommand)
    {
        case READ:
            result = serverRead();
            break;
        case WRITE:
            result = serverWrite(msg);
            break;
        case GET_FILE:
            result = serverSendFile(msg);
            break;
        case ABORT:
            serverAbort(msg);
            break;
        default:
            PRINT_ERROR_WITH_FUNCTION("serverHandleMessage", "Bad COMMAND");
            break;
    }
    if (result == PROJECT_ERROR)
        serverClose(true);
}

static void serverHandleError(Message *msg)
{
    printf("\n[SERVER ERROR]: Received bad msg:\n");
    if (msg == NULL)
    {
        printf("Got null msg.\n");
        serverClose(true);
    }
    PRINT_MSG(msg);
}

// ------------------------------ functions -----------------------------

ReturnValue serverInitialize(CommunicationMethodCode cMethod)
{
    printf("\nInitializing server...\n");
    return MPServerInitConnection(cMethod);
}

_Noreturn void serverListen()
{
    printf("Server is listening...\n");
    while (true)
    {
        Message *msg = MPServerListen();
        if (!messageValidateFormat(msg))
            serverHandleError(msg);

        serverHandleMessage(msg);
        messageFree(msg);
        sleep(1);
    }
}
