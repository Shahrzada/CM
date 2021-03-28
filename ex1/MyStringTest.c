#include "MyString.h"
#include <time.h>


MyStringRetVal basicMyStringTest();

MyStringRetVal nullMyStringTest();

int main()
{
    MyStringRetVal result;

    result = basicMyStringTest();
    if (result == MYSTRING_ERROR)
    {
        printf("Failed the basic MyString tests");
    }

    result = nullMyStringTest();
    if (result == MYSTRING_ERROR)
    {
        printf("Failed the basic MyString tests");
    }

    return 0;
}

MyStringRetVal basicMyStringTest() {
    const char * cString = "I am a beautiful string";
    srand(time(NULL));
    int n = rand();

    int result;

    // initializing
    MyString * a = myStringAlloc();
    IF_NULL_RETURN_MYSTRING_ERROR(a);
    MyString * b = myStringAlloc();
    IF_NULL_RETURN_MYSTRING_ERROR(b);

    result = myStringSetFromInt(a, n);
    IF_MYSTRING_ERROR_RETURN_MYSTRING_ERROR(result);
    result = myStringSetFromCString(b, cString);
    IF_MYSTRING_ERROR_RETURN_MYSTRING_ERROR(result);

    // cloning and comparing
    MyString * c = myStringClone(a);
    IF_NULL_RETURN_MYSTRING_ERROR(c);
    MyString * d = myStringAlloc();
    IF_NULL_RETURN_MYSTRING_ERROR(d);
    result = myStringSetFromMyString(d, b);
    IF_MYSTRING_ERROR_RETURN_MYSTRING_ERROR(result);
    result = myStringEqual(a, c);
    if (result <= 0)
    {
        return MYSTRING_ERROR;
    }
    result = myStringEqual(b, d);
    if (result <= 0)
    {
        return MYSTRING_ERROR;
    }

    myStringFree(a);
    myStringFree(b);
    myStringFree(c);
    myStringFree(d);

    return MYSTRING_SUCCESS;
}

MyStringRetVal checkSingleMyStringInputFunction(const MyString *str,
                                                MyStringRetVal (*func)(const MyString *))
{
    return func(str);
}

MyStringRetVal nullMyStringTest() {
    const char * cString = "I am another beautiful string";
    srand(time(NULL));
    int n = rand();

    int result;

    // initializing
    MyString * a = NULL;
    MyString * b = myStringAlloc();
    IF_NULL_RETURN_MYSTRING_ERROR(b);

    result = myStringSetFromInt(a, n);
    if (result != MYSTRING_ERROR)
    {
        return MYSTRING_ERROR;
    }
    result = myStringSetFromCString(b, cString);
    IF_MYSTRING_ERROR_RETURN_MYSTRING_ERROR(result);

    // cloning and comparing
    MyString * c = myStringClone(a);
    if (c != NULL)
    {
        return MYSTRING_ERROR;
    }

    result = myStringSetFromMyString(a, c);
    if (result != MYSTRING_ERROR)
    {
        return MYSTRING_ERROR;
    }

    result = myStringEqual(a, c);
    if (result != MYSTR_ERROR_CODE)
    {
        return MYSTRING_ERROR;
    }

    result = myStringSetFromCString(b, NULL);
    if (result != MYSTRING_ERROR)
    {
        return MYSTRING_ERROR;
    }

    result = myStringSetFromCString(a, "abcde");
    if (result != MYSTRING_ERROR)
    {
        return MYSTRING_ERROR;
    }

    result = myStringToInt(a);
    if (result != MYSTRING_ERROR)
    {
        return MYSTRING_ERROR;
    }


    myStringFree(a);
    myStringFree(b);
    myStringFree(c);

    return MYSTRING_SUCCESS;
}

bool filterRemoveB(const char * ch)
{
    if (ch != NULL && *ch == 'b')
    {
        return true;
    }
    return false;
}

int comp(const char c1, const char c2)
{
    return 0;
}

//FILE * generateStream()
//{
//    FILE * pFile;
//    pFile = fopen("myfile.txt","w+");
//    if (pFile != NULL)
//    {
//        myStringWrite(a, pFile);
//        fclose (pFile);
//    }
//}
//

int testQuickSort()
{
    MyString * newline = myStringAlloc();
    myStringSetFromCString(newline, "\n");

    FILE * pFile;
    pFile = fopen("myfile.txt","w+");
    if (pFile == NULL)
    {
        return -1;
    }

    MyString ** arr = (MyString **) malloc(sizeof(MyString*)*4);
    char words[4][40] = {"d1234", "b1234", "c1234", "a1234"};

    for (int i = 0; i < 4; i++)
    {
        MyString * ms = myStringAlloc();
        myStringSetFromCString(ms, words[i]);
        myStringWrite(ms, pFile);
        myStringWrite(newline, pFile);
        *(arr + i) = ms;
    }

    myStringWrite(newline, pFile);

    myStringCustomSort(arr, 4, (int (*)(const void *, const void *)) myStringCompare);
    for (int i = 0; i < 4; i++)
    {
        myStringWrite(*(arr + i), pFile);
        myStringWrite(newline, pFile);
        free(*(arr + i));
    }

    fclose (pFile);
    myStringFree(newline);
    return 0;
}





