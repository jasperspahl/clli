//
// Created by jasper on 03.02.23.
//

#include "string_utils.h"

size_t string_length(const char *string) {
	size_t length = 0;
	while (string[length] != '\0') {
		length++;
	}
	return length;
}

int find_first_char_like(const char *string, char c) {
	int i;
	for (i = 0; string[i] != '\0'; i++) {
		if (string[i] == c) {
			return i;
		}
	}
	return -1;
}

int find_first_string_like(const char *src, const char *to_find) {
	int i;
	size_t fl = string_length(to_find);
	size_t sl = string_length(src);
	for (i = 0; sl - i >= fl; i++) {
		if (string_in(src + i, to_find)) return i;
	}
	return -1;
}

int stringcompare(const char *a, const char *b) {
	while (*a != '\0' || *b != '\0') {
		if (*a < *b) {
			return -1;
		} else if (*a > *b) {
			return 1;
		}
		a++;
		b++;
	}
	if (*a == '\0' && *b == '\0') {
		return 0;
	} else if (*a == '\0') {
		return -1;
	} else {
		return 1;
	}
}

bool string_equals(const char *a, const char *b) {
	while (*a != '\0' && *b != '\0') {
		if (*a != *b) {
			return false;
		}
		a++;
		b++;
	}
	return (*a == '\0' && *b == '\0');
}


bool string_in(const char *a, const char *b) {
	while (*a != '\0' && *b != '\0') {
		if (*a != *b) {
			return false;
		}
		a++;
		b++;
	}
	return (*b == '\0');
}
