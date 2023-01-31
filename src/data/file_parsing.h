#ifndef PROJEKT_FILE_PARSING_H
#define PROJEKT_FILE_PARSING_H

#include "linked_list.h"

linked_list *read_file(const char *filename);

void write_file(const char *filename, linked_list *list);

#endif //PROJEKT_FILE_PARSING_H
