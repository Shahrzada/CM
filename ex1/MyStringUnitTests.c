#include "MyString.h"


bool filterRemoveB(const char * ch)
{
    if (ch != NULL && *ch == 'b')
    {
        return true;
    }
    return false;
}
