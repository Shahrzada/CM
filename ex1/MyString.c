
// ------------------------------ includes ------------------------------

#include "MyString.h"
#include "MyStringUtils.h"
#include "MyStringMacros.h"

// -------------------------- const definitions -------------------------
// CR: Why did you choose to put this here and not in the header file?
// CR_S: The declaration the structure is in the header, the variables are
//       in this a source file for avoiding access from outside directly;
//       There are getters and setters for both.
struct _MyString {
    int length;
    char * value;
};

// -------------------------- private functions -------------------------

static MyStringRetVal myStringArrayCheckNull(MyString **arr, unsigned int size)
{
    IF_NULL_RETURN_MYSTRING_ERROR(arr);
    for (int i = 0; i < size; i++)
    {
        CHECK_MYSTRING_NULL_RETURN_MYSTRING_ERROR(arr[i]);
    }
    return MYSTRING_SUCCESS;
}

// -------------------------- functions -------------------------

MyString *myStringAlloc()
{
    MyString * str = (MyString *) malloc(sizeof(MyString));
    CHECK_NULL_RETURN_NULL(str);

    str->value = (char *) malloc(sizeof(char));
    if (str->value == NULL)
    {
        // CR: In c, you should use goto for cleanup.
        free(str);
        return NULL;
    }
    
    str->length = 0;
    // CR: Do you really need this?
    *(str->value) = NULL_CHAR;
    return str;
}

void myStringFree(MyString *str)
{
    CHECK_NULL_RETURN(str);
    if (str->value != NULL)
    {
        free(str->value);
    }
    free(str);
}

MyStringRetVal myStringSetFromMyString(MyString *str, const MyString *other)
{
    CHECK_MYSTRING_NULL_RETURN_MYSTRING_ERROR(str);
    CHECK_MYSTRING_NULL_RETURN_MYSTRING_ERROR(other);
    return myStringSetFromCString(str, other->value);
}

MyString *myStringClone(const MyString *str)
{
    CHECK_MYSTRING_NULL_RETURN_NULL(str);
    MyString * new_str = myStringAlloc();
    CHECK_NULL_RETURN_NULL(new_str);
    if (myStringSetFromMyString(new_str, str) == MYSTRING_ERROR)
    {
        myStringFree(new_str);
        return NULL;
    }
    return new_str;
}
// CR: filt is not really a good name a variable, dont cheap out on the name(filterFunc)
// CR: use a typedef for the filt, it would be more readable. filter_func_t 
MyStringRetVal myStringFilter(MyString *str, bool (*filt)(const char *))
{
    CHECK_MYSTRING_NULL_RETURN_MYSTRING_ERROR(str);
    // CR: this is dangurous, since you have a lot of malloc calls in youre code
    //     and not a single place to unifity it, this can happen.
    char * output = (char *) malloc(sizeof(char));
    IF_NULL_RETURN_MYSTRING_ERROR(output);

    char * current = str->value;
    int n = 0;

    while (*current != '\0') // CR: magic
    {
        if (filt(current) == false)
        {
            output[n] = *current;
            n++;
        }
        current++;
    }
    output[n] = '\0'; // CR: magic
    str->length = n;
    // CR: this should be preformed from a "private" function
    free(str->value);
    str->value = output;
    return MYSTRING_SUCCESS;

}

MyStringRetVal myStringSetFromCString(MyString *str, const char *cString)
{
    CHECK_MYSTRING_NULL_RETURN_MYSTRING_ERROR(str);
    CHECK_NULL_RETURN_MYSTRING_ERROR(cString);

    int charLength = charArrayLen(cString);
    CHECK_MYSTRING_ERROR_RETURN_MYSTRING_ERROR(charLength);

    char * newCString = (char *) malloc(sizeof(char)*(charLength));
    IF_NULL_RETURN_MYSTRING_ERROR(newCString);

    memcpy(newCString, cString, sizeof(char)*charLength);
    free(str->value);
    str->value = newCString;
    str->length = charLength - LAST_NULL_CHAR_SIZE;
    return MYSTRING_SUCCESS;
}

MyStringRetVal myStringSetFromInt(MyString *str, int n)
{
    CHECK_MYSTRING_NULL_RETURN_MYSTRING_ERROR(str);
    char * output = intToChar(n);
    CHECK_NULL_RETURN_MYSTRING_ERROR(output);
    return myStringSetFromCString(str, output);
}

int myStringToInt(const MyString *str)
{
    CHECK_MYSTRING_NULL_RETURN_MYSTRING_ERROR(str);
    if (str->length == 0)
    {
        return MYSTRING_ERROR;
    }
    return charToInt(str->value, str->length);
}

char *myStringToCString(const MyString *str)
{
    CHECK_MYSTRING_NULL_RETURN_NULL(str);
    int cStringLength = str->length + LAST_NULL_CHAR_SIZE;
    char * output = (char *) malloc(sizeof(char)*cStringLength);
    CHECK_NULL_RETURN_NULL(output);
    memcpy(output, str->value, sizeof(char)*cStringLength);
    output[str->length] = NULL_CHAR;
    return output;
}

