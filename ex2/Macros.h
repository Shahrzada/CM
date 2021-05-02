#pragma once

// -------------------------- includes -------------------------

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// -------------------------- const definitions -------------------------

/* Communication methods */
typedef enum {
    EMPTY_METHOD = -1,
    SOCKET_METHOD,
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
    READ,
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

enum ARGUMENTS{CONFIG_PATH = 1, TOTAL_ARGUMENTS};

// -------------------------- macros -------------------------

#define NULL_CHAR '\0'
#define EOL_CHAR '\n'

#define EMPTY_STRING ""

#define NULL_CHAR_SIZE 1

#define FILE_READ_MODE "r"
#define FILE_READ_BINARY_MODE "rb"
#define FILE_WRITE_BINARY_MODE "wb"
#define FILE_WRITE_UPDATE_MODE "w+"
#define FILE_APPEND_MODE "a"

#define MAX_MSG_LENGTH 1025
#define MAX_FILE_MSG_LENGTH 700

// -------------------------- macro functions -------------------------

#define CHECK_NULL_RETURN_VALUE(pointer, value) do {if ((pointer) == NULL) return (value);} while(0)
#define CHECK_NULL_RETURN(pointer) do {if ((pointer) == NULL) return;} while(0)
#define CHECK_NULL_GOTO_CLEANUP(pointer) do {if ((pointer) == NULL) goto cleanup;} while(0)

#define CHECK_ERROR_RETURN_VALUE(result, value) do {if ((result) == PROJECT_ERROR) return (value);} while(0)
#define CHECK_ERROR_RETURN(result) do {if ((result) == PROJECT_ERROR) return;} while(0)
#define CHECK_ERROR_GOTO_CLEANUP(result) do {if ((result) == PROJECT_ERROR) goto cleanup;} while(0)

#define CHECK_ZERO_RETURN_VALUE(number, value) do {if ((number) == 0) return (value);} while(0)
#define CHECK_ZERO_RETURN(number) do {if ((number) == 0) return;} while(0)
#define CHECK_ZERO_GOTO_CLEANUP(number) do {if ((number) == 0) goto cleanup;} while(0)


#define CHECK_ERROR_PRINT_AND_RETURN_ERROR(result, function) do { \
           if ((result) == PROJECT_ERROR) {                                 \
                printf("Error with %s.\n", (function));             \
                 return PROJECT_ERROR; }                                   \
           } while(0)

#define PRINT_ERROR_WITH_FUNCTION(functionName, msg) do { \
           printf("[PROJECT_ERROR]: In %s - %s.\n", (functionName),(msg)); \
           } while(0)

#define PRINT_ERROR_WITH_FUNCTION_AND_RETURN(functionName, msg) do { \
           printf("[PROJECT_ERROR]: In %s - %s.\n", (functionName),(msg)); \
           return; \
           } while(0)

#define PRINT_ERROR_WITH_FUNCTION_AND_RETURN_ERROR(functionName, msg) do { \
           printf("[PROJECT_ERROR]: In %s - %s.\n", (functionName),(msg)); \
           return PROJECT_ERROR; \
           } while(0)

#define PRINT_ERROR_WITH_FUNCTION_AND_RETURN_NULL(functionName, msg) do { \
           printf("[PROJECT_ERROR]: In %s - %s.\n", (functionName),(msg)); \
           return NULL; \
           } while(0)

#define PRINT_ERROR_WITH_FUNCTION_AND_RETURN_ZERO(functionName, msg) do { \
           printf("[PROJECT_ERROR]: In %s - %s.\n", (functionName),(msg)); \
           return 0; \
           } while(0)
