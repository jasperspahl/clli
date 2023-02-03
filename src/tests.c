#include <stdio.h>
#include <time.h>

#include "utils/test_utils.h"

#include "tests/linkedlist_tests.h"
#include "tests/string_function_tests.h"


int main(void) {
	clock_t start = clock();

	RUN_TEST_SUITE(linked_list_tests);
	RUN_TEST_SUITE(string_tests);

	clock_t end = clock();
	printf("All tests passed in %.5f!\n", (double) (end - start) / CLOCKS_PER_SEC);
	return 0;
}
