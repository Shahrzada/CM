#pragma once

#define CHECK_NULL_RETURN_NULL(pointer) do {if ((pointer) == NULL) return NULL;} while(0)
#define CHECK_NULL_RETURN(pointer) do {if ((pointer) == NULL) return;} while(0)
#define CHECK_NULL_RETURN_MYSTRING_ERROR(pointer) do {if ((pointer) == NULL) return MYSTRING_ERROR;} while(0)
#define CHECK_NULL_RETURN_0(pointer) do {if ((pointer) == NULL) return 0;} while(0)
#define CHECK_NULL_GOTO_CLEANUP(pointer) do {if ((pointer) == NULL) goto cleanup;;} while(0)

#define CHECK_MYSTRING_ERROR_RETURN_MYSTRING_ERROR(number) do {if ((number) == MYSTRING_ERROR) return MYSTRING_ERROR;} while(0)
#define CHECK_MYSTRING_ERROR_GOTO_CLEANUP(number) do {if ((number) == MYSTRING_ERROR) goto cleanup;;} while(0)

#define CHECK_ZERO_RETURN_MYSTRING_SUCCESS(number) do {if ((number) == 0) return MYSTRING_SUCCESS;} while(0)
#define CHECK_ZERO_RETURN_MYSTRING_ERROR(number) do {if ((number) == 0) return MYSTRING_ERROR;} while(0)

#define CHECK_MYSTRING_NULL_RETURN_MYSTRING_ERROR(str) do { \
           CHECK_NULL_RETURN_MYSTRING_ERROR(str);\
           CHECK_NULL_RETURN_MYSTRING_ERROR(str->value);\
           } while(0)
#define CHECK_MYSTRING_NULL_RETURN_NULL(str) do { \
           CHECK_NULL_RETURN_NULL(str);\
           CHECK_NULL_RETURN_NULL(str->value);\
           } while(0)
#define CHECK_MYSTRING_NULL_RETURN_0(str) do { \
           CHECK_NULL_RETURN_0(str);\
           CHECK_NULL_RETURN_0(str->value);\
           } while(0)


#define NULL_CHAR '\0'
#define LAST_NULL_CHAR_SIZE 1