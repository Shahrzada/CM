#pragma once

/********************************************************************************
 * @file MyString.h
 * @author  slabc <slbac@cs.huji.ac.il>
 * @version 1.0
 * @date 9 Aug 2015
 *
 * @brief The SLabC Standard Strings library.
 *
 * @section LICENSE
 * This program is not a free software;
 *
 * @section DESCRIPTION
 * The LabC Standard Strings library.
 *
 * This library provides a string type with extra capabilities.
 *
 * The library provides the following features:
 *  - basic string operations
 *  - conversion to other types (ints, C strings)
 *
 * Error handling
 * ~~~~~~~~~~~~~~
 * Most functions may fail due to failure to allocate dynamic memory. When
 * this happens the functions will return an appropriate failure value. If this
 * happens, then the state of the other outputs of the function is undefined.
 ********************************************************************************/

// ------------------------------ includes ------------------------------

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// -------------------------- const definitions -------------------------

/*
 * MyString represents a manipulable string.
 */
struct _MyString;
typedef struct _MyString MyString;

/* Return values */
typedef enum {
  MYSTRING_ERROR = -999,
  MYSTRING_SUCCESS = 0,
} MyStringRetVal;

typedef bool (filter_function_t)(const char *);
typedef int (my_string_comparator_t)(const char, const char);
typedef int (my_string_sort_comparator_t)(const void *, const void *);

// ------------------------------ functions -----------------------------

/**
 * @brief Allocates a new MyString and sets its value to "" (the empty string).
 * 			It is the caller's responsibility to free the returned
 * MyString.
 *
 * RETURN VALUE:
 * @return a pointer to the new string, or NULL if the allocation failed.
 */
MyString *myStringAlloc();

/**
 * @brief Frees the memory and resources allocated to str.
 * @param str the MyString to free.
 * If str is NULL, no operation is performed.
 */
void myStringFree(MyString *str);

/**
 * @brief Allocates a new MyString with the same value as str. It is the
 * caller's responsibility to free the returned MyString.
 * @param str the MyString to clone.
 * RETURN VALUE:
 *   @return a pointer to the new string, or NULL if the allocation failed.
 */
MyString *myStringClone(const MyString *str);

/**
 * @brief Sets the value of str to the value of other.
 * @param str the MyString to set
 * @param other the MyString to set from
 * RETURN VALUE:
 *  @return MYSTRING_SUCCESS on success, MYSTRING_ERROR on failure.
 */
MyStringRetVal myStringSetFromMyString(MyString *str, const MyString *other);

/**
 * @brief filter the value of str according to a filter.
 * 	remove from str all the occurrence of chars that are filtered by filt
 *	(i.e. filr(char)==true)
 * @param str the MyString to filter
 * @param filt the filter
 * RETURN VALUE:
 *  @return MYSTRING_SUCCESS on success, MYSTRING_ERROR on failure. */
MyStringRetVal myStringFilter(MyString *str, bool (*filt)(const char *));

/**
 * @brief Sets the value of str to the value of the given C string.
 * 			The given C string must be terminated by the null
 * character. Checking will not use a string without the null character.
 * @param str the MyString to set.
 * @param cString the C string to set from.
 * RETURN VALUE:
 *  @return MYSTRING_SUCCESS on success, MYSTRING_ERROR on failure.
 */
MyStringRetVal myStringSetFromCString(MyString *str, const char *cString);

/**
 * @brief Sets the value of str to the value of the integer number.
 *	(i.e. if number=7 than str should contain ???7???)
 * 	You are not allowed to use itoa (or a similar functions) here but must
 *code your own conversion function.
 * @param str the MyString to set.
 * @param number the int to set from.
 * RETURN VALUE:
 *  @return MYSTRING_SUCCESS on success, MYSTRING_ERROR on failure.
 */
MyStringRetVal myStringSetFromInt(MyString *str, int number);

/**
 * @brief Returns the value of str as an integer.
 * 	If str cannot be parsed as an integer,
 * 	the return value should be MYSTR_ERROR_CODE
 * 	NOTE: positive and negative integers should be supported.
 * @param str the MyString
 * @return an integer
 */
int myStringToInt(const MyString *str);

/**
 * @brief Returns the value of str as a C string, terminated with the
 * 	null character. It is the caller's responsibility to free the returned
 * 	string by calling free().
 * @param str the MyString
 * RETURN VALUE:
 *  @return the new string, or NULL if the allocation failed.
 */
char *myStringToCString(const MyString *str);

/**
 * @brief Appends a copy of source to dest
 * dst.
 * @param dest to alter
 * @param source to append
 * RETURN VALUE:
 *  @return MYSTRING_SUCCESS on success, MYSTRING_ERROR on failure.
 */
