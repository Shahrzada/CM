
// ------------------------------ includes ------------------------------
// CR: I like these :D

#include "MyString.h"
#include "MyStringUtils.h"

// -------------------------- const definitions -------------------------
// CR: Why did you choose to put this here and not in the header file?
struct _MyString {
    int len; // CR: dont cheap out on the names... change len to length
    char * value;
};

// -------------------------- private functions -------------------------

// CR: This should've been a macro or an inline function
static MyStringRetVal myStringCheckNull(const MyString *str)
{
    IF_NULL_RETURN_MYSTRING_ERROR(str);
    IF_NULL_RETURN_MYSTRING_ERROR(str->value);
    return MYSTRING_SUCCESS;
}

static MyStringRetVal myStringArrayCheckNull(MyString **arr, unsigned int size)
{
    IF_NULL_RETURN_MYSTRING_ERROR(arr);
    for (int i = 0; i < size; i++)
    {
        if (myStringCheckNull(arr[i]) == MYSTRING_ERROR)
        {
            return MYSTRING_ERROR;
        }
    }
    return MYSTRING_SUCCESS;
}

// -------------------------- functions -------------------------

MyString *myStringAlloc()
{
    MyString * str = (MyString *) malloc(sizeof(MyString));
    IF_NULL_RETURN_NULL(str);

    str->value = (char *) malloc(sizeof(char));
    if (str->value == NULL)
    {
        // CR: In c, you should use goto for cleanup.
        free(str);
        return NULL;
    }
    
    str->len = 0;
    // CR: Do you really need this?
    *(str->value) = EMPTY_STR; 
    return str;
}

void myStringFree(MyString *str)
{
    IF_NULL_RETURN_NOTHING(str);

    if (str->value != NULL)
    {
        free(str->value);
    }

    free(str);
}

MyStringRetVal myStringSetFromMyString(MyString *str, const MyString *other)
{
    // CR: Send errors to the right, meaning let the if check for bad values 
    //     and if you find something, deal with in inside the if scope (it can reduce nesting)
    if (myStringCheckNull(str) == MYSTRING_SUCCESS &&
        myStringCheckNull(other) == MYSTRING_SUCCESS)
    {
        return myStringSetFromCString(str, other->value);
    }
    return MYSTRING_ERROR;
}

MyString *myStringClone(const MyString *str)
{
    if (myStringCheckNull(str) == MYSTRING_SUCCESS)
    {
        MyString * new_str = myStringAlloc();
        IF_NULL_RETURN_NULL(new_str);
        if (myStringSetFromMyString(new_str, str) == MYSTRING_SUCCESS)
        {
            return new_str;
        }
        // CR: this is a bit code dup
        free(new_str);
        return NULL;
    }
    return NULL;
}
// CR: filt is not really a good name a variable, dont cheap out on the name(filterFunc)
// CR: use a typedef for the filt, it would be more readable. filter_func_t 
MyStringRetVal myStringFilter(MyString *str, bool (*filt)(const char *))
{
    if (myStringCheckNull(str) == MYSTRING_SUCCESS)
    {
        // CR: this is dangurous, since you have a lot of malloc calls in youre code 
        //     and not a single place to unifity it, this can happen. 
        char * output = (char *) malloc(sizeof(char));
        IF_NULL_RETURN_MYSTRING_ERROR(output);

        char * current = str->value;
        int n = 0;

        while (*current != '\0') // CR: magic
        {
            if (filt(current) == false)
            {
                output[n] = *current;
                n++;
            }
            current++;
        }
        output[n] = '\0'; // CR: magic
        str->len = n;
        // CR: this should be preformed from a "private" function
        free(str->value);
        str->value = output;
        return MYSTRING_SUCCESS;
    }
    return MYSTRING_ERROR;
}

