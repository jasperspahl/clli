//
// Created by jasper on 03.02.23.
//

#include "string_utils.h"
#include <stdio.h>
#include <stdlib.h>

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

char **split_string(const char *string, char delimiter, size_t *size) {
	char ** result;
	char ** del_pos = malloc(1);
	del_pos[0]=NULL;
	char * tmp = (char *)string;
	*size = 0;
	while(*tmp != '\0') {
		if (delimiter == *tmp) {
			*size += 1;
			del_pos = realloc(del_pos, *size + 1);
			del_pos[*size-1]=tmp;
			del_pos[*size] = NULL;
		}
		tmp++;
	}
	// increase size if the string doesn't end with the delimiter char
	*size += *(string + string_length(string) - 1) != delimiter;

	result = malloc(sizeof(char*) * *size); //for my info ;) (sizeof(char*) [times] [value behind]size)

	tmp = (char *)string;
	for (size_t i = 0; i < *size; i++) {
		size_t sub_str_len;
		if (del_pos[i] != NULL) {
			sub_str_len = del_pos[i] - tmp;
		} else if (i+1 == *size){
			sub_str_len = (string + string_length(string)) - tmp;
		} else {
			fprintf(stderr,
				"THIS IS NOT ALLOWED TO BE HAPPENING @ %s:%d\n"
				"\t DEBUG_INFO: string: %s, delimiter: %c, size: %zu, i: %zu",
				__FILE__, __LINE__, string, delimiter, *size, i);
			exit(EXIT_FAILURE);
		}
		result[i] = malloc(sub_str_len +1);
		for (size_t j = 0; j < sub_str_len; j++) {
			result[i][j] = tmp[j];
		}
		result[i][sub_str_len] = '\0';
		tmp = del_pos[i] + 1;
	}
	tmp = NULL;
	free(del_pos);
	return result;
}
