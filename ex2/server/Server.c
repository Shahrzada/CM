
// ------------------------------ includes ------------------------------

#include <unistd.h>
#include <config.h>
#include <libgen.h>

#include "Server.h"
#include "MessageProtocol.h"
#include "base64.h"

// ------------------------------ private functions -----------------------------

static int serverLoadFileIntoBuffer(char *buf, int bufLength, FILE *pFile) {
    CHECK_NULL_RETURN_ZERO(buf);
    CHECK_NULL_RETURN_ZERO(pFile);

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
    CHECK_NULL_RETURN_NULL(pFile);
    Message *msg = (Message *) malloc(sizeof(Message));
    CHECK_NULL_RETURN_NULL(msg);

    // Get max bytes of file (notice we read less than MAX_MSG_LENGTH due to encoding)
    int maxMsgLength = MAX_FILE_MSG_LENGTH - MSG_FORMAT_LENGTH + NULL_CHAR_SIZE;
    char buffer[maxMsgLength];
    int counter = serverLoadFileIntoBuffer(buffer, maxMsgLength, pFile);
    // CR: comments like this are more hurtful than helpful, good code requires little to no comments.
    //     the one above can stay, the one below should disappear
    // Reached EOF
    if (counter == 0)
        goto cleanup;

    // Create the msg format for the file data
    msg = messageSet(SERVER, GET_FILE, counter, buffer);
    CHECK_NULL_GOTO_CLEANUP(msg);

    return msg;

    cleanup:
    free(msg);
    return NULL;
}

_Noreturn static void serverClose(bool errorExitFlag) {
    MPServerCloseConnection();

    if (errorExitFlag)
        printf("\nAn error occurred, closing...\n");
    else
        printf("\nServer got ABORT command, closing...\n");

    exit(PROJECT_SUCCESS);
}
// CR: why is this void? what if the server fails.
//     you have way too many function that aren't reporting if they succussed or not

static void serverRead() {
    // CR: what? what is this comment?
    // Assuming we did some reading from somewhere
    MPServerSendSuccessOrFailure(PROJECT_SUCCESS);
}

static void serverWrite(Message *msg) {
    CHECK_NULL_RETURN(msg);

    // Assuming server wrote content to somewhere, and succeeded.
    MPServerSendSuccessOrFailure(PROJECT_SUCCESS);
}

static ReturnValue sendFileTitle(Message *msg) {
    if (!messageValidateFormat(msg))
        PRINT_ERROR_WITH_FUNCTION_AND_RETURN_ERROR("sendFileTitle", "Bad msg format");

    // Extract file title and extension from the msg
    char *fileTitle = basename(msg->contents);
    CHECK_NULL_RETURN_ERROR(fileTitle);
    unsigned int fileTitleLength = strnlen(fileTitle, MAX_JSON_VALUE_LENGTH);
    CHECK_ZERO_RETURN_ERROR(fileTitleLength);

    // Create a msg out of the data
    Message *fileTitleMsg = messageSet(SERVER, GET_FILE, fileTitleLength, fileTitle);
    CHECK_NULL_RETURN_ERROR(fileTitleMsg);

    ReturnValue result = MPServerSend(fileTitleMsg);
    free(fileTitleMsg);
    CHECK_ERROR_RETURN_ERROR(result);

    // Wait for client reply
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
    CHECK_NULL_RETURN_ERROR(pFile);
    Message *fileMsg = NULL;
    ReturnValue result = PROJECT_ERROR;

    while (true)
    {
        fileMsg = serverLoadFileIntoMsgFormat(pFile);
        if (fileMsg == NULL)
        {
            result = PROJECT_SUCCESS;
            goto cleanup; 
            // CR: goto in happy flow is a bit wired, this shouldve been a break;
            //     or better yet change the condition
        }

        result = MPServerSend(fileMsg);
        CHECK_ERROR_GOTO_CLEANUP(result);

        messageFree(fileMsg);
        fileMsg = NULL;

        // Wait for client reply
        Message *clientReply = MPServerListen();
        MSG_CHECK_VALID_GOTO_CLEANUP(clientReply);
        Command replyCommand = clientReply->command;
        messageFree(clientReply);
        if (replyCommand != GET_FILE)
        {
            result = PROJECT_ERROR;
            goto cleanup;
        }
    }

    cleanup:
    messageFree(fileMsg);
    return result;
}

static void serverSendFile(Message *msg)
{
    if (!messageValidateFormat(msg))
        PRINT_ERROR_WITH_FUNCTION_AND_RETURN("serverSendFile", "Bad msg format");
    char *filePath = msg->contents;

    FILE *pFile = NULL;
    ReturnValue result = PROJECT_ERROR;

    // Open the file to be sent
    pFile = fopen(filePath, FILE_READ_BINARY_MODE);
    CHECK_NULL_GOTO_CLEANUP(pFile);

    // First msg to send is always the file's title
    result = sendFileTitle(msg);
    CHECK_ERROR_GOTO_CLEANUP(result);

    // Then, send the file itself (using multiple msgs, if needed)
    result = serverStreamFile(pFile);

    cleanup:
    fclose(pFile);
    MPServerSendSuccessOrFailure(result);
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
    switch (currentCommand) {
        case READ:
            serverRead();
            break;
        case WRITE:
            serverWrite(msg);
            break;
        case GET_FILE:
            serverSendFile(msg);
            break;
        case ABORT:
            serverAbort(msg);
            break;
        default:
            PRINT_ERROR_WITH_FUNCTION("serverHandleMessage", "Bad COMMAND");
            break;
    }
}

static void serverHandleError(Message *msg)
{
    printf("\n[ERROR]: Received bad msg:\n");
    if (msg == NULL)
    {
        printf("Got null msg.\n");
        serverClose(true);
    }
    PRINT_MSG(msg);
}

// ------------------------------ functions -----------------------------

ReturnValue serverInitialize(CommunicationMethodCode cMethod) {
    return MPServerInitConnection(cMethod);
}

_Noreturn void serverListen() {
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
