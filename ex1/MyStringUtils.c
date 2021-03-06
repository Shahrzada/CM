
// ------------------------------ includes ------------------------------

#include "MyStringUtils.h"
#include "MyStringMacros.h"

// -------------------------- private functions -------------------------

static int getTotalDigitsOfNumber(int number)
{
    int result = 0;
    if (number < 0) // count in the minus sign
    {
        result++;
    }
    do
    {
        result++;
        number /= 10;
    }
    while(number != 0);

    return result;
}

// -------------------------- functions -------------------------

int cStringLength(const char * cStr)
{
    CHECK_NULL_RETURN_MYSTRING_ERROR(cStr);
    const char * current = cStr;
    while (*current != NULL_CHAR)
    {
        current++;
    }
    return (current - cStr) + NULL_CHAR_SIZE;
}

int cStringToInt(char * cStr, unsigned int size)
{
    CHECK_NULL_RETURN_MYSTRING_ERROR(cStr);
    CHECK_ZERO_RETURN_MYSTRING_ERROR(size);

    char * current = cStr;
    unsigned int decile = size - 1;
    int output = 0;
    int sign = 1;
    int i = 0;

    if (*current == MINUS_SYMBOL)
    {
        sign = -1;
        i++;
        decile--;
    }

    while (i < size)
    {
        int digit = current[i] - ASCII_ZERO_DEC;
        CHECK_NOT_DIGIT_RETURN_MYSTRING_ERROR(digit);
        double position = pow(10, decile);
        output += ((int)position) * digit;
        decile--;
        i++;
    }
    return output * sign;
}

char * intToCString(int number)
{
    int totalDigits = getTotalDigitsOfNumber(number);
    char * output = (char *) malloc(sizeof(char)*totalDigits);
    CHECK_NULL_RETURN_NULL(output);
    int signPosition = -1;
    if (number < 0)
    {
        *output = MINUS_SYMBOL;
        signPosition++;
    }
    else
    {
        number = -number;
    }
    for (int i = totalDigits - 1; i > signPosition; i--, number /= 10)
    {
        int digit = - number % 10;
        output[i] = ASCII_ZERO_DEC + digit;
    }
    return output;
}

char * charConcat(const char * cStr1, unsigned int cStr1Length, const char * cStr2, unsigned int cStr2Length)
{
    CHECK_NULL_RETURN_NULL(cStr1);
    CHECK_NULL_RETURN_NULL(cStr2);

    unsigned outputLength = cStr1Length + cStr2Length;

    char * output = (char *) malloc(sizeof(char)*(outputLength + NULL_CHAR_SIZE));
    CHECK_NULL_RETURN_NULL(output);

    memcpy(output, cStr1, sizeof(char)*cStr1Length);
    char * outputPointer = output + cStr1Length;
    memcpy(outputPointer, cStr2, sizeof(char)*(cStr2Length + NULL_CHAR_SIZE));

    return output;
}

int myCStringFilter(char *cStr, unsigned int strLength, char *output, filter_function_t *filterFunction)
{
    CHECK_NULL_RETURN_MYSTRING_ERROR(cStr);
    CHECK_NULL_RETURN_MYSTRING_ERROR(output);
    int newStrLength = 0;
    for (int i = 0; i < strLength; i++)
    {
        if (filterFunction(&cStr[i]) == false) // By returning false it means "keep this char"
        {
            output[newStrLength] = cStr[i];
            newStrLength++;
        }
    }
    if (newStrLength < strLength) // we want to save up memory usage
    {
        output = (char *) realloc(output, sizeof(char)*newStrLength);
        CHECK_NULL_RETURN_MYSTRING_ERROR(output);
    }
    return newStrLength;
}

int charCompare(const char char1, const char char2)
{
    return memcmp(&char1, &char2, 1);
}

char * allocateCStringByLength(unsigned int length)
{
    char * output = (char *) malloc(sizeof(char)*(length));
    return output;
}
