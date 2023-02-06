//
// Created by jasper on 06.02.23.
//

#include "search.h"
#include "utils/string_utils.h"

#include <stdlib.h>

search_res search(linked_list *list, char *search_term, search_score_calculator calculator) {
	search_res result = {0};
	result.searchResults = malloc(sizeof(search_res_entry) * list->size);
	node* current = list->head;
	while (current != NULL) {
		result.searchResults[result.size].search_score = calculator(current->value, search_term);
		result.searchResults[result.size].node = current;
		current = current->next;
		if (result.searchResults[result.size].search_score >= 0)
			result.size++;
	}
	return result;
}

int compare_search_res_entry(const void *a, const void *b) {
	search_res_entry *entry_a = (search_res_entry *) a;
	search_res_entry *entry_b = (search_res_entry *) b;
	return entry_b->search_score - entry_a->search_score;
}

void sort_search_res(search_res *res) {
	// TODO: implement a better sorting algorithm
	qsort(res->searchResults, res->size, sizeof(search_res_entry), compare_search_res_entry);
}
