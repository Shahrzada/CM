//#include "MyString.h"
//#include "MyStringMacros.h"
//
//#define EXACT_AMOUNT_OF_INPUT_PARAMS 3
//#define TOTAL_MYSTRING_OBJECTS 4
//#define IS_SMALLER " is smaller than "
//
//MyStringRetVal validateInput(int argc, char *argv[]);
//MyStringRetVal initMyStrings(MyString **arr, int len, char *argv[]);
//MyStringRetVal compareMyStringsAndCreateOutput(MyString **arr);
//MyStringRetVal createMyStringOutput(MyString * output, MyString * left, MyString * middle, MyString * right);
//MyStringRetVal openAndWriteToFile(MyString **arr, int len);
//
//int main (int argc, char *argv[])
//{
//    MyString * a = myStringAlloc();
//    CHECK_NULL_RETURN_MYSTRING_ERROR(a);
//    myStringSetFromInt(a, 123);
//
//
//
//
//
//    if (validateInput(argc, argv) == MYSTRING_ERROR)
//    {
//        return MYSTRING_ERROR;
//    }
//
//    MyStringRetVal result;
//
//    // create MyString objects
//    MyString **arr = getArrayOfMyStringBySize(TOTAL_MYSTRING_OBJECTS);
//    if (arr == NULL)
//    {
//        printf("Couldn't init the strings.\n");
//    }
//
//    result = initMyStrings(arr, TOTAL_MYSTRING_OBJECTS, argv);
//    if (result == MYSTRING_ERROR)
//    {
//        printf("Couldn't init the strings.\n");
//        freeArrayOfMyStringBySize(arr, TOTAL_MYSTRING_OBJECTS);
//        return MYSTRING_ERROR;
//    }
//
//    // compare and create the relevant output msg
//    result = compareMyStringsAndCreateOutput(arr);
//    if (result == MYSTRING_ERROR)
//    {
//        printf("Couldn't compare the strings.\n");
//        freeArrayOfMyStringBySize(arr, TOTAL_MYSTRING_OBJECTS);
//        return MYSTRING_ERROR;
//    }
//
//    // open and write to file
//    result = openAndWriteToFile(arr, TOTAL_MYSTRING_OBJECTS);
//    freeArrayOfMyStringBySize(arr, TOTAL_MYSTRING_OBJECTS);
//
//    if (result == MYSTRING_ERROR)
//    {
//        return MYSTRING_ERROR;
//    }
//    return MYSTRING_SUCCESS;
//}
//
//MyStringRetVal compareMyStringsAndCreateOutput(MyString **arr) {
//    if (arr == NULL)
//    {
//        printf("Null pointer error.\n");
//        return MYSTRING_ERROR;
//    }
//
//    MyStringRetVal result;
//    MyString * str1 = *(arr);
//    MyString * str2 = *(arr + 1);
//    MyString * middle = *(arr + 2);
//    MyString * output = *(arr + 3);
//
//    result = myStringCompare(str1, str2);
//    if (result == MYSTRING_ERROR)
//    {
//        printf("Couldn't compare the strings.\n");
//        return MYSTRING_ERROR;
//    }
//    else if (result <= 0) // str1 <= str2
//    {
//        return createMyStringOutput(output, str1, middle, str2);
//    }
//    else // str2 < str1
//    {
//        return createMyStringOutput(output, str2, middle, str1);
//    }
//}
//
//MyStringRetVal validateInput(int argc, char *argv[])
//{
//    if (argv == NULL)
//    {
//        printf("Didn't know argv can be NULL.\n");
//        return MYSTRING_ERROR;
//    }
//    // CR: lol, use enum for these :D
//    if (argc != EXACT_AMOUNT_OF_INPUT_PARAMS)
//    {
//        printf("Bebi, you know you need to enter exactly two strings :)\nUsage: bebi <str1> <str2>\n");
//        return MYSTRING_ERROR;
//    }
//
//    // CR: in the same enum as the above.
//    char * str1 = argv[1];
//    char * str2 = argv[2];
//
//    if (str1 == NULL || str2 == NULL)
//    {
//        printf("Got NULL pointers as input.\n");
//        return MYSTRING_ERROR;
//    }
//
//    return MYSTRING_SUCCESS;
//}
//
//MyStringRetVal initMyStrings(MyString **arr, int len, char *argv[])
//{
//    if (arr == NULL)
//    {
//        printf("Couldn't allocate memory for MyStrings.\n");
//        return MYSTRING_ERROR;
//    }
//
//    for (int i = 0; i < len; i++)
//    {
//        if (*(arr + i) == NULL)
//        {
//            printf("Couldn't allocate memory for MyStrings.\n");
//            freeArrayOfMyStringBySize(arr, len);
//            return MYSTRING_ERROR;
//        }
//    }
//
//    // Assign values
//    MyString * str1 = *(arr);
//    MyString * str2 = *(arr + 1);
//    MyString * middle = *(arr + 2);
//
//    MyStringRetVal result1 = myStringSetFromCString(str1, argv[1]);
//    MyStringRetVal result2 = myStringSetFromCString(str2, argv[2]);
//    MyStringRetVal resultOutput = myStringSetFromCString(middle, IS_SMALLER);
//
//    if (result1 == MYSTRING_ERROR || result2 == MYSTRING_ERROR || resultOutput == MYSTRING_ERROR)
//    {
//        printf("myStringSetFromCString failed.\n");
//        freeArrayOfMyStringBySize(arr, len);
//        return MYSTRING_ERROR;
//    }
//
//    return MYSTRING_SUCCESS;
//}
//
//MyStringRetVal createMyStringOutput(MyString * output, MyString * left, MyString * middle, MyString * right)
//{
//    if (output == NULL || left == NULL || middle == NULL || right == NULL)
//    {
//        return MYSTRING_ERROR;
//    }
//
//    MyStringRetVal result;
//    result = myStringSetFromMyString(output, left);
//    IF_MYSTRING_ERROR_RETURN_MYSTRING_ERROR(result);
//
//    result = myStringConcatToFirst(output, middle);
//    IF_MYSTRING_ERROR_RETURN_MYSTRING_ERROR(result);
//
//    result = myStringConcatToFirst(output, right);
//    IF_MYSTRING_ERROR_RETURN_MYSTRING_ERROR(result);
//
//    return MYSTRING_SUCCESS;
//}
//
//MyStringRetVal openAndWriteToFile(MyString **arr, int len)
//{
//    if (arr == NULL || *(arr + 3) == NULL)
//    {
//        printf("Got a NULL pointer.\n");
//        return MYSTRING_ERROR;
//    }
//
//    MyString * output = *(arr + 3);
//    MyStringRetVal result;
//    FILE * pFile;
//
//    pFile = fopen("test.out","w+");
//    if (pFile == NULL)
//    {
//        printf("Couldn't create the file.\n");
//        freeArrayOfMyStringBySize(arr, len);
//        return MYSTRING_ERROR;
//    }
//
//    result = myStringWrite(output, pFile);
//    if (result == MYSTRING_ERROR)
//    {
//        printf("Couldn't write to file.\n");
//        fclose(pFile);
//        freeArrayOfMyStringBySize(arr, len);
//        return MYSTRING_ERROR;
//    }
//
//    fclose(pFile);
//    return MYSTRING_SUCCESS;
//}