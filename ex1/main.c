#include "MyString.h"




int main()
{
    const char * cString = "I am a c string";
    int n = -4852;
    MyString * a = myStringAlloc();
    MyString * b = myStringAlloc();
    MyString * c = myStringAlloc();

    myStringSetFromCString(a, "abc");
    myStringSetFromCString(b, "abd");

//    MyString * b = myStringClone(a);

//    int i = myStringSetFromMyString(b, a);

//    myStringFilter(a, filterRemoveB);

//    myStringSetFromInt(a, n);

//    int bb = myStringToInt(b);

//    char * p = myStringToCString(a);

//    int aa = myStringCat(a, b);

//    int aa = myStringCatTo(a, b, c);

//    int aa = myStringCompare(a, b);

//    int aa = myStringCustomCompare(a, b, comp);




    int aaaaa = 0;

    myStringFree(a);
    myStringFree(b);
    myStringFree(c);
    return 0;
}