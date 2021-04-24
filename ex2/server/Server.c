
// ------------------------------ includes ------------------------------

#include <unistd.h>
#include <utils/base64.h>

#include "server/Server.h"
#include "communicationProtocol/include/MessageProtocol.h"

// -------------------------- macros -------------------------

#define GET_FILE_PATH "C:\\Users\\ADMIN\\CLionProjects\\bebi\\ex2\\test.png"
#define GET_FILE_TITLE "test.png"

// ------------------------------ private functions -----------------------------

static char *serverLoadFileIntoMsgFormat(FILE *pFile)
{
    CHECK_NULL_RETURN_NULL(pFile);
    char *encodedMsg = (char *) malloc(MAX_MSG_LENGTH);
    CHECK_NULL_RETURN_NULL(encodedMsg);

    // get max bytes of file
    int maxMsgLength = (MAX_MSG_LENGTH/2) - MSG_FORMAT_LENGTH + NULL_CHAR_SIZE;
    char buf[maxMsgLength];
    int ch = 0;
    int counter  = 0;
    while (counter < maxMsgLength)
    {
        ch = fgetc(pFile);
        if (ch == EOF)
            break;
        buf[counter] = (char)ch;
        counter++;
    }
    if (counter == 0)
        return NULL;
    if (Base64encode(encodedMsg, buf, counter) == 0)
    {
        free(encodedMsg);
        printf("Error with encoding file...");
        return NULL;
    }

    // create the msg format for the contents
    char *msg = messageSet(SERVER, GET_FILE, encodedMsg);
    return msg;
}

static void serverClose(bool errorExitFlag)
{
    MPServerCloseConnection();

    if (errorExitFlag)
        printf("An error occurred, closing...");
    else
        printf("Server got ABORT command, closing...");

    exit(PROJECT_SUCCESS);
}

static void serverRead()
{
    // Assuming we did some reading from somewhere
    MPServerSendSuccessOrFailure(PROJECT_SUCCESS);
}

static void serverWrite(const char *msg)
{
    CHECK_NULL_RETURN(msg);

    // Assuming server wrote content to somewhere, and succeeded.
    MPServerSendSuccessOrFailure(PROJECT_SUCCESS);
}

static ReturnValue sendFileTitle(char *msg)
{
    CHECK_NULL_RETURN_ERROR(msg);

    char *fileTitleMsg = messageSet(SERVER, GET_FILE, GET_FILE_TITLE);
    CHECK_NULL_RETURN_ERROR(fileTitleMsg);

    ReturnValue result = MPServerSend(fileTitleMsg);
    free(fileTitleMsg);
    return result;
}

static void serverSendFile(char *msg)
{
    CHECK_NULL_RETURN(msg);
//    char *filePath = messageGetContents(msg);
//    CHECK_NULL_RETURN(filePath);

    // const location atm
    char *filePath = GET_FILE_PATH;
    char *fileMsg = NULL;
    FILE *pFile = NULL;
    ReturnValue result = PROJECT_ERROR;

    // open the file to be sent
    pFile = fopen(filePath, FILE_READ_BINARY_MODE);
    CHECK_NULL_GOTO_CLEANUP(pFile);

    // first msg is always the file's title
    result = sendFileTitle(msg);
    CHECK_ERROR_GOTO_CLEANUP(result);

    // then, send the file itself using multiple msgs (if needed)
    while (true)
    {
        fileMsg = serverLoadFileIntoMsgFormat(pFile);
        CHECK_NULL_GOTO_CLEANUP(fileMsg);
        result = MPServerSend(fileMsg);
        CHECK_ERROR_GOTO_CLEANUP(result);
        free(fileMsg);
    }

cleanup:
    free(fileMsg);
    fclose(pFile);
    MPServerSendSuccessOrFailure(result);
}

static void serverAbort(char *msg)
{
    int errorExitFlag = true;
    if (msg != NULL)
    {
        free(msg);
        errorExitFlag = false;
    }
    MPServerSendSuccessOrFailure(PROJECT_SUCCESS);
    serverClose(errorExitFlag);
}

static void serverHandleMessage(char *msg)
{
    if (!messageValidateFormat(msg))
        PRINT_ERROR_MSG_AND_FUNCTION_NAME("serverHandleMessage", "Bad msg format");

    Command currentCommand = messageGetCommand(msg);

    switch (currentCommand) {
        case READ: serverRead(); break;
        case WRITE:  serverWrite(msg); break;
        case GET_FILE: serverSendFile(msg); break;
        case ABORT:  serverAbort(msg); break;
        default: PRINT_ERROR_MSG_AND_FUNCTION_NAME("serverHandleMessage", "Bad COMMAND"); break;
    }
}

// ------------------------------ functions -----------------------------

ReturnValue serverInitialize(CommunicationMethodCode cMethod)
{
    return MPServerInitConnection(cMethod);
}

_Noreturn void serverListen()
{
    while (true)
    {
        char *incomingMsg = MPServerListen();
        if (!messageValidateFormat(incomingMsg)) {
            PRINT_ERROR_MSG_AND_FUNCTION_NAME("serverListen", "Bad msg format");

            if (incomingMsg == NULL)
                serverClose(true);

            printf("Server got: %s\n", incomingMsg);
        }

        serverHandleMessage(incomingMsg);
        free(incomingMsg);
        sleep(1);
    }
}
