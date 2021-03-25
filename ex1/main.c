#include "MyString.h"



int main()
{
    MyString * a = myStringAlloc();
    MyString * b = myStringAlloc();

    myStringSetValue(a, "abc");

//    MyString * b = myStringClone(a);

//    int i = myStringSetFromMyString(b, a);

    myStringFilter(a, filterRemoveB);



    myStringFree(a);
    myStringFree(b);
    return 0;
}