MyStringRetVal myStringConcatToFirst(MyString *str1, const MyString *str2)
{
    CHECK_MYSTRING_NULL_RETURN_MYSTRING_ERROR(str1);
    CHECK_MYSTRING_NULL_RETURN_MYSTRING_ERROR(str2);

    int outputLength = str1->length + str2->length;
    CHECK_ZERO_RETURN_MYSTRING_SUCCESS(outputLength);

    char * output = charConcat(str1->value, str1->length, str2->value, str2->length);
    IF_NULL_RETURN_MYSTRING_ERROR(output);

    return myStringSetFromCString(str1, output);
}

MyStringRetVal myStringConcatToFirstResult(const MyString *str1, const MyString *str2, MyString *result)
{
    CHECK_MYSTRING_NULL_RETURN_MYSTRING_ERROR(str1);
    CHECK_MYSTRING_NULL_RETURN_MYSTRING_ERROR(str2);
    CHECK_MYSTRING_NULL_RETURN_MYSTRING_ERROR(result);

    MyStringRetVal firstConcatReturnValue = myStringSetFromMyString(result, str1);
    CHECK_MYSTRING_ERROR_RETURN_MYSTRING_ERROR(firstConcatReturnValue);
    return myStringConcatToFirst(result, str2);
}

int myStringCompare(const MyString *str1, const MyString *str2)
{
    CHECK_MYSTRING_NULL_RETURN_MYSTRING_ERROR(str1);
    CHECK_MYSTRING_NULL_RETURN_MYSTRING_ERROR(str2);
    return memcmp(str1->value, str2->value, str1->length);
}

// CR: typedef comparator like filt(lol)
int myStringCustomCompare(const MyString *str1, const MyString *str2,
                          int (*comparator)(const char, const char))
{
    CHECK_MYSTRING_NULL_RETURN_MYSTRING_ERROR(str1);
    CHECK_MYSTRING_NULL_RETURN_MYSTRING_ERROR(str2);
    CHECK_NULL_RETURN_MYSTRING_ERROR(comparator);

    int result = -1;
    for (int i = 0; i < str1->length; i++)
    {
        result = comparator(*(str1->value + i), *(str2->value + i));
        if (result != 0)
        {
            return result;
        }
    }
    return 0;
}

int myStringEqual(const MyString *str1, const MyString *str2)
{
    CHECK_MYSTRING_NULL_RETURN_MYSTRING_ERROR(str1);
    CHECK_MYSTRING_NULL_RETURN_MYSTRING_ERROR(str2);

    if (str1->length != str2->length)
    {
        return 0; // CR: 0 is good or bad?
    }
    int result = myStringCompare(str1, str2);
    if (result == 0)
    {
        return 1; // CR: 1 is good or bad?
    }
    else if (result == MYSTRING_ERROR)
    {
        return result;
    }
    return 0;
}

// CR: seems like a bit duplication from the code above.
int myStringCustomEqual(const MyString *str1, const MyString *str2,
                        int (*comparator)(const char, const char))
{
    CHECK_MYSTRING_NULL_RETURN_MYSTRING_ERROR(str1);
    CHECK_MYSTRING_NULL_RETURN_MYSTRING_ERROR(str2);
    CHECK_NULL_RETURN_MYSTRING_ERROR(comparator);

    if (str1->length != str2->length)
    {
        return 0;
    }
    int result = myStringCustomCompare(str1, str2, comparator);
    if (result == 0)
    {
        return 1;
    }
    else if (result == MYSTRING_ERROR)
    {
        return result;
    }
    return 0;
}

unsigned long myStringMemUsage(const MyString *str1)
{
    CHECK_MYSTRING_NULL_RETURN_0(str1);
    return sizeof(MyString) + sizeof(char)*str1->length;
}

unsigned long myStringLen(const MyString *str1)
{
    CHECK_MYSTRING_NULL_RETURN_0(str1);
    return str1->length;
}

MyStringRetVal myStringWrite(const MyString *str, FILE *stream)
{
    CHECK_MYSTRING_NULL_RETURN_MYSTRING_ERROR(str);
    CHECK_NULL_RETURN_MYSTRING_ERROR(stream);

    int result = fputs(str->value, stream);
    if (result < 0 || result == EOF)
    {
        return MYSTRING_ERROR;
    }
    return MYSTRING_SUCCESS;
}

void myStringCustomSort(MyString **arr, unsigned int size,
                        int (*comparator)(const void *, const void *))
{
    if (myStringArrayCheckNull(arr, size) == MYSTRING_SUCCESS &&
        comparator != NULL && size > 1)
    {
        quicksortCharArraysUsingComp(arr, comparator, 0, (int) size - 1);
    }
}

void myStringSort(MyString **arr, unsigned int len)
{
    // CR: nice, but this shouldve been your clue to use typedef :D
    myStringCustomSort(arr, len, (int (*)(const void *, const void *)) myStringCompare);
}

MyString ** getArrayOfMyStringBySize(int arraySize)
{
    if (arraySize <= 0)
    {
        return NULL;
    }

    MyString **array = (MyString **) malloc(sizeof(MyString) * arraySize);
    for (int i = 0; i < arraySize; i++)
    {
        MyString * a = myStringAlloc();
        if (a != NULL)
        {
            array[i] = a;
        }
        else
        {
            freeArrayOfMyStringBySize(array, i);
            return NULL;
        }
    }
    return array;
}

void freeArrayOfMyStringBySize(MyString ** array, int arraySize)
{
    CHECK_NULL_RETURN(array);
    for (int i = 0; i < arraySize; i++)
    {
        myStringFree(array[i]);
    }
    free(array);
}
