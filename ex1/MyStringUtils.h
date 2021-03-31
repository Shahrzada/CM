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
 * @param str
 *
 * @return the length of the char array, -1 if str is NULL
 */
int charArrayLen(const char * str);

/**
 * @brief Returns the value of the char array as an integer.
 * @param cString
 * @param size
 *
 * @return an integer
 */
int charToInt(char * cString, unsigned int size);

/**
 * @brief Returns the value of an int as a char array, terminated with the
 * 	null character.
 * @param ch, will hold that output char array
 * @param n
 *
 */
char * intToChar(int n);


char * charConcat(const char * cStr1, int cStr1Length, const char * cStr2, int cStr2Length);

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

