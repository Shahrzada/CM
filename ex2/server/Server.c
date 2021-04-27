
// ------------------------------ includes ------------------------------

#include <unistd.h>

#include "server/Server.h"
#include "MessageProtocol.h"
#include "base64.h"

// -------------------------- macros -------------------------

// TODO REMOVE THIS
#define GET_FILE_PATH "C://Users//ADMIN//CLionProjects//bebi//ex2//test.png"
#define GET_FILE_TITLE "test.png"

// ------------------------------ private functions
// -----------------------------

static int serverLoadFileIntoBuffer(char *buf, int bufLength, FILE *pFile) {
  CHECK_NULL_RETURN_ZERO(buf);
  CHECK_NULL_RETURN_ZERO(pFile);

  int ch = 0, counter = 0;
  while (counter < bufLength) {
    ch = fgetc(pFile);
    if (ch == EOF)
      break;
    buf[counter++] = (char)ch;
  }
  return counter;
}

static char *serverLoadFileIntoMsgFormat(FILE *pFile) {
  CHECK_NULL_RETURN_NULL(pFile);
  char *encodedMsg = (char *)malloc(MAX_MSG_LENGTH);
  CHECK_NULL_RETURN_NULL(encodedMsg);
  char *msg = NULL;

  // get max bytes of file, notice we read less than MAX_MSG_LENGTH due to
  // encoding
  int maxMsgLength = MAX_FILE_MSG_LENGTH - MSG_FORMAT_LENGTH + NULL_CHAR_SIZE;
  char buf[maxMsgLength];
  int counter = serverLoadFileIntoBuffer(buf, maxMsgLength, pFile);

  // reached EOF
  if (counter == 0)
    goto cleanup;

  // Encode le msg
  if (Base64encode(encodedMsg, buf, counter) == 0) {
    printf("Error with encoding file...");
    goto cleanup;
  }

  // create the msg format for the encoded contents
  msg = messageSet(SERVER, GET_FILE, encodedMsg);

cleanup:
  free(encodedMsg);
  return msg;
}

static void serverClose(bool errorExitFlag) {
  MPServerCloseConnection();

  if (errorExitFlag)
    printf("An error occurred, closing...");
  else
    printf("Server got ABORT command, closing...");

  exit(PROJECT_SUCCESS);
}

static void serverRead() {
  // Assuming we did some reading from somewhere
  MPServerSendSuccessOrFailure(PROJECT_SUCCESS);
}

static void serverWrite(const char *msg) {
  CHECK_NULL_RETURN(msg);

  // Assuming server wrote content to somewhere, and succeeded.
  MPServerSendSuccessOrFailure(PROJECT_SUCCESS);
}

static ReturnValue sendFileTitle(const char *msg) {
  CHECK_NULL_RETURN_ERROR(msg);
  // TODO extract file title from msg, we now use a const file

  char *fileTitleMsg = messageSet(SERVER, GET_FILE, GET_FILE_TITLE);
  CHECK_NULL_RETURN_ERROR(fileTitleMsg);

  ReturnValue result = MPServerSend(fileTitleMsg);
  free(fileTitleMsg);
  return result;
}

static void serverSendFile(char *msg) {
  CHECK_NULL_RETURN(msg);
  // TODO we use a const file path atm, we should extract it from the msg
  char *filePath = GET_FILE_PATH;
  char *fileMsg = NULL;
  FILE *pFile = NULL;
  ReturnValue result = PROJECT_ERROR;

  // open the file to be sent
  pFile = fopen(filePath, FILE_READ_BINARY_MODE);
  CHECK_NULL_GOTO_CLEANUP(pFile);

  // first msg to send is always the file's title
  result = sendFileTitle(msg);
  CHECK_ERROR_GOTO_CLEANUP(result);

  // then, send the file itself (using multiple msgs, if needed)
  while (true) {
    fileMsg = serverLoadFileIntoMsgFormat(pFile);
    CHECK_NULL_GOTO_CLEANUP(fileMsg);

    result = MPServerSend(fileMsg);
    CHECK_ERROR_GOTO_CLEANUP(result);

    free(fileMsg);
    fileMsg = NULL;
  }

cleanup:
  fclose(pFile);
  free(fileMsg);
  MPServerSendSuccessOrFailure(result);
}

static void serverAbort(char *msg) {
  bool errorExitFlag = (msg == NULL);
  ReturnValue result = (errorExitFlag) ? PROJECT_ERROR : PROJECT_SUCCESS;

  free(msg);
  MPServerSendSuccessOrFailure(result);
  serverClose(errorExitFlag);
}

static void serverHandleMessage(char *msg) {
  if (!messageValidateFormat(msg)) {
    PRINT_ERROR_MSG_AND_FUNCTION_NAME("serverHandleMessage", "Bad msg format");
    return;
  }

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
    PRINT_ERROR_MSG_AND_FUNCTION_NAME("serverHandleMessage", "Bad COMMAND");
    break;
  }
}

// ------------------------------ functions -----------------------------

ReturnValue serverInitialize(CommunicationMethodCode cMethod) {
  return MPServerInitConnection(cMethod);
}

_Noreturn void serverListen() {
  while (true) {
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
