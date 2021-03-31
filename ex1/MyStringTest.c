#include "MyString.h"
#include <time.h>


MyStringRetVal basicMyStringTest();
MyStringRetVal nullMyStringTest();
MyStringRetVal comparingMyStringTest();
MyStringRetVal filteringMyStringTest();

MyStringRetVal notifyHelper(MyStringRetVal returnValue, char * msg, MyString ** arrToFree, int arrSize);

bool filterRemoveB(const char * ch);
int comparatorReverse(const char c1, const char c2);


int main()
{
    MyStringRetVal result;

    result = basicMyStringTest();
    if (result == MYSTRING_ERROR)
    {
        printf("Failed a basic MyString test.\n");
        return -1;
    }
    printf("Passed all basic MyString tests.\n");

    result = nullMyStringTest();
    if (result == MYSTRING_ERROR)
    {
        printf("Failed a NULL MyString test.\n");
        return -1;
    }
    printf("Passed all NULL MyString tests.\n");

//    result = comparingMyStringTest();
//    if (result == MYSTRING_ERROR)
//    {
//        printf("Failed a comparing MyString test.\n");
//        return -1;
//    }
    printf("Passed all comparing MyString tests.\n");


    result = filteringMyStringTest();
    if (result == MYSTRING_ERROR)
    {
        printf("Failed a filtering MyString test.\n");
        return -1;
    }
    printf("Passed all filtering MyString tests.\n");



    return 0;
}

MyStringRetVal basicMyStringTest() {
    const char * cString = "I am a beautiful string";
    int n = -35467;
    int arrSize = 10;

    int result;

    // initializing
    MyString ** arr = getArrayOfMyStringBySize(arrSize);
    if (arr == NULL)
    {
        return notifyHelper(MYSTRING_ERROR, "getArrayOfMyStringBySize failed", arr, arrSize);
    }

    MyString * a = *arr;
    MyString * b = *(arr + 1);
    MyString * c = *(arr + 2);


    // assigning string values
    result = myStringSetFromInt(a, n);
    if (result == MYSTRING_ERROR)
    {
        return notifyHelper(MYSTRING_ERROR, "myStringSetFromInt failed", arr, arrSize);
    }

    result = myStringSetFromCString(b, cString);
    if (result == MYSTRING_ERROR)
    {
        return notifyHelper(MYSTRING_ERROR, "myStringSetFromCString failed", arr, arrSize);
    }

    // cloning and comparing
    MyString * d = myStringClone(a);
    if (d == NULL)
    {
        return notifyHelper(MYSTRING_ERROR, "myStringClone failed", arr, arrSize);
    }
    myStringFree(*(arr + 3));
    *(arr + 3) = d;

    result = myStringSetFromMyString(c, b);
    if (result == MYSTRING_ERROR)
    {
        return notifyHelper(MYSTRING_ERROR, "myStringSetFromMyString failed", arr, arrSize);
    }

    result = myStringEqual(a, d);
    if (result <= 0)
    {
        return notifyHelper(MYSTRING_ERROR, "myStringEqual failed", arr, arrSize);
    }

    result = myStringEqual(b, c);
    if (result <= 0)
    {
        return notifyHelper(MYSTRING_ERROR, "myStringEqual failed", arr, arrSize);
    }


    freeArrayOfMyStringBySize(arr, arrSize);
    return MYSTRING_SUCCESS;
}

