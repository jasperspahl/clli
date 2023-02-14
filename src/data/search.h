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

/**
 * Sorts the search results by their search score.
 * @note This function uses quicksort.
 * @param res The search results to sort.
 */
void sort_search_res(search_res *res);
void sort_search_res_r(search_res *res, size_t start, size_t end);
void sort_search_res_swap(search_res *res, size_t a, size_t b);
void sort_search_res_partition(search_res *res, size_t start, size_t end, size_t *pivot);

#endif //PROJEKT_SEARCH_H
