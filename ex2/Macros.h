#pragma once

// -------------------------- const definitions -------------------------

/* Communication methods */
typedef enum {
    SOCKET_METHOD = 0,
    FILE_METHOD,
} CommunicationMethodCode;

/* Return values */
typedef enum {
    PROJECT_SUCCESS = 0,
    PROJECT_ERROR = -999,
} ReturnValue;

/* Command types */
typedef enum {
    EMPTY_COMMAND = -1,
    READ = 0,
    WRITE,
    GET_FILE,
    ABORT,
    REPLY,
} Command;

/* Sender types */
typedef enum {
    EMPTY_SENDER = -1,
    SERVER = 0,
    CLIENT,
} Sender;


// -------------------------- macros -------------------------

#define EOL_CHAR_ASCII_DEC_VALUE 10
#define NULL_CHAR '\0'
#define NEWLINE_CHAR '\n'
#define EOL_CHAR '\n'
#define EOL_STRING "\n"
#define COMMA_CHAR ','
#define ZERO_CHAR '0'

#define NULL_CHAR_SIZE 1
#define EOL_CHAR_SIZE 1

#define FILE_READ_MODE "r"
#define FILE_READ_BINARY_MODE "rb"
#define FILE_WRITE_BINARY_MODE "wb"
#define FILE_WRITE_UPDATE_MODE "w+"
#define FILE_APPEND_MODE "a"
#define FILE_APPEND_AND_READ_MODE "a+"

#define MAX_MSG_LENGTH 513

// -------------------------- macro functions -------------------------

#define CHECK_NULL_RETURN_NULL(pointer) do {if ((pointer) == NULL) return NULL;} while(0)
#define CHECK_NULL_RETURN_ERROR(pointer) do {if ((pointer) == NULL) return PROJECT_ERROR;} while(0)
#define CHECK_NULL_RETURN_FALSE(pointer) do {if ((pointer) == NULL) return false;} while(0)
#define CHECK_NULL_RETURN(pointer) do {if ((pointer) == NULL) return;} while(0)
#define CHECK_NULL_RETURN_ZERO(pointer) do {if ((pointer) == NULL) return 0;} while(0)
#define CHECK_NULL_GOTO_CLEANUP(pointer) do {if ((pointer) == NULL) goto cleanup;} while(0)

#define CHECK_NON_ZERO_RETURN_ERROR(number) do {if ((number) != 0) return PROJECT_ERROR;} while(0)
#define CHECK_NON_ZERO_RETURN_NULL(number) do {if ((number) != 0) return NULL;} while(0)
#define CHECK_NON_ZERO_GOTO_CLEANUP(number) do {if ((number) != 0) goto cleanup;} while(0)

#define CHECK_ZERO_RETURN_NULL(number) do {if ((number) == 0) return NULL;} while(0)

#define CHECK_ERROR_RETURN_ERROR(number) do {if ((number) == PROJECT_ERROR) return PROJECT_ERROR;} while(0)
#define CHECK_ERROR_RETURN_NULL(number) do {if ((number) == PROJECT_ERROR) return NULL;} while(0)
#define CHECK_ERROR_RETURN(number) do {if ((number) == PROJECT_ERROR) return;} while(0)
#define CHECK_ERROR_GOTO_CLEANUP(number) do {if ((number) == PROJECT_ERROR) goto cleanup;} while(0)

#define CHECK_ERROR_PRINT_AND_RETURN_ERROR(result, function) do { \
           if ((result) == PROJECT_ERROR) {                                 \
                printf("Error with %s.\n", (function));             \
                 return PROJECT_ERROR; }                                   \
           } while(0)

#define CHECK_NULL_PRINT_AND_RETURN_ERROR(result, function) do { \
           if ((result) == NULL) {                                 \
                printf("Error with %s.\n", (function));             \
                 return PROJECT_ERROR; }                                   \
           } while(0)

#define PRINT_ERROR_MSG_AND_FUNCTION_NAME(functionName, msg) do { \
           printf("[PROJECT_ERROR]: In %s - %s.\n", (functionName),(msg)); \
           } while(0)

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
