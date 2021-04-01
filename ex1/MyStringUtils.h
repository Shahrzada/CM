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

char * charConcat(const char * cStr1, unsigned int cStr1Length, const char * cStr2, unsigned int cStr2Length);

unsigned int myCStringFilter(char *cStr, unsigned int strLength, char *output, FilterFunction *filterFunction);

int charCompare(const char ch1, const char ch2);

char * allocateCStringByLength(unsigned int arraySize);