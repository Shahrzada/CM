
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
    unsigned int length;
    char * value;
};

// -------------------------- private functions -------------------------

static MyStringRetVal myStringArrayCheckNull(MyString **arr, unsigned int size)
{
    CHECK_NULL_RETURN_MYSTRING_ERROR(arr);
    for (int i = 0; i < size; i++)
    {
        CHECK_MYSTRING_NULL_RETURN_MYSTRING_ERROR(arr[i]);
    }
    return MYSTRING_SUCCESS;
}

// CR_S: why is this a private function and not a macro function? I guess because this is
//       more like a getter/setter for an 'object' rather than a checker of some sort.
static MyStringRetVal setMyString(MyString* str, char * cStr, unsigned int length)
{
    CHECK_NULL_RETURN_MYSTRING_ERROR(str);
    CHECK_NULL_RETURN_MYSTRING_ERROR(cStr);
    free(str->value);
    str->value = cStr;
    str->length = length;
    return MYSTRING_SUCCESS;
}

// -------------------------- functions -------------------------

MyString *myStringAlloc()
{
    MyString * str = (MyString *) malloc(sizeof(MyString));
    CHECK_NULL_RETURN_NULL(str);

    str->value = (char *) malloc(sizeof(char));
    CHECK_NULL_GOTO_CLEANUP(str->value);

    str->length = 0;
    // CR: Do you really need this?
    *(str->value) = NULL_CHAR;
    return str;

cleanup:
    free(str);
    return NULL;
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

MyStringRetVal myStringFilter(MyString *str, FilterFunction *filterFunction)
{
    CHECK_MYSTRING_NULL_RETURN_MYSTRING_ERROR(str);
    CHECK_NULL_RETURN_MYSTRING_ERROR(filterFunction);

    // CR: this is dangurous, since you have a lot of malloc calls in youre code
    //     and not a single place to unifity it, this can happen.

    char * output = (char *) malloc(sizeof(char)*str->length);
    CHECK_NULL_RETURN_MYSTRING_ERROR(output);
    unsigned int newStrLength = myCStringFilter(str->value, str->length, output, filterFunction);
    CHECK_MYSTRING_ERROR_GOTO_CLEANUP(newStrLength);

    if (newStrLength < str->length)
    {
        output = (char *) realloc(output, sizeof(char)*newStrLength);
        CHECK_NULL_GOTO_CLEANUP(output);
    }

    CHECK_MYSTRING_ERROR_GOTO_CLEANUP(setMyString(str, output, newStrLength));
    return MYSTRING_SUCCESS;

cleanup:
    free(output);
    return MYSTRING_ERROR;
}

MyStringRetVal myStringSetFromCString(MyString *str, const char *cString)
{
    CHECK_MYSTRING_NULL_RETURN_MYSTRING_ERROR(str);
    CHECK_NULL_RETURN_MYSTRING_ERROR(cString);

    int charLength = charArrayLen(cString);
    CHECK_MYSTRING_ERROR_RETURN_MYSTRING_ERROR(charLength);

    char * newCString = (char *) malloc(sizeof(char)*(charLength));
    CHECK_NULL_RETURN_MYSTRING_ERROR(newCString);

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
    unsigned int cStringLength = str->length + LAST_NULL_CHAR_SIZE;
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

    unsigned int outputLength = str1->length + str2->length;
    CHECK_ZERO_RETURN_MYSTRING_SUCCESS(outputLength);

    char * output = charConcat(str1->value, str1->length, str2->value, str2->length);
    CHECK_NULL_RETURN_MYSTRING_ERROR(output);

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

int myStringCustomCompare(const MyString *str1, const MyString *str2, MyStringComparator *comparator)
{
    CHECK_MYSTRING_NULL_RETURN_MYSTRING_ERROR(str1);
    CHECK_MYSTRING_NULL_RETURN_MYSTRING_ERROR(str2);
    CHECK_NULL_RETURN_MYSTRING_ERROR(comparator);

    int result = 0;
    for (int i = 0; i < str1->length; i++)
    {
        result = comparator(*(str1->value + i), *(str2->value + i));
        if (result != 0)
        {
            return result;
        }
    }
    return result;
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
int myStringCustomEqual(const MyString *str1, const MyString *str2, MyStringComparator *comparator)
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

void myStringCustomSort(MyString **arr, unsigned int size, MyStringSortComparator *comparator)
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
