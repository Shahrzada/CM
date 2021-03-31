
// ------------------------------ includes ------------------------------

#include "MyStringUtils.h"
#include "MyStringMacros.h"

// -------------------------- private functions -------------------------

static int quicksortMoveRight(MyString **array, int (*comparator)(const void *, const void *),
                       int i, int pivot, unsigned int last)
{
    CHECK_NULL_RETURN_MYSTRING_ERROR(array);
    CHECK_NULL_RETURN_MYSTRING_ERROR(comparator);
    int resultIPivot;
    while(1)  // while (arr[i] <= arr[pivot] and i < last)
    {
        CHECK_NULL_RETURN_MYSTRING_ERROR(array[i]);
        resultIPivot = comparator(array[i], array[pivot]);
        CHECK_MYSTRING_ERROR_RETURN_MYSTRING_ERROR(resultIPivot);
        if (resultIPivot > 0 || i >= last)
        {
            return i;
        }
        i++;
    }
}

static int quicksortMoveLeft(MyString **array, int (*comparator)(const void *, const void *),
                      int j, int pivot)
{
    CHECK_NULL_RETURN_MYSTRING_ERROR(array);
    CHECK_NULL_RETURN_MYSTRING_ERROR(comparator);
    int resultJPivot;
    while(1)
    {
        CHECK_NULL_RETURN_MYSTRING_ERROR(array[j]);
        resultJPivot = comparator(array[j], array[pivot]);
        CHECK_MYSTRING_ERROR_RETURN_MYSTRING_ERROR(resultJPivot);
        if (resultJPivot <= 0)
        {
            return j;
        }
        j--;
    }
}

static char * intToCharHelper(char * output, int n)
{
    CHECK_NULL_RETURN_NULL(output);
    if (n <= -10)
    {
        output = intToCharHelper(output, n/10);
        CHECK_NULL_RETURN_NULL(output);
    }
    int digit = - n%10;
    *output = ASCII_ZERO_DEC + digit;
    output++;
    return output;
}
// CR: dont use recursion
static int getTotalDigitsOfNumHelper(int n)
{
    if (n <= -10)
    {
        return getTotalDigitsOfNumHelper(n/10) + 1;
    }
    return 1;
}

static int getTotalDigitsOfNum(int n)
{
    int result = 0;
    if (n < 0)
    {
        result++;
    }
    else
    {
        n = -n;
    }
    return result + getTotalDigitsOfNumHelper(n);
}

// -------------------------- functions -------------------------

int charArrayLen(const char * cStr)
{
    CHECK_NULL_RETURN_MYSTRING_ERROR(cStr);
    const char * current = cStr;
    while (*current != '\0')
    {
        current++;
    }
    return (current - cStr) + NULL_CHAR_SIZE;
}

int charToInt(char * cStr, unsigned int size)
{
    CHECK_NULL_RETURN_MYSTRING_ERROR(cStr);
    if (size <= 0)
    {
        return MYSTRING_ERROR;
    }

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
        if (digit < 0 || 9 < digit)
        {
            return MYSTRING_ERROR;
        }
        double position = pow(10, decile);
        output += ((int)position) * digit;
        decile--;
        i++;
    }
    return output * sign;
}

char * intToChar(int n)
{
    int totalDigits = getTotalDigitsOfNum(n);
    char * output = (char *) malloc(sizeof (char)*totalDigits);
    char * op = output; // CR: dont cheap out on the variable names.
    if (n < 0)
    {
        *output = '-';
        op++;
    }
    else
    {
        n = -n;
    }
    op = intToCharHelper(op, n);
    *op = '\0';
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


// CR: do it without recursion.
void quicksortCharArraysUsingComp(MyString **arr, int (*comparator)(const void *, const void *),
                                  int first, int last)
{
    if (arr == NULL || comparator == NULL)
    {
        return;
    }

    int i, j, pivot;
    MyString * temp;

    if (first < last){
        pivot = first;
        i = first;
        j = last;

        while(i < j)
        {
            if (arr[i] == NULL || arr[j] == NULL || arr[pivot] == NULL)
            {
                return;
            }

            i = quicksortMoveRight(arr, comparator, i, pivot, last);
            if (i == MYSTRING_ERROR)
            {
                return;
            }

            j = quicksortMoveLeft(arr, comparator, j, pivot);
            if (j == MYSTRING_ERROR)
            {
                return;
            }

            if (i < j)
            {
                temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }

        temp = arr[pivot];
        arr[pivot] = arr[j];
        arr[j] = temp;

        quicksortCharArraysUsingComp(arr, comparator, first, j - 1);
        quicksortCharArraysUsingComp(arr, comparator, j + 1, last);
    }
}

unsigned int myCStringFilter(char *cStr, unsigned int strLength, char *output, FilterFunction *filterFunction)
{
    CHECK_NULL_RETURN_MYSTRING_ERROR(cStr);
    CHECK_NULL_RETURN_MYSTRING_ERROR(output);
    unsigned int newStrLength = 0;
    for (int i = 0; i < strLength; i++)
    {
        if (filterFunction(&cStr[i]) == false) // By returning false it means "keep this char"
        {
            output[newStrLength] = cStr[i];
            newStrLength++;
        }
    }
    return newStrLength;
}

int charCompare(const char ch1, const char ch2)
{
    return memcmp(&ch1, &ch2, 1);
}

char * getCStringBySize(unsigned int arraySize)
{
    char * output = (char *) malloc(sizeof(char)*(arraySize));
    CHECK_NULL_RETURN_NULL(output);
}