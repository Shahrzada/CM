//#include "MyString.h"
//#include "MyStringMacros.h"
//
//#define IS_SMALLER " is smaller than "
//
//enum ARGUMENTS{STR1_INPUT = 1, STR2_INPUT = 2, TOTAL_ARGS, TOTAL_MYSTRING_OBJECTS};
//
//MyStringRetVal validateInput(int argc, char *argv[]);
//MyStringRetVal initMyStrings(MyString **arr, int len, char *argv[]);
//MyStringRetVal compareMyStringsAndCreateOutput(MyString **arr);
//MyStringRetVal createMyStringOutput(MyString * output, MyString * left, MyString * middle, MyString * right);
//MyStringRetVal openAndWriteToFile(MyString **arr, int len);
//
//int main (int argc, char *argv[])
//{
//    if (validateInput(argc, argv) == MYSTRING_ERROR)
//    {
//        return MYSTRING_ERROR;
//    }
//
//    MyStringRetVal result;
//
//    // create MyString objects
//    MyString **arr = allocateMyStringArrayBySize(TOTAL_MYSTRING_OBJECTS);
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
//    if (myStringArrayCheckNull(arr, TOTAL_MYSTRING_OBJECTS) == MYSTRING_ERROR)
//    {
//        printf("Couldn't init the strings array.\n");
//        freeArrayOfMyStringBySize(arr, TOTAL_MYSTRING_OBJECTS);
//        return MYSTRING_ERROR;
//    }
//
//    MyStringRetVal result;
//    MyString * str1 = arr[0];
//    MyString * str2 = arr[1];
//    MyString * middle = arr[2];
//    MyString * output = arr[3];
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
//
//    if (argc != TOTAL_ARGS)
//    {
//        printf("Bebi, you know you need to enter exactly two strings :)\nUsage: bebi <str1> <str2>\n");
//        return MYSTRING_ERROR;
//    }
//
//    char * str1 = argv[STR1_INPUT];
//    char * str2 = argv[STR2_INPUT];
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
//    if (myStringArrayCheckNull(arr, len) == MYSTRING_ERROR)
//    {
//        printf("Couldn't init the strings array.\n");
//        freeArrayOfMyStringBySize(arr, len);
//        return MYSTRING_ERROR;
//    }
//
//    // Assign values
//    MyStringRetVal result1 = myStringSetFromCString(arr[0], argv[1]);
//    MyStringRetVal result2 = myStringSetFromCString(arr[1], argv[2]);
//    MyStringRetVal resultOutput = myStringSetFromCString(arr[2], IS_SMALLER);
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
//    CHECK_MYSTRING_ERROR_RETURN_MYSTRING_ERROR(result);
//
//    result = myStringConcat(output, middle);
//    CHECK_MYSTRING_ERROR_RETURN_MYSTRING_ERROR(result);
//
//    result = myStringConcat(output, right);
//    CHECK_MYSTRING_ERROR_RETURN_MYSTRING_ERROR(result);
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
//    FILE_METHOD * pFile;
//
//    pFile = fopen("test.out","w+");
//    if (pFile == NULL)
//    {
//        printf("Couldn't create the file.\n");
//        freeArrayOfMyStringBySize(arr, len);
//        return MYSTRING_ERROR;
//    }
//
//    result = myStringWriteStrValueToStream(output, pFile);
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