//
// Created by jasper on 03.02.23.
//

#ifndef PROJEKT_STRING_UTILS_H
#define PROJEKT_STRING_UTILS_H

#include <stddef.h>
#include <stdbool.h>

/**
 * @return The length of string.
 */
size_t string_length(const char *string);

/**
 * @return The index of the first occurrence of c in string.
 * If c is not in string, -1 is returned.
 */
int find_first_char_like(const char *string, char c);

/**
 * @return The index of the first occurrence of to_find in src.
 * If no character in to_find is in src, the length of src is returned.
 */
int find_first_string_like(const char *src, const char *to_find);

/**
 * Compares two strings.
 * @returns 0 if a and b are equal.
 * @returns -1 if a is smaller than b.
 * @returns 1 if a is bigger than b.
 */
int stringcompare(const char *a, const char *b);

/**
 * Compares two strings.
 * @retruns true if a and b are equal.
 * @retruns false if a and b are not equal.
 */
bool string_equals(const char *a, const char *b);
/**
 * Compares to strings
 * @return true if one is at the start of the other
 */
bool string_in(const char *a, const char *b);

#endif //PROJEKT_STRING_UTILS_H
