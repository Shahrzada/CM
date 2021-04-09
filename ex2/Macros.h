#pragma once

// -------------------------- const definitions -------------------------

/* Communication methods */
typedef enum {
    SOCKET_METHOD = 0,
    FILE_METHOD,
} CommunicationMethodCode;

/* Return values */
typedef enum {
    SUCCESS = 0,
    ERROR = -999,
} ReturnValue;

/* Command types */
typedef enum {
    EMPTY_COMMAND = -1,
    READ = 0,
    WRITE,
    ABORT,
} Command;

/* Sender types */
typedef enum {
    EMPTY_SENDER = -1,
    SERVER = 0,
    CLIENT,
} Sender;


// -------------------------- macros -------------------------

#define NULL_CHAR '\0'
#define QUOTATION_MARK_CHAR_ASCII_DEC_VALUE 34
#define APOSTROPHE_CHAR_ASCII_DEC_VALUE 39
#define EOL_CHAR_ASCII_DEC_VALUE 10
#define NEWLINE_CHAR '\n'

#define NULL_CHAR_SIZE 1
#define NEWLINE_CHAR_SIZE 1

#define FILE_READ_MODE "r"
#define FILE_WRITE_UPDATE_MODE "w+"
#define FILE_APPEND_MODE "a"

// -------------------------- macro functions -------------------------

#define CHECK_NULL_RETURN_NULL(pointer) do {if ((pointer) == NULL) return NULL;} while(0)
#define CHECK_NULL_RETURN_ERROR(pointer) do {if ((pointer) == NULL) return ERROR;} while(0)
#define CHECK_NULL_RETURN(pointer) do {if ((pointer) == NULL) return;} while(0)
#define CHECK_NULL_RETURN_ZERO(pointer) do {if ((pointer) == NULL) return 0;} while(0)
#define CHECK_NULL_GOTO_CLEANUP(pointer) do {if ((pointer) == NULL) goto cleanup;} while(0)

#define CHECK_NON_ZERO_RETURN_ERROR(number) do {if ((number) != 0) return ERROR;} while(0)
#define CHECK_NON_ZERO_RETURN_NULL(number) do {if ((number) != 0) return NULL;} while(0)
#define CHECK_NON_ZERO_GOTO_CLEANUP(number) do {if ((number) != 0) goto cleanup;} while(0)

#define CHECK_ERROR_RETURN_ERROR(number) do {if ((number) == ERROR) return ERROR;} while(0)
#define CHECK_ERROR_RETURN_NULL(number) do {if ((number) == ERROR) return NULL;} while(0)
#define CHECK_ERROR_GOTO_CLEANUP(number) do {if ((number) == ERROR) goto cleanup;} while(0)


