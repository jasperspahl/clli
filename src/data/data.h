#ifndef PROJEKT_DATA_H
#define PROJEKT_DATA_H

#include <stdio.h>
#include <stdint.h>

typedef struct opensource_project_ {
	char *name;
	char *description;
	char *url;
	uint32_t stars;
	uint32_t issues;
} opensource_project;

opensource_project *
create_opensource_project(const char *name, const char *description, const char *url, uint32_t stars, uint32_t issues);

void free_opensource_project(void *value);

/**
 * Serialize the data to file
 * data of an osp gets stored like this:
 * @example
 * @code{.c}
 * struct osp_buffer {
 * 	size_t name_length;
 * 	char name[.name_length];
 * 	size_t desc_length;
 * 	char description[.desc_length];
 * 	size_t url_length;
 * 	char url[.url_length];
 * 	uint32_t stars;
 * 	uint32_t issues;
 * };
 * @endcode
 * @param value
 * @param file
 */
void serialize_opensource_project(void *value, FILE *file);

opensource_project *deserialize_opensource_project(FILE *file);

void print_opensource_project(void *value);

#endif //PROJEKT_DATA_H
