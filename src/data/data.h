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
create_opensource_project(char *name, char *description, char *url, uint32_t stars, uint32_t issues);

opensource_project *curl_opensource_project(char *url);

void free_opensource_project(void *value);

void serialize_opensource_project(void *value, FILE *file);

opensource_project *deserialize_opensource_project(FILE *file);

void print_opensource_project(void *value);

#endif //PROJEKT_DATA_H
