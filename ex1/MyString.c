
// ------------------------------ includes ------------------------------

#include "MyString.h"
#include "MyStringUtils.h"
#include "MyStringMacros.h"

#define MYSTRING_STRINGS_ARE_EQUAL 1
#define MYSTRING_STRINGS_NOT_EQUAL 0
#define MEMCMP_STRINGS_ARE_EQUAL 0
#define LEFT_STRING_IS_GREATER 1
#define RIGHT_STRING_IS_GREATER -1

// -------------------------- const definitions -------------------------
struct _MyString {
    unsigned int length;
    char * value;
};

// -------------------------- private functions -------------------------

static MyStringRetVal setMyString(MyString* str, char * cStr, unsigned int length)
{
    CHECK_NULL_RETURN_MYSTRING_ERROR(str);
    CHECK_NULL_RETURN_MYSTRING_ERROR(cStr);
    free(str->value);
    str->value = cStr;
    str->length = length;
    return MYSTRING_SUCCESS;
}

static void myStringBubbleSortWithComparator(MyString ** array, unsigned int arraySize,
                                             MyStringSortComparator * comparator)
{
    CHECK_NULL_RETURN(array);
    MyString * temp = myStringAlloc();
    CHECK_NULL_RETURN(temp);
    int result = 0;

    // bubble sort
    for(int outerIndex = 1; outerIndex <= arraySize; outerIndex++)
        for(int innerIndex = 0; innerIndex < arraySize - outerIndex; innerIndex++)
        {
            MyString *leftStr = array[innerIndex];
            MyString *rightStr = array[innerIndex + 1];

            result = comparator(leftStr, rightStr);
            CHECK_MYSTRING_ERROR_GOTO_CLEANUP(result);

            if (result == LEFT_STRING_IS_GREATER) {
                SWAP_MYSTRINGS(leftStr, rightStr, temp);
            }
        }

cleanup:
    myStringFree(temp);
}

// -------------------------- functions -------------------------

MyString *myStringAlloc()
{
    MyString * str = (MyString *) malloc(sizeof(MyString));
    CHECK_NULL_RETURN_NULL(str);
    str->value = allocateCStringByLength(NULL_CHAR_SIZE);
    CHECK_NULL_GOTO_CLEANUP(str->value);
    str->length = 0;
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
    return setMyString(str, myStringToCString(other), other->length);
}

MyString *myStringClone(const MyString *str)
{
    CHECK_MYSTRING_NULL_RETURN_NULL(str);
    MyString * newStr = myStringAlloc();
    CHECK_NULL_RETURN_NULL(newStr);
    int result = myStringSetFromMyString(newStr, str);
    CHECK_MYSTRING_ERROR_GOTO_CLEANUP(result);
    return newStr;

cleanup:
    free(newStr);
    return NULL;
}

MyStringRetVal myStringFilter(MyString *str, FilterFunction *filterFunction)
{
    CHECK_MYSTRING_NULL_RETURN_MYSTRING_ERROR(str);
    CHECK_NULL_RETURN_MYSTRING_ERROR(filterFunction);
    char * output = allocateCStringByLength(str->length);
    CHECK_NULL_RETURN_MYSTRING_ERROR(output);
    int newStrLength = myCStringFilter(str->value, str->length, output, filterFunction);
    CHECK_MYSTRING_ERROR_GOTO_CLEANUP(newStrLength);
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
    int strLength = charLength - NULL_CHAR_SIZE;
    CHECK_MYSTRING_ERROR_RETURN_MYSTRING_ERROR(charLength);

    char * output = allocateCStringByLength(charLength);
    CHECK_NULL_RETURN_MYSTRING_ERROR(output);
    memcpy(output, cString, sizeof(char)*strLength);

    CHECK_MYSTRING_ERROR_GOTO_CLEANUP(setMyString(str, output, strLength));
    return MYSTRING_SUCCESS;

cleanup:
    free(output);
    return MYSTRING_ERROR;
}

MyStringRetVal myStringSetFromInt(MyString *str, int number)
{
    CHECK_MYSTRING_NULL_RETURN_MYSTRING_ERROR(str);
    char * output = intToChar(number);
    CHECK_NULL_RETURN_MYSTRING_ERROR(output);
    return myStringSetFromCString(str, output);
}

int myStringToInt(const MyString *str)
{
    CHECK_MYSTRING_NULL_RETURN_MYSTRING_ERROR(str);
    CHECK_ZERO_RETURN_MYSTRING_ERROR(str->length);
    return charToInt(str->value, str->length);
}