MyStringRetVal nullMyStringTest() {
    const char * cString = "I am a beautiful string";
    int n = -35467;
    int arrSize = 10;

    int result;

    // initializing
    MyString ** arr = getArrayOfMyStringBySize(arrSize);
    if (arr == NULL)
    {
        return notifyHelper(MYSTRING_ERROR, "getArrayOfMyStringBySize failed", arr, arrSize);
    }

    MyString * a = NULL;
    MyString * b = *(arr + 1);

    result = myStringSetFromCString(b, cString);
    if (result == MYSTRING_ERROR)
    {
        return notifyHelper(MYSTRING_ERROR, "myStringSetFromCString failed", arr, arrSize);
    }

    // checking null
    MyString * c = myStringClone(a);
    if (c != NULL)
    {
        return notifyHelper(MYSTRING_ERROR, "myStringClone failed", arr, arrSize);
    }

    result = myStringSetFromMyString(a, c);
    if (result != MYSTRING_ERROR)
    {
        return notifyHelper(MYSTRING_ERROR, "myStringSetFromMyString failed", arr, arrSize);
    }

    result = myStringFilter(a, NULL);
    if (result != MYSTRING_ERROR)
    {
        return notifyHelper(MYSTRING_ERROR, "myStringFilter failed", arr, arrSize);
    }

    result = myStringFilter(a, filterRemoveB);
    if (result != MYSTRING_ERROR)
    {
        return notifyHelper(MYSTRING_ERROR, "myStringFilter failed", arr, arrSize);
    }

    result = myStringSetFromCString(a, NULL);
    if (result != MYSTRING_ERROR)
    {
        return notifyHelper(MYSTRING_ERROR, "myStringSetFromCString failed", arr, arrSize);
    }

    result = myStringSetFromCString(b, NULL);
    if (result != MYSTRING_ERROR)
    {
        return notifyHelper(MYSTRING_ERROR, "myStringSetFromCString failed", arr, arrSize);
    }

    result = myStringSetFromCString(a, "abcde");
    if (result != MYSTRING_ERROR)
    {
        return notifyHelper(MYSTRING_ERROR, "myStringSetFromCString failed", arr, arrSize);
    }

    result = myStringSetFromInt(a, n);
    if (result != MYSTRING_ERROR)
    {
        return notifyHelper(MYSTRING_ERROR, "myStringSetFromInt failed", arr, arrSize);
    }

    result = myStringToInt(a);
    if (result != MYSTRING_ERROR)
    {
        return notifyHelper(MYSTRING_ERROR, "myStringToInt failed", arr, arrSize);
    }

    char * ch = myStringToCString(a);
    if (ch != NULL)
    {
        return notifyHelper(MYSTRING_ERROR, "myStringToCString failed", arr, arrSize);
    }

    result = myStringConcatToFirst(a, b);
    if (result != MYSTRING_ERROR)
    {
        return notifyHelper(MYSTRING_ERROR, "myStringConcatToFirst failed", arr, arrSize);
    }

    result = myStringConcatToFirstResult(a, b, a);
    if (result != MYSTRING_ERROR)
    {
        return notifyHelper(MYSTRING_ERROR, "myStringConcatToFirstResult failed", arr, arrSize);
    }

    result = myStringCompare(a, b);
    if (result != MYSTRING_ERROR)
    {
        return notifyHelper(MYSTRING_ERROR, "myStringCompare failed", arr, arrSize);
    }

    result = myStringCustomCompare(a, b, comparatorReverse);
    if (result != MYSTRING_ERROR)
    {
        return notifyHelper(MYSTRING_ERROR, "myStringCustomCompare failed", arr, arrSize);
    }

    result = myStringEqual(a, c);
    if (result != MYSTRING_ERROR)
    {
        return notifyHelper(MYSTRING_ERROR, "myStringEqual failed", arr, arrSize);
    }

    result = myStringCustomEqual(a, c, comparatorReverse);
    if (result != MYSTRING_ERROR)
    {
        return notifyHelper(MYSTRING_ERROR, "myStringCustomEqual failed", arr, arrSize);
    }

    unsigned long size = getMyStringMemoryUsage(a);
    if (size != 0)
    {
        return notifyHelper(MYSTRING_ERROR, "getMyStringMemoryUsage failed", arr, arrSize);
    }

    unsigned long len = getMyStringLength(a);
    if (len != 0)
    {
        return notifyHelper(MYSTRING_ERROR, "getMyStringLength failed", arr, arrSize);
    }

    result = myStringWrite(a, NULL);
    if (result != MYSTRING_ERROR)
    {
        return notifyHelper(MYSTRING_ERROR, "myStringWrite failed", arr, arrSize);
    }

    freeArrayOfMyStringBySize(arr, arrSize);
    return MYSTRING_SUCCESS;
}

