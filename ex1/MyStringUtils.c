#include "MyString.h"

int charArrayLen(char * str)
{
    if (str != NULL)
    {
        int n = 0;
        char * current = str;
        while (*current != '\0')
        {
            n++;
            current++;
        }
        return n;
    }
    return -1;
}

