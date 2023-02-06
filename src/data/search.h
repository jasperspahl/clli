//
// Created by jasper on 06.02.23.
//

#ifndef PROJEKT_SEARCH_H
#define PROJEKT_SEARCH_H

#include "linked_list.h"

typedef struct search_res_entry_ {
	node * node;
	int search_score;
} search_res_entry;

typedef struct search_res_ {
	search_res_entry *searchResults;
	size_t size;
} search_res;

typedef int (*search_score_calculator)(void *, char *);

search_res search(linked_list *list, char* search_term, search_score_calculator calculator);

void sort_search_res(search_res *res);

#endif //PROJEKT_SEARCH_H
