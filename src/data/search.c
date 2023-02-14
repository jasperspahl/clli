//
// Created by jasper on 06.02.23.
//

#include "search.h"
#include "utils/string_utils.h"

#include <stdlib.h>

search_res search(linked_list *list, char *search_term, search_score_calculator calculator) {
	search_res result = {0};
	result.searchResults = NULL;
	node* current = list->head;
	while (current != NULL) {
		int search_score = calculator(current->value, search_term);
		if (search_score > 0) {
			result.searchResults = realloc(result.searchResults, ++result.size * sizeof(search_res_entry));
			result.searchResults[result.size-1].search_score = search_score;
			result.searchResults[result.size-1].node = current;
		}
		current = current->next;
	}
	return result;
}

void sort_search_res(search_res *res) {
	sort_search_res_r(res, 0, res->size-1);
}

void sort_search_res_r(search_res *res, size_t start, size_t end) {
	// an empty list or a list with one element is already sorted
	if (res->size <= 1) {
		return;
	}
	if (start < end) {
		return;
	}
	size_t pivot;
	sort_search_res_partition(res, start, end, &pivot);
	sort_search_res_r(res, start, pivot-1);
	sort_search_res_r(res, pivot+1, end);
}

void sort_search_res_swap(search_res *res, size_t a, size_t b) {
	if (a == b || a >= res->size || b >= res->size) {
		return;
	}
	search_res_entry tmp = res->searchResults[a];
	res->searchResults[a] = res->searchResults[b];
	res->searchResults[b] = tmp;
}

void sort_search_res_partition(search_res *res, size_t start, size_t end, size_t *pivot) {
	size_t left = start;
	size_t right = end;
	*pivot = start;
	while (left < right) {
		while (left < end && res->searchResults[left].search_score <= res->searchResults[*pivot].search_score) {
			left++;
		}
		while (res->searchResults[right].search_score > res->searchResults[*pivot].search_score) {
			right--;
		}
		if (left < right) {
			sort_search_res_swap(res, left, right);
		}
	}
	sort_search_res_swap(res, *pivot, right);
	*pivot = right;
}