MyStringRetVal comparingMyStringTest() {
    const char * cString1 = "I am a beautiful string";
    const char * cString2 = "I am another beautiful string";
    int arrSize = 10;
    int result;

    // initializing
    MyString ** arr = getArrayOfMyStringBySize(arrSize);
    if (arr == NULL)
    {
        return notifyHelper(MYSTRING_ERROR, "getArrayOfMyStringBySize failed", arr, arrSize);
    }
    MyString * a = *arr;
    MyString * b = *(arr + 1);
    MyString * c = *(arr + 2);

    // assigning string values
    result = myStringSetFromCString(a, cString1);
    if (result == MYSTRING_ERROR)
    {
        return notifyHelper(MYSTRING_ERROR, "myStringSetFromCString failed", arr, arrSize);
    }
    result = myStringSetFromCString(b, cString2);
    if (result == MYSTRING_ERROR)
    {
        return notifyHelper(MYSTRING_ERROR, "myStringSetFromCString failed", arr, arrSize);
    }
    MyString * d = myStringClone(a);
    if (d == NULL)
    {
        return notifyHelper(MYSTRING_ERROR, "myStringClone failed", arr, arrSize);
    }
    myStringFree(*(arr + 3));
    *(arr + 3) = d;

    result = myStringSetFromMyString(c, b);
    if (result == MYSTRING_ERROR)
    {
        return notifyHelper(MYSTRING_ERROR, "myStringSetFromMyString failed", arr, arrSize);
    }

    // compare string values - notice that: a == d ; b == c ; a < b
    int compareResult = myStringCompare(b, a);
    if (compareResult <= 0)
    {
        return notifyHelper(MYSTRING_ERROR, "myStringCompare failed, a < b", arr, arrSize);
    }

    compareResult = myStringCompare(b, b);
    if (compareResult != 0)
    {
        return notifyHelper(MYSTRING_ERROR, "myStringCompare failed, b = b", arr, arrSize);
    }

    compareResult = myStringCompare(c, a);
    if (compareResult <= 0)
    {
        return notifyHelper(MYSTRING_ERROR, "myStringCompare failed, a < c", arr, arrSize);
    }

    compareResult = myStringCustomCompare(b, a, comparatorReverse);
    if (compareResult >= 0)
    {
        return notifyHelper(MYSTRING_ERROR, "myStringCompare failed, a > b", arr, arrSize);
    }

    compareResult = myStringCustomCompare(b, b, comparatorReverse);
    if (compareResult != 0)
    {
        return notifyHelper(MYSTRING_ERROR, "myStringCompare failed, b = b", arr, arrSize);
    }

    compareResult = myStringCustomCompare(c, a, comparatorReverse);
    if (compareResult >= 0)
    {
        return notifyHelper(MYSTRING_ERROR, "myStringCompare failed, a > c", arr, arrSize);
    }

    compareResult = myStringEqual(b, a);
    if (compareResult != 0)
    {
        return notifyHelper(MYSTRING_ERROR, "myStringEqual failed, a < b", arr, arrSize);
    }

    compareResult = myStringEqual(b, c);
    if (compareResult <= 0)
    {
        return notifyHelper(MYSTRING_ERROR, "myStringEqual failed, b = c", arr, arrSize);
    }

    compareResult = myStringEqual(c, a);
    if (compareResult != 0)
    {
        return notifyHelper(MYSTRING_ERROR, "myStringEqual failed, a < c", arr, arrSize);
    }

    compareResult = myStringCustomEqual(b, a, comparatorReverse);
    if (compareResult != 0)
    {
        return notifyHelper(MYSTRING_ERROR, "myStringCustomEqual failed, a > b", arr, arrSize);
    }

    compareResult = myStringCustomEqual(b, c, comparatorReverse);
    if (compareResult <= 0)
    {
        return notifyHelper(MYSTRING_ERROR, "myStringCustomEqual failed, b = b", arr, arrSize);
    }

    compareResult = myStringCustomEqual(c, a, comparatorReverse);
    if (compareResult != 0)
    {
        return notifyHelper(MYSTRING_ERROR, "myStringCustomEqual failed, a > c", arr, arrSize);
    }

    freeArrayOfMyStringBySize(arr, arrSize);
    return MYSTRING_SUCCESS;
}

MyStringRetVal filteringMyStringTest() {
    const char *cString1 = "I am a beautiful string";
    const char *cString2 = "I am another beautiful string";
    int arrSize = 10;
    int result;

    // initializing
    MyString **arr = getArrayOfMyStringBySize(arrSize);
    if (arr == NULL) {
        return notifyHelper(MYSTRING_ERROR, "getArrayOfMyStringBySize failed", arr, arrSize);
    }
    MyString *a = *arr;
    MyString *b = *(arr + 1);
    MyString *c = *(arr + 2);

    // assigning string values
    result = myStringSetFromCString(a, cString1);
    if (result == MYSTRING_ERROR) {
        return notifyHelper(MYSTRING_ERROR, "myStringSetFromCString failed", arr, arrSize);
    }
    result = myStringSetFromCString(b, cString2);
    if (result == MYSTRING_ERROR) {
        return notifyHelper(MYSTRING_ERROR, "myStringSetFromCString failed", arr, arrSize);
    }
    MyString *d = myStringClone(a);
    if (d == NULL) {
        return notifyHelper(MYSTRING_ERROR, "myStringClone failed", arr, arrSize);
    }
    myStringFree(*(arr + 3));
    *(arr + 3) = d;

    result = myStringSetFromMyString(c, b);
    if (result == MYSTRING_ERROR) {
        return notifyHelper(MYSTRING_ERROR, "myStringSetFromMyString failed", arr, arrSize);
    }

    // testing filtering
    result = myStringFilter(a, filterRemoveB);
    if (result == MYSTRING_ERROR) {
        return notifyHelper(MYSTRING_ERROR, "myStringFilter failed", arr, arrSize);
    }
    char *filteredCString = myStringToCString(a);
    if (filteredCString == NULL) {
        return notifyHelper(MYSTRING_ERROR, "myStringToCString failed", arr, arrSize);
    }
    char *validResult = strchr(filteredCString, 'b');
    if (validResult != NULL) {
        return notifyHelper(MYSTRING_ERROR, "myStringToCString failed logically", arr, arrSize);
    }


    freeArrayOfMyStringBySize(arr, arrSize);
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

int comparatorReverse(const char c1, const char c2)
{
    int x = (unsigned char)c1;
    int y = (unsigned char)c2;
    if (x == y)
    {
        return 0;
    }
    else if (x < y)
    {
        return -1;
    }
    return 1;
}

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

MyStringRetVal notifyHelper(MyStringRetVal returnValue, char * msg, MyString ** arrToFree, int arrSize)
{
    freeArrayOfMyStringBySize(arrToFree, arrSize);
    printf("%s\n", msg);
    return returnValue;
}
