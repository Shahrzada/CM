#pragma once

#define CHECK_NULL_RETURN_NULL(pointer) do {if ((pointer) == NULL) return NULL;} while(0)
#define CHECK_NULL_RETURN(pointer) do {if ((pointer) == NULL) return;} while(0)
#define CHECK_NULL_RETURN_MYSTRING_ERROR(pointer) do {if ((pointer) == NULL) return MYSTRING_ERROR;} while(0)

#define CHECK_MYSTRING_NULL_RETURN_MYSTRING_ERROR(str) do { \
           IF_NULL_RETURN_MYSTRING_ERROR(str);\
           IF_NULL_RETURN_MYSTRING_ERROR(str->value);\
           } while(0)
#define CHECK_MYSTRING_NULL_RETURN_NULL(str) do { \
           CHECK_NULL_RETURN_NULL(str);\
           CHECK_NULL_RETURN_NULL(str->value);\
           } while(0)
#define CHECK_MYSTRING_NULL_RETURN_0(str) do { \
           IF_NULL_RETURN_MYSTRING_ERROR(str);\
           IF_NULL_RETURN_MYSTRING_ERROR(str->value);\
           } while(0)
