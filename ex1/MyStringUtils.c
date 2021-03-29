
// ------------------------------ includes ------------------------------

#include "MyStringUtils.h"

// -------------------------- private functions -------------------------

static int quicksortMoveRight(MyString **arr, int (*comparator)(const void *, const void *),
                       int i, int pivot, unsigned int last)
{
    if (arr == NULL || comparator == NULL)
    {
        return MYSTR_ERROR_CODE;
    }

    int result_i_pivot;
    while(true)  // while (arr[i] <= arr[pivot] and i < last)
    {
        if (arr[i] == NULL)
        {
            return MYSTR_ERROR_CODE;
        }
        result_i_pivot = comparator(arr[i], arr[pivot]);
        if (result_i_pivot == MYSTR_ERROR_CODE)
        {
            return MYSTR_ERROR_CODE;
        }
        if (result_i_pivot > 0 || i >= last)
        {
            return i;
        }
        i++;
    }
}

static int quicksortMoveLeft(MyString **arr, int (*comparator)(const void *, const void *),
                      int j, int pivot)
{
    if (arr == NULL || comparator == NULL)
    {
        return MYSTR_ERROR_CODE;
    }

    int result_j_pivot;
    while (true)
    {
        if (arr[j] == NULL)
        {
            return MYSTR_ERROR_CODE;
        }
        result_j_pivot = comparator(arr[j], arr[pivot]);
        if (result_j_pivot == MYSTR_ERROR_CODE)
        {
            return MYSTR_ERROR_CODE;
        }
        if (result_j_pivot <= 0)
        {
            return j;
        }
        j--;
    }
}

static char * intToCharHelper(char * output, int n)
{
    IF_NULL_RETURN_NULL(output);
    if (n <= -10)
    {
        output = intToCharHelper(output, n/10);
        IF_NULL_RETURN_NULL(output);
    }
    int digit = - n%10;
    *output = ASCII_ZERO_DEC + digit;
    output++;
    return output;
}

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

int charArrayLen(const char * str)
{
    if (str != NULL)
    {
        int n = 0;
        const char * current = str;
        while (*current != '\0')
        {
            n++;
            current++;
        }
        return n;
    }
    return MYSTRING_ERROR;
}

int charToInt(char * ch, unsigned int size)
{
    if (ch != NULL && size > 0)
    {
        char * current = ch;
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
                return MYSTR_ERROR_CODE;
            }
            double position = pow(10, decile);
            output += ((int)position) * digit;
            decile--;
            i++;
        }
        return output * sign;
    }
    return MYSTR_ERROR_CODE;
}

char * intToChar(int n)
{
    int totalDigits = getTotalDigitsOfNum(n);
    char * output = (char *) malloc(sizeof (char)*totalDigits);
    char * op = output;
    if (n < 0)
    {
        *output = '-';
        output++;
    }
    else
    {
        n = -n;
    }
    op = intToCharHelper(op, n);
    *op = '\0';
    return output;
}

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
            if (i == MYSTR_ERROR_CODE)
            {
                return;
            }

            j = quicksortMoveLeft(arr, comparator, j, pivot);
            if (j == MYSTR_ERROR_CODE)
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
