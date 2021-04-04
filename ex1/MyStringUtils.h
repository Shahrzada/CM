#pragma once

// CR_S: not only pragma once is slightly faster then good 'ol ifndef, it
//       also overcomes ifndef's bad guard names problems.
//       Although, pragma once can become an issue when the same file is
//       in different locations which make the compiler think these are
//       different files. That's not an issue in this ex.

// ------------------------------ includes ------------------------------

#include "MyString.h"
#include <math.h>

// -------------------------- const definitions -------------------------

#define ASCII_ZERO_DEC 48
#define MINUS_SYMBOL '-'

// ------------------------------ functions -----------------------------

/**
 * @brief returns the length of a char array
 * @param cStr
 *
 * @return the length of the char array, -1 if str is NULL
 */
int cStringLength(const char * str);

/**
 * @brief Returns the value of the char array as an integer.
 * @param cStr
 * @param size
 *
 * @return an integer
 */
int cStringToInt(char * cString, unsigned int size);

/**
 * @brief Returns the value of an int as a char array, terminated with the
 * 	null character.
 * @param ch, will hold that output char array
 * @param number
 *
 */
char * intToCString(int n);

/**
 * @brief Create a new cStr by appending a copy of cStr2 to cStr1
 * dst.
 * @param cStr1 to alter
 * @param cStr2 to append
 * RETURN VALUE:
 *  @return the new cStr on success, NULL on failure.
 */
char * charConcat(const char * cStr1, unsigned int cStr1Length, const char * cStr2, unsigned int cStr2Length);

/**
 * @brief filter the value of cStr according to a filter function.
 * @param cStr to filter
 * @param strLength
 * @param output - the cStr to contain the new filtered string
 * @param filterFunction
 * RETURN VALUE:
 *  @return new string size on success, MYSTRING_ERROR on failure. */
int myCStringFilter(char *cStr, unsigned int strLength, char *output, filter_function_t *filterFunction);

/**
 * @brief a wrapper for memcmp to compare two single chars, used as a simple comparator
 * @param ch1
 * @param ch2
 * RETURN VALUE:
 *  @return same return as memcmp(&ch1, &ch2, 1). */
int charCompare(const char ch1, const char ch2);

/**
 * @brief allocates a cStr by desired length
 * @param length
 * RETURN VALUE:
 *  @return same return as malloc for the given length. */
char * allocateCStringByLength(unsigned int arraySize);
