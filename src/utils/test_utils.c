#include "test_utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>


void test_fn(void (*fn)(void), char *name) {
	printf("\tTesting %s...", name);
	clock_t start = clock();
	fn();
	clock_t end = clock();
	printf("OK took %.5f sec\n", (double) (end - start) / CLOCKS_PER_SEC);
}

void test_suite_fn(void (*test_suite)(void), char *name) {
	printf("Running test suite %s...\n", name);
	clock_t start = clock();
	test_suite();
	clock_t end = clock();
	printf("Test suite %s finished after %.5f sec\n", name, (double) (end - start) / CLOCKS_PER_SEC);
}

void free_int(void *value) {
	free(value);
}

linked_list *create_int_list(const int *values, size_t size) {
	linked_list *list = create_linked_list(free_int);
	for (size_t i = 0; i < size; i++) {
		int *value = malloc(sizeof(int));
		*value = values[i];
		add_node(list, value);
	}
	return list;
}

int compare_ints(const void *a, const void *b) {
	return *(int *) a - *(int *) b;
}