MyStringRetVal myStringSetFromCString(MyString *str, const char *cString)
{
    // CR: I wouldve like to this both of these in separate macro
    if (myStringCheckNull(str) == MYSTRING_SUCCESS && cString != NULL)
    {
        int n = charArrayLen(cString); // CR: Bad name bebi.
        if (n == MYSTRING_ERROR) // CR: macro?
        {
            return MYSTRING_ERROR;
        }
        free(str->value);
        // CR: +1? magic :)
        str->value = (char *) malloc(sizeof(char)*(n+1)); // CR: while this is a good practice, according to C ABI, sizeof(char)==?
        IF_NULL_RETURN_MYSTRING_ERROR(str->value);
        str->len = n;
        // CR: Why not copy +1?
        memcpy(str->value, cString, sizeof(char)*str->len); 
        str->value[n] = '\0'; // CR: magic :)
        return MYSTRING_SUCCESS;
    }
    return MYSTRING_ERROR;
}

MyStringRetVal myStringSetFromInt(MyString *str, int n)
{
    if (myStringCheckNull(str) == MYSTRING_SUCCESS)
    {
        char * output = intToChar(n);
        int len = charArrayLen(output);
        if (len == MYSTRING_ERROR)
        {
            // CR: use goto 
            free(output);
            return MYSTRING_ERROR;
        }
        free(str->value);
        str->value = output;
        str->len = len;
        return MYSTRING_SUCCESS;
    }
    return MYSTRING_ERROR;
}

int myStringToInt(const MyString *str)
{
    // CR: this is a good example of why you should change the condintions,
    //     where is the good flow and where is the bad flow?
    if (myStringCheckNull(str) == MYSTRING_SUCCESS)
    {
        if (str->len == 0)
        {
            return MYSTR_ERROR_CODE;
        }

        return charToInt(str->value, str->len);
    }
    return MYSTR_ERROR_CODE;
}

char *myStringToCString(const MyString *str)
{
    if (myStringCheckNull(str) == MYSTRING_SUCCESS)
    {
        // CR: off by one?
        char * output = (char *) malloc(sizeof(char)*str->len);
        IF_NULL_RETURN_NULL(output);
        memcpy(output, str->value, sizeof(char)*str->len);
        output[str->len] = '\0';
        return output;
    }
    return NULL;
}

MyStringRetVal myStringCat(MyString *dest, const MyString *src)
{
    if (myStringCheckNull(dest) == MYSTRING_SUCCESS &&
        myStringCheckNull(src) == MYSTRING_SUCCESS)
    {
        int new_len = dest->len + src->len;

        // if both are empty, there's nothing to do
        // CR: why not add private function for isNullOrEmpty?
        if (new_len == 0)
        {
            return MYSTRING_SUCCESS;
        }

        char * output = (char *) malloc(sizeof(char)*(new_len+1));
        IF_NULL_RETURN_MYSTRING_ERROR(output);

        memcpy(output, dest->value, sizeof(char)*dest->len);
        // CR: output_cat?
        char * output_cat = output + dest->len;
        memcpy(output_cat, src->value, sizeof(char)*src->len);

        free(dest->value);
        dest->value = output;
        dest->len = new_len;
        output[new_len] = '\0';
        return MYSTRING_SUCCESS;
    }
    return MYSTRING_ERROR;
}

MyStringRetVal myStringCatTo(const MyString *str1, const MyString *str2,  MyString *result)
{
    if (myStringCheckNull(str1) == MYSTRING_SUCCESS &&
        myStringCheckNull(str2) == MYSTRING_SUCCESS &&
        myStringCheckNull(result) == MYSTRING_SUCCESS)
    {
        MyStringRetVal retVal = myStringSetFromMyString(result, str1);
        if (retVal == MYSTRING_ERROR)
        {
            return MYSTRING_ERROR;
        }
        return myStringCat(result, str2);
    }
    return MYSTRING_ERROR;
}

int myStringCompare(const MyString *str1, const MyString *str2)
{
    if (myStringCheckNull(str1) == MYSTRING_SUCCESS &&
        myStringCheckNull(str2) == MYSTRING_SUCCESS &&
        str1->len > 0 && str2->len > 0) // CR: if both are empty why is it an ERROR?
    {
        return memcmp(str1->value, str2->value, str1->len);
    }
    return MYSTR_ERROR_CODE;
}
// CR: typedef comparator like filt(lol)
int myStringCustomCompare(const MyString *str1, const MyString *str2,
                          int (*comparator)(const char, const char))
{
    if (myStringCheckNull(str1) == MYSTRING_SUCCESS &&
        myStringCheckNull(str2) == MYSTRING_SUCCESS &&
        comparator != NULL &&
        str1->len == str2->len) // CR: same as the above.
    {
        int result; // CR: its a good practice to alway initalize your variables
        for (int i = 0; i < str1->len; i++)
        {
            result = comparator(*(str1->value + i), *(str2->value + i));
            if (result != 0)
            {
                return result;
            }
        }
        return 0;
    }
    return MYSTR_ERROR_CODE;
}

