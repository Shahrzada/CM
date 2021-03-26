#include "MyString.h"

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
    return -1;
}

char * intToCharHelper(char * output, int n)
{
    if (n <= -10)
    {
        output = intToCharHelper(output, n/10);
    }
    int digit = - n%10;
    *output = ASCII_ZERO_DEC + digit;
    output++;
    return output;
}

void intToChar(char * ch, int n)
{
    char * output = ch;
    if (n < 0)
    {
        *output = '-';
        output++;
    }
    else
    {
        n = -n;
    }
    output = intToCharHelper(output, n);
    *output = '\0';
}

