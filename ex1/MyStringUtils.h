#ifndef MYSTRINGUTILS_H
#define MYSTRINGUTILS_H

/* Note: Do not start the guard symbol with an underscore! Leading underscore
 * names are reserved for internal use by the C implementation – the
 * preprocessor, compiler, and Standard Library – breaking this rule can
 * cause unnecessary and very puzzling errors.
 * From http://www.umich.edu/~eecs381/handouts/CHeaderFileGuidelines.pdf? */

// ------------------------------ includes ------------------------------

#include "MyString.h"
#include <math.h>

// -------------------------- const definitions -------------------------

#define ASCII_ZERO_DEC 48
#define EMPTY_STR '\0'
#define MINUS_SYMBOL '-'

// ------------------------------ macros -----------------------------

#define IF_NULL_RETURN_NULL(val) if (val == NULL) return NULL
#define IF_NULL_RETURN_NOTHING(val) if (val == NULL) return

// ------------------------------ functions -----------------------------

/**
 * @brief returns the length of a char array
 * @param str
 *
 * @return the length of the char array, -1 if str is NULL
 */
int charArrayLen(const char * str);

/**
 * @brief Returns the value of the char array as an integer.
 * @param ch
 * @param size
 *
 * @return an integer
 */
int charToInt(char * ch, unsigned int size);

/**
 * @brief Returns the value of an int as a char array, terminated with the
 * 	null character.
 * @param ch, will hold that output char array
 * @param n
 *
 */
char * intToChar(int n);

/**
 * @brief Applies the quicksort algorithm to sort an array of char arrays using
 * a comparatorReverse
 * @param arr
 * @param comparator
 * @param first
 * @param last
 *
 */
void quicksortCharArraysUsingComp(MyString **arr, int (*comparator)(const void *, const void *),
                                  int first, int last);

#endif // MYSTRINGUTILS_H