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
        str->len = other->len;
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

MyStringRetVal myStringSetFromCString(MyString *str, const char *cString)
{
    if (myStringCheckNull(str) == MYSTRING_SUCCESS && cString != NULL)
    {
        int n = charArrayLen(cString);
        if (n == -1)
        {
            return MYSTRING_ERROR;
        }
        free(str->value);
        str->value = (char *) malloc(sizeof(char)*n);
        if (str->value == NULL)
        {
            return MYSTRING_ERROR;
        }
        str->len = n;
        memcpy(str->value, cString, sizeof(char)*str->len);
        str->value[n] = '\0';
        return MYSTRING_SUCCESS;
    }
    return MYSTRING_ERROR;
}

MyStringRetVal myStringSetFromInt(MyString *str, int n)
{
    if (myStringCheckNull(str) == MYSTRING_SUCCESS)
    {
        char * output = (char *) malloc(sizeof(char));
        if (output == NULL)
        {
            return MYSTRING_ERROR;
        }
        intToChar(output, n);
        int len = charArrayLen(output);
        if (len == -1)
        {
            return MYSTRING_ERROR;
        }
        free(str->value);
        str->value = output;
        str->len = len;
        return MYSTRING_SUCCESS;

    }
    return MYSTRING_ERROR;
}

int myStringToInt(const MyString *str)
{
    if (myStringCheckNull(str) == MYSTRING_SUCCESS)
    {
        if (str->len == 0)
        {
            return MYSTR_ERROR_CODE;
        }

        char * current = str->value;
        int decile = str->len - 1;
        int output = 0;
        int sign = 1;
        int i = 0;

        if (*current == '-')
        {
            sign = -1;
            i++;
            decile--;
        }

        for (i; i < str->len; i++)
        {
            int digit = current[i] - ASCII_ZERO_DEC;
            if (digit < 0 || 9 < digit)
            {
                return MYSTR_ERROR_CODE;
            }
            double position = pow(10, decile);
            output += ((int)position) * digit;
            decile--;
        }
        return output * sign;
    }
    return MYSTRING_ERROR;
}

char *myStringToCString(const MyString *str)
{
    if (myStringCheckNull(str) == MYSTRING_SUCCESS)
    {
        char * output = (char *) malloc(sizeof(char)*str->len);
        if (output == NULL)
        {
            return NULL;
        }
        memcpy(output, str->value, sizeof(char)*str->len);
        output[str->len] = '\0';
        return output;
    }
    return NULL;
}

MyStringRetVal myStringCat(MyString *dest, const MyString *src)
{
    if (myStringCheckNull(dest) == MYSTRING_SUCCESS &&
        myStringCheckNull(src) == MYSTRING_SUCCESS)
    {
        int new_len = dest->len + src->len;
        char * output = (char *) malloc(sizeof(char)*new_len);
        if (output == NULL)
        {
            return MYSTRING_ERROR;
        }
        memcpy(output, dest->value, sizeof(char)*dest->len);
        char * output_cat = output + dest->len;
        memcpy(output_cat, src->value, sizeof(char)*src->len);

        free(dest->value);
        dest->value = output;
        dest->len = new_len;
        return MYSTRING_SUCCESS;
    }
    return MYSTRING_ERROR;
}

MyStringRetVal myStringCatTo(const MyString *str1, const MyString *str2, MyString *result)
{
    if (myStringCheckNull(str1) == MYSTRING_SUCCESS &&
        myStringCheckNull(str2) == MYSTRING_SUCCESS &&
        myStringCheckNull(result) == MYSTRING_SUCCESS)
    {
        MyStringRetVal retVal = myStringSetFromMyString(result, str1);
        if (retVal == MYSTRING_ERROR)
        {
            return MYSTRING_ERROR;
        }
        return myStringCat(result, str2);
    }
    return MYSTRING_ERROR;
}

int myStringCompare(const MyString *str1, const MyString *str2)
{
    if (myStringCheckNull(str1) == MYSTRING_SUCCESS &&
        myStringCheckNull(str2) == MYSTRING_SUCCESS &&
        str1->len == str2->len)
    {
        return memcmp(str1->value, str2->value, str1->len);
    }
    return MYSTR_ERROR_CODE;
}

int myStringCustomCompare(const MyString *str1, const MyString *str2,
                          int (*comparator)(const char, const char))
{
    if (myStringCheckNull(str1) == MYSTRING_SUCCESS &&
        myStringCheckNull(str2) == MYSTRING_SUCCESS &&
        str1->len == str2->len)
    {
        int result;
        for (int i = 0; i < str1->len; i++)
        {
            result = comparator(*(str1->value + i), *(str2->value + i));
            if (result != 0)
            {
                return result;
            }
        }
        return 0;
    }
    return MYSTR_ERROR_CODE;
}