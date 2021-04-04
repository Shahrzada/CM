#pragma once

#define CHECK_NULL_RETURN_NULL(pointer) do {if ((pointer) == NULL) return NULL;} while(0)
#define CHECK_NULL_RETURN(pointer) do {if ((pointer) == NULL) return;} while(0)
#define CHECK_NULL_RETURN_MYSTRING_ERROR(pointer) do {if ((pointer) == NULL) return MYSTRING_ERROR;} while(0)
#define CHECK_NULL_RETURN_VALUE(pointer, returnValue) do {if ((pointer) == NULL) return (returnValue);} while(0)
#define CHECK_NULL_GOTO_CLEANUP(pointer) do {if ((pointer) == NULL) goto cleanup;;} while(0)

#define CHECK_MYSTRING_ERROR_RETURN_MYSTRING_ERROR(number) do {if ((number) == MYSTRING_ERROR) return MYSTRING_ERROR;} while(0)
#define CHECK_MYSTRING_ERROR_GOTO_CLEANUP(number) do {if ((number) == MYSTRING_ERROR) goto cleanup;;} while(0)

#define CHECK_ZERO_RETURN_MYSTRING_SUCCESS(number) do {if ((number) == 0) return MYSTRING_SUCCESS;} while(0)
#define CHECK_ZERO_RETURN_MYSTRING_ERROR(number) do {if ((number) == 0) return MYSTRING_ERROR;} while(0)
#define CHECK_ZERO_RETURN_NULL(number) do {if ((number) == 0) return NULL;} while(0)

#define CHECK_MYSTRING_NULL_RETURN_MYSTRING_ERROR(str) do { \
           CHECK_NULL_RETURN_MYSTRING_ERROR(str);\
           CHECK_NULL_RETURN_MYSTRING_ERROR(str->value);\
           } while(0)
#define CHECK_MYSTRING_NULL_RETURN_NULL(str) do { \
           CHECK_NULL_RETURN_NULL(str);\
           CHECK_NULL_RETURN_NULL(str->value);\
           } while(0)
#define CHECK_MYSTRING_NULL_RETURN_VALUE(str, returnValue) do { \
           CHECK_NULL_RETURN_VALUE(str, returnValue);\
           CHECK_NULL_RETURN_VALUE(str->value, returnValue);\
           } while(0)

#define MIN(x,y,result) do { \
            if ((x) < (y)) { \
                (result) = (x); } \
            else { \
                (result) = (y); } \
            } while(0)

#define CHECK_NOT_DIGIT_RETURN_MYSTRING_ERROR(number) do {if ((number) < 0 || 9 < (number)) return MYSTRING_ERROR;} while(0)

#define NULL_CHAR '\0'
#define NULL_CHAR_SIZE 1