char *myStringToCString(const MyString *str)
{
    CHECK_MYSTRING_NULL_RETURN_NULL(str);
    unsigned int cStringLength = str->length + NULL_CHAR_SIZE;

    char * output = allocateCStringByLength(cStringLength);
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
    return setMyString(str1, output, outputLength);
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
    if (str1->length == str2->length)
    {
        return memcmp(str1->value, str2->value, str1->length);
    }

    unsigned int minimalLength = 0;
    MIN(str1->length, str2->length, minimalLength);
    int result = memcmp(str1->value, str2->value, minimalLength);
    if (result == MEMCMP_STRINGS_ARE_EQUAL)
    {
        if (minimalLength == str1->length)  // if str1 is the shorter of the two
        {
            return RIGHT_STRING_IS_GREATER;
        }
        return LEFT_STRING_IS_GREATER;
    }
    return result;
}

int myStringCustomCompare(const MyString *str1, const MyString *str2, MyStringComparator *comparator)
{
    CHECK_MYSTRING_NULL_RETURN_MYSTRING_ERROR(str1);
    CHECK_MYSTRING_NULL_RETURN_MYSTRING_ERROR(str2);
    CHECK_NULL_RETURN_MYSTRING_ERROR(comparator);

    int result = 0;
    for (int i = 0; i < str1->length; i++)
    {
        result = comparator(str1->value[i], str2->value[i]);
        if (result != 0)
        {
            return result;
        }
    }
    return result;
}

int myStringEqual(const MyString *str1, const MyString *str2)
{
    return myStringCustomEqual(str1, str2, charCompare);
}

int myStringCustomEqual(const MyString *str1, const MyString *str2, MyStringComparator *comparator)
{
    CHECK_MYSTRING_NULL_RETURN_MYSTRING_ERROR(str1);
    CHECK_MYSTRING_NULL_RETURN_MYSTRING_ERROR(str2);
    CHECK_NULL_RETURN_MYSTRING_ERROR(comparator);
    if (str1->length != str2->length)
    {
        return MYSTRING_STRINGS_NOT_EQUAL;
    }
    int result = myStringCustomCompare(str1, str2, comparator);
    CHECK_MYSTRING_ERROR_RETURN_MYSTRING_ERROR(result);
    if (result == MEMCMP_STRINGS_ARE_EQUAL)
    {
        return MYSTRING_STRINGS_ARE_EQUAL;
    }
    return MYSTRING_STRINGS_NOT_EQUAL;
}

unsigned long getMyStringMemoryUsage(const MyString *str)
{
    CHECK_MYSTRING_NULL_RETURN_0(str);
    return sizeof(MyString) + sizeof(char)*str->length;
}

unsigned long getMyStringLength(const MyString *str)
{
    CHECK_MYSTRING_NULL_RETURN_0(str);
    return str->length;
}

MyStringRetVal myStringWrite(const MyString *str, FILE *stream)
{
    CHECK_MYSTRING_NULL_RETURN_MYSTRING_ERROR(str);
    CHECK_NULL_RETURN_MYSTRING_ERROR(stream);
    char * cStr = myStringToCString(str);
    CHECK_NULL_RETURN_MYSTRING_ERROR(cStr);
    int result = fputs(cStr, stream);
    free(cStr);
    if (result < 0 || result == EOF)
    {
        return MYSTRING_ERROR;
    }
    return MYSTRING_SUCCESS;
}

void myStringCustomSort(MyString **array, unsigned int arraySize, MyStringSortComparator *comparator)
{
    if (myStringArrayCheckNull(array, arraySize) == MYSTRING_SUCCESS &&
        comparator != NULL &&
        arraySize > 1)
    {
        myStringBubbleSortWithComparator(array, arraySize, comparator);
    }
}

void myStringSort(MyString **array, unsigned int arraySize)
{
    myStringCustomSort(array, arraySize, (MyStringSortComparator*) myStringCompare);
}

MyString ** getArrayOfMyStringBySize(unsigned int arraySize)
{
    CHECK_ZERO_RETURN_NULL(arraySize);
    MyString **array = (MyString **) malloc(sizeof(MyString) * arraySize);
    CHECK_NULL_RETURN_NULL(array);
    for (int i = 0; i < arraySize; i++)
    {
        array[i] = myStringAlloc();
        if (array[i] == NULL)
        {
            freeArrayOfMyStringBySize(array, i);
            return NULL;
        }
    }
    return array;
}

void freeArrayOfMyStringBySize(MyString ** array, unsigned int arraySize)
{
    CHECK_NULL_RETURN(array);
    for (int i = 0; i < arraySize; i++)
    {
        myStringFree(array[i]);
    }
    free(array);
}

MyStringRetVal myStringArrayCheckNull(MyString **array, unsigned int arraySize)
{
    CHECK_NULL_RETURN_MYSTRING_ERROR(array);
    for (int i = 0; i < arraySize; i++)
    {
        CHECK_MYSTRING_NULL_RETURN_MYSTRING_ERROR(array[i]);
    }
    return MYSTRING_SUCCESS;
}