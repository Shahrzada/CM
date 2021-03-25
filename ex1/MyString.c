#include "MyString.h"



// -------------------------- const definitions -------------------------

/*
 * mystr error code
 */
#define EMPTY_STR '\0'

struct _MyString {
    char * value;
    int len;
};

// -------------------------- private functions -------------------------
MyStringRetVal myStringSetValue(MyString *str, char * value)
{
    if (str != NULL && str->value != NULL && value != NULL) {
        str->len = charArrayLen(value);
        memcpy(str->value, value, sizeof(char)*str->len);
        *(str->value + str->len) = '\0';
        return MYSTRING_SUCCESS;
    }
    return MYSTRING_ERROR;
}


MyStringRetVal myStringCheckNull(const MyString *str)
{
    if (str != NULL && str->value != NULL)
    {
        return MYSTRING_SUCCESS;
    }
    return MYSTRING_ERROR;
}

// -------------------------- header functions -------------------------

MyString *myStringAlloc()
{
    MyString * str = (MyString *) malloc(sizeof(MyString));
    if (str != NULL) {
        str->value = (char *) malloc(sizeof(char));
        if (str->value != NULL) {
            *(str->value) = EMPTY_STR;
            str->len = 0;
            return str;
        }
    }
    return NULL;
}

void myStringFree(MyString *str)
{
    if (str != NULL)
    {
        if (str->value != NULL)
        {
            free(str->value);
        }
        free(str);
    }
}

MyString *myStringClone(const MyString *str)
{
    if (myStringCheckNull(str) == MYSTRING_SUCCESS)
    {
        MyString * new_str = myStringAlloc();
        memcpy(new_str->value, str->value, sizeof(char)*str->len);
        new_str->len = str->len;
        return new_str;
    }
    return NULL;
}

MyStringRetVal myStringSetFromMyString(MyString *str, const MyString *other)
{
    if (myStringCheckNull(str) == MYSTRING_SUCCESS &&
        myStringCheckNull(other) == MYSTRING_SUCCESS)
    {
        memcpy(str->value, other->value, sizeof(char)*other->len);
        return MYSTRING_SUCCESS;
    }
    return MYSTRING_ERROR;
}

MyStringRetVal myStringFilter(MyString *str, bool (*filt)(const char *))
{
    if (myStringCheckNull(str) == MYSTRING_SUCCESS)
    {
        char * output = (char *) malloc(sizeof(char));
        if (output == NULL)
        {
            return MYSTRING_ERROR;
        }
        char * current = str->value;
        int n = 0;

        while (*current != '\0')
        {
            if (filt(current) == false)
            {
                output[n] = *current;
                n++;
            }
            current++;
        }
        output[n] = '\0';
        str->len = n;
        free(str->value);
        str->value = output;
    }
    return MYSTRING_ERROR;
}