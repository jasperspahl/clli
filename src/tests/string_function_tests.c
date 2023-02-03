//
// Created by jasper on 03.02.23.
//

#include <assert.h>
#include <string.h>
#include "string_function_tests.h"

#include "utils/test_utils.h"
#include "utils/string_utils.h"

void test_string_equals__returns_true(void) {
	// arrange
	char *a = "hallo";
	char *b = "hallo";
	// act
	bool res = string_equals(a, b);
	// assert
	assert(res == true);
}

void test_string_equals__returns_false(void) {
	// arrange
	char *a = "test";
	char *b = "not_test";
	// act
	bool res = string_equals(a, b);
	// assert
	assert(res == false);
}

void test_string_length__acts_like_strlen(void) {
	// arrange
	char *str = "123456789";
	// act
	size_t res = string_length(str);
	// assert
	assert(res == strlen(str));
}

void test_string_length__123456789_is_9(void) {
	// arrange
	char *str = "123456789";
	// act
	size_t res = string_length(str);
	// assert
	assert(res == strlen(str));
}

void test_find_first_char_like__found(void) {
	// arrange
	char *str = "123456789";
	// act
	int i = find_first_char_like(str, '4');
	// assert
	assert(i == 3);
}

void test_find_first_char_like__not_found(void) {
	// arrange
	char *str = "123456789";
	// act
	int i = find_first_char_like(str, 'c');
	// assert
	assert(i == -1);
}

void test_find_first_string_like__found(void) {
	// arrange
	char *str = "hello world";
	char *to_find = "world";
	// act
	int i = find_first_string_like(str, to_find);
	// assert
	assert(i == 6);
}

void test_find_first_string_like__not_found(void) {
	// arrange
	char *str = "123456789";
	char *to_find = "world";
	// act
	int i = find_first_string_like(str, to_find);
	// assert
	assert(i == -1);
}

void string_tests(void) {
	TEST(test_string_equals__returns_true);
	TEST(test_string_equals__returns_false);
	TEST(test_string_length__123456789_is_9);
	TEST(test_string_length__acts_like_strlen);
	TEST(test_find_first_char_like__found);
	TEST(test_find_first_char_like__not_found);
	TEST(test_find_first_string_like__found);
	TEST(test_find_first_string_like__not_found);
}