int myStringEqual(const MyString *str1, const MyString *str2)
{
    if (myStringCheckNull(str1) == MYSTRING_SUCCESS &&
        myStringCheckNull(str2) == MYSTRING_SUCCESS)
    {
        if (str1->len != str2->len)
        {
            return 0; // CR: 0 is good or bad?
        }
        int result = myStringCompare(str1, str2);
        if (result == 0)
        {
            return 1; // CR: 1 is good or bad?
        }
        else if (result == MYSTR_ERROR_CODE)
        {
            return result;
        }
        return 0;

    }
    return MYSTR_ERROR_CODE;
}
// CR: seems like a bit duplication from the code above.
int myStringCustomEqual(const MyString *str1, const MyString *str2,
                        int (*comparator)(const char, const char))
{
    if (myStringCheckNull(str1) == MYSTRING_SUCCESS &&
        myStringCheckNull(str2) == MYSTRING_SUCCESS &&
        comparator != NULL)
    {
        if (str1->len != str2->len)
        {
            return 0;
        }
        int result = myStringCustomCompare(str1, str2, comparator);
        if (result == 0)
        {
            return 1;
        }
        else if (result == MYSTR_ERROR_CODE)
        {
            return result;
        }
        return 0;

    }
    return MYSTR_ERROR_CODE;
}

unsigned long myStringMemUsage(const MyString *str1)
{
    if (myStringCheckNull(str1) == MYSTRING_SUCCESS)
    {
        return sizeof(MyString) + sizeof(char)*str1->len;
    }
    return 0;
}

unsigned long myStringLen(const MyString *str1)
{
    if (myStringCheckNull(str1) == MYSTRING_SUCCESS)
    {
        return str1->len;
    }
    return 0;
}

MyStringRetVal myStringWrite(const MyString *str, FILE *stream)
{
    if (myStringCheckNull(str) == MYSTRING_SUCCESS &&
        stream != NULL)
    {
        // CR: what does fputs return? did you check?
        int result = fputs(str->value, stream);
        if (result < 0 || result == EOF)
        {
            return MYSTRING_ERROR;
        }
        return MYSTRING_SUCCESS;
    }
    return MYSTRING_ERROR;
}

void myStringCustomSort(MyString **arr, unsigned int size,
                        int (*comparator)(const void *, const void *))
{
    if (myStringArrayCheckNull(arr, size) == MYSTRING_SUCCESS &&
        comparator != NULL && size > 1)
    {
        quicksortCharArraysUsingComp(arr, comparator, 0, (int) size - 1);
    }
}

void myStringSort(MyString **arr, unsigned int len)
{
    if (myStringArrayCheckNull(arr, len) == MYSTRING_SUCCESS && len > 1)
    {
        // CR: nice, but this shouldve been your clue to use typedef :D
        quicksortCharArraysUsingComp(arr, (int (*)(const void *, const void *)) myStringCompare, 0, (int) len - 1);
    }
}

MyString ** getArrayOfMyStringByLen(int n)
{
    if (n > 0)
    {
        MyString **arr = (MyString **) malloc(sizeof(MyString)*n);
        for (int i = 0; i < n; i++)
        {
            MyString * a = myStringAlloc();

            if (a != NULL)
            {
                // CR: this is kind of worrying, Im not really sure about struct assignment.
                //     I mean its not a problem here. but can you think of a problem?
                *(arr+ i) = a; // CR: there is a better way to write this
            }
            else
            {
                // CR: nice catch.
                freeArrayOfMyStringByLen(arr, i);
                return NULL;
            }
        }
        return arr;
    }
    return NULL;
}

void freeArrayOfMyStringByLen(MyString ** arr, int n)
{
    if (arr != NULL)
    {
        for (int i = 0; i < n; i++)
        {
            myStringFree(*(arr+ i));// CR: there is a better way to write this
        }
        free(arr);
    }
}