MyStringRetVal myStringConcat(MyString *dest, const MyString *source);

/**
 * @brief Sets result to be the concatenation of str1 and str2.
 * 	result should be initially allocated by the caller.
 * 	result shouldn't be the same struct as str1 or str2.
 * @param str1
 * @param str2
 * @param result
 * RETURN VALUE:
 *  @return MYSTRING_SUCCESS on success, MYSTRING_ERROR on failure.
 */
MyStringRetVal myStringConcatToResult(const MyString *str1, const MyString *str2,
                                      MyString *result);

/**
 * @brief Compare str1 and str2.
 * @param str1
 * @param str2
 *
 * RETURN VALUE:
 * @return an integral value indicating the relationship between the strings:
 * 	A zero value indicates that the strings are equal.
 * 	A value greater than zero indicates that the first character that does
 * not match has a greater ASCII value in str1 than in str2; And a value less
 * than zero indicates the opposite. If strings cannot be compared, the return
 * value should be MYSTR_ERROR_CODE
 */
int myStringCompare(const MyString *str1, const MyString *str2);

/**
 * @brief Compares str1 and str2.
 * @param str1
 * @param str2
 * @param comparator
 * RETURN VALUE:
 * @return an integral value indicating the relationship between the strings:
 * 	A zero value indicates that the strings are equal according to the
 * custom comparatorReverse (3rd parameter). A value greater than zero indicates that
 * the first MyString is bigger according to the comparatorReverse. And a value less
 * than zero indicates the opposite. If strings cannot be compared, the return
 * value should be MYSTR_ERROR_CODE
 */

int myStringCustomCompare(const MyString *str1, const MyString *str2,
                          int (*comparator)(const char, const char));

/**
 * @brief Check if str1 is equal to str2.
 * @param str1
 * @param str2
 *
 * RETURN VALUE:
 * @return an integral value indicating the equality of the strings (logical
 * equality - that they are composed of the very same characters): A zero value
 * indicates that the strings are not equal. A greater than zero value indicates
 * that the strings are equal. If strings cannot be compared, the return value
 * should be MYSTR_ERROR_CODE
 */
int myStringEqual(const MyString *str1, const MyString *str2);

/**
 * @brief Check if str1 is equal to str2.
 * @param str1
 * @param str2
 * @param comparator
 *
 * RETURN VALUE:
 * @return an integral value indicating the equality of the strings using a
 * custom comparatorReverse (3rd parameter): A zero value indicates that the strings
 * are not equal. A greater than zero value indicates that the strings are
 * equal. If string cannot be compared, the return value should be
 * MYSTR_ERROR_CODE
 */
int myStringCustomEqual(const MyString *str1, const MyString *str2,
                        int (*comparator)(const char, const char));

/**
 * @return the amount of memory (all the memory that used by the MyString object
 * itself and its allocations), in bytes, allocated to str1.
 */
unsigned long getMyStringMemoryUsage(const MyString *str);

/**
 * @return the length of the string in str1.
 */
unsigned long getMyStringLength(const MyString *str);

/**
 * Writes the content of str to stream. (like fputs())
 *
 * RETURNS:
 *  @return MYSTRING_SUCCESS on success, MYSTRING_ERROR on failure.
 */
MyStringRetVal myStringWriteStrValueToStream(const MyString *str, FILE *stream);

/**
 * @brief sort an array of MyString pointers
 * @param arr
 * @param len
 * @param comparator custom comparatorReverse
 *
 * RETURN VALUE: MYSTRING_SUCCESS on success, MYSTRING_ERROR on failure.
 */
void myStringCustomSort(MyString **arr, unsigned int size,
                        int (*comparator)(const void *, const void *));

/**
 * @brief sorts an array of MyString pointers according to the default
 * comparison (like in myStringCompare)
 * @param arr
 * @param len
 *
 * RETURN VALUE: none
 */
void myStringSort(MyString **arr, unsigned int len);

/**
 * @brief creates an array of the desired arraySize of MyString objects with no value
 * (= empty string)
 * @param arraySize
 *
 * RETURN VALUE: a pointer to the array
 */
MyString ** allocateMyStringArrayBySize(unsigned int arraySize);

/**
 * @brief given a pointer to an array of MyString objects, it frees all of them and itself
 * @param array
 * @param arraySize
 *
 * RETURN VALUE: none
 */
void freeArrayOfMyStringBySize(MyString ** array, unsigned int arraySize);

/**
 * @brief given a pointer to an array of MyString objects, it verifies all MyStrings are not null
 * @param array
 * @param arraySize
 *
 * RETURN VALUE: MYSTRING_SUCCESS on success, MYSTRING_ERROR on failure.
 */
MyStringRetVal myStringArrayCheckNull(MyString **array, unsigned int arraySize);