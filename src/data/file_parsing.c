#include "file_parsing.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "linked_list.h"
#include "data.h"

linked_list *read_file(const char *filename) {
	FILE *file = fopen(filename, "rb");
	if (file == NULL) {
		printf("Could not open file %s: %s\n", filename, strerror(errno));
		exit(1);
	}
	linked_list *list = create_linked_list(free_opensource_project);

	while (!feof(file)) {
		opensource_project *project = deserialize_opensource_project(file);
		if (project == NULL) {
			break;
		}
		add_node(list, project);
	}
	fclose(file);
	return list;
}

void write_file(const char *filename, linked_list *list) {
	FILE *file = fopen(filename, "wb");
	if (file == NULL) {
		fprintf(stderr, "Could not open file %s: %s\n", filename, strerror(errno));
		exit(1);
	}
	node *current = list->head;
	while (current != NULL) {
		serialize_opensource_project(current->value, file);
		current = current->next;
	}
	fclose(file);
}
