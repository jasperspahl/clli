#ifndef PROJEKT_TEST_UTILS_H
#define PROJEKT_TEST_UTILS_H

#include "linked_list.h"

#define TEST(x) test_fn(x, #x)
#define qll() create_linked_list(NULL)

/*
 * Runs the given function and prints the result.
 * @param fn The function to run.
 * @param name The name of the function.
 */
void test_fn(void (*fn)(void), char *name);

void free_int(void *value);

/*
 * Creates a linked list with the given values.
 * @param values The values to add to the linked list.
 * @param size The size of the values array.
 * @return A pointer to the new linked list.
 */
linked_list *create_int_list(const int *values, size_t size);

int compare_ints(const void *a, const void *b);

#endif //PROJEKT_TEST_UTILS_H
