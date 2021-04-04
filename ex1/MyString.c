
// ------------------------------ includes ------------------------------

#include "MyString.h"
#include "MyStringUtils.h"
#include "MyStringMacros.h"

// -------------------------- const definitions -------------------------

struct _MyString {
    unsigned int length;
    char * value;
};

#define MYSTRING_STRINGS_ARE_EQUAL 1
#define MYSTRING_STRINGS_NOT_EQUAL 0
#define CMP_STRINGS_ARE_EQUAL 0
#define LEFT_STRING_IS_GREATER 1
#define RIGHT_STRING_IS_GREATER -1

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

static MyStringRetVal swapMyStringContents(MyString *left, MyString *right, MyString *temp)
{
    CHECK_NULL_RETURN_MYSTRING_ERROR(left);
    CHECK_NULL_RETURN_MYSTRING_ERROR(right);
    CHECK_NULL_RETURN_MYSTRING_ERROR(temp);
    int result = 0;

    result = myStringSetFromMyString((temp), (left));
    CHECK_MYSTRING_ERROR_RETURN_MYSTRING_ERROR(result);

    result = myStringSetFromMyString((left), (right));
    CHECK_MYSTRING_ERROR_RETURN_MYSTRING_ERROR(result);

    result = myStringSetFromMyString((right), (temp));
    CHECK_MYSTRING_ERROR_RETURN_MYSTRING_ERROR(result);

    return MYSTRING_SUCCESS;
}

static void myStringBubbleSortWithComparator(MyString ** array, unsigned int arraySize,
                                             my_string_sort_comparator_t * comparator)
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
                result = swapMyStringContents(leftStr, rightStr, temp);
                CHECK_MYSTRING_ERROR_GOTO_CLEANUP(result);
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
    myStringFree(str);
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
    myStringFree(newStr);
    return NULL;
}

MyStringRetVal myStringFilter(MyString *str, filter_function_t *filterFunction)
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

    int charLength = cStringLength(cString);
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
    char * output = intToCString(number);
    CHECK_NULL_RETURN_MYSTRING_ERROR(output);
    return myStringSetFromCString(str, output);
}

int myStringToInt(const MyString *str)
{
    CHECK_MYSTRING_NULL_RETURN_MYSTRING_ERROR(str);
    CHECK_ZERO_RETURN_MYSTRING_ERROR(str->length);
    return cStringToInt(str->value, str->length);
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

MyStringRetVal myStringConcat(MyString *dest, const MyString *source)
{
    CHECK_MYSTRING_NULL_RETURN_MYSTRING_ERROR(dest);
    CHECK_MYSTRING_NULL_RETURN_MYSTRING_ERROR(source);

    unsigned int outputLength = dest->length + source->length;
    CHECK_ZERO_RETURN_MYSTRING_SUCCESS(outputLength);

    char * output = charConcat(dest->value, dest->length, source->value, source->length);
    CHECK_NULL_RETURN_MYSTRING_ERROR(output);
    return setMyString(dest, output, outputLength);
}

MyStringRetVal myStringConcatToResult(const MyString *str1, const MyString *str2, MyString *result)
{
    CHECK_MYSTRING_NULL_RETURN_MYSTRING_ERROR(str1);
    CHECK_MYSTRING_NULL_RETURN_MYSTRING_ERROR(str2);
    CHECK_MYSTRING_NULL_RETURN_MYSTRING_ERROR(result);

    MyStringRetVal firstConcatReturnValue = myStringSetFromMyString(result, str1);
    CHECK_MYSTRING_ERROR_RETURN_MYSTRING_ERROR(firstConcatReturnValue);
    return myStringConcat(result, str2);
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
    if (result != CMP_STRINGS_ARE_EQUAL)
    {
        return result;
    }
    // the two strings are equal but only by the shortest comparable string (E. abc and ab)
    if (minimalLength != str2->length)
    {
        return RIGHT_STRING_IS_GREATER;
    }
    return LEFT_STRING_IS_GREATER;
}

int myStringCustomCompare(const MyString *str1, const MyString *str2, my_string_comparator_t *comparator)
{
    CHECK_MYSTRING_NULL_RETURN_MYSTRING_ERROR(str1);
    CHECK_MYSTRING_NULL_RETURN_MYSTRING_ERROR(str2);
    CHECK_NULL_RETURN_MYSTRING_ERROR(comparator);

    int result = 0;
    for (int i = 0; i < str1->length; i++)
    {
        result = comparator(str1->value[i], str2->value[i]);
        if (result != CMP_STRINGS_ARE_EQUAL)
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

int myStringCustomEqual(const MyString *str1, const MyString *str2, my_string_comparator_t *comparator)
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
    return result == CMP_STRINGS_ARE_EQUAL ? MYSTRING_STRINGS_ARE_EQUAL : MYSTRING_STRINGS_NOT_EQUAL;
}

unsigned long getMyStringMemoryUsage(const MyString *str)
{
    CHECK_MYSTRING_NULL_RETURN_VALUE(str, 0);
    return sizeof(MyString) + sizeof(char)*str->length;
}

unsigned long getMyStringLength(const MyString *str)
{
    CHECK_MYSTRING_NULL_RETURN_VALUE(str, 0);
    return str->length;
}

MyStringRetVal myStringWriteStrValueToStream(const MyString *str, FILE *stream)
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

void myStringCustomSort(MyString **array, unsigned int arraySize, my_string_sort_comparator_t *comparator)
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
    myStringCustomSort(array, arraySize, (my_string_sort_comparator_t*) myStringCompare);
}

MyString ** allocateMyStringArrayBySize(unsigned int arraySize)
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