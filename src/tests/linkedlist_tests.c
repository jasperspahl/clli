//
// Created by jasper on 03.02.23.
//

#include "linkedlist_tests.h"
#include <assert.h>
#include <stdlib.h>

#include "utils/test_utils.h"

void test_ll_create(void) {
	linked_list *list = create_linked_list(NULL);
	assert(list != NULL);
	assert(list->head == NULL);
	assert(list->tail == NULL);
	assert(list->size == 0);
	assert(list->free_value_fn == NULL);
	free_list(list);
	linked_list *list2 = create_linked_list(free);
	assert(list2 != NULL);
	assert(list2->head == NULL);
	assert(list2->tail == NULL);
	assert(list2->size == 0);
	assert(list2->free_value_fn == free);
	free_list(list2);
}

void test_ll_add(void) {
	// arrange
	linked_list *list = qll();
	// arrange
	int value = 1;
	node *n = add_node(list, &value);
	//assert
	assert(list->head == n);
	assert(list->tail == n);
	assert(list->size == 1);
	assert(n->value == &value);
	assert(n->next == NULL);
	assert(n->previous == NULL);
	// cleanup
	free_list(list);
}

void test_ll_remove_first_item(void) {
	// arrange
	linked_list *list = qll();
	int v1 = 1, v2 = 2;
	node *n = add_node(list, &v1);
	node *n2 = add_node(list, &v2);
	// act
	int *value = (int *) remove_node(list, n);
	//assert
	assert(*value == 1);
	assert(list->head == n2);
	assert(list->tail == n2);
	assert(list->size == 1);
	assert(n2->value == &v2);
	assert(n2->next == NULL);
	assert(n2->previous == NULL);
	// cleanup
	free_list(list);
}

void test_ll_remove_second_item(void) {
	// arrange
	linked_list *list = qll();

	int v1 = 1, v2 = 2;
	node *n = add_node(list, &v1);
	node *n2 = add_node(list, &v2);
	// act
	int *value = (int *) remove_node(list, n2);
	//assert
	assert(*value == 2);
	assert(list->head == n);
	assert(list->tail == n);
	assert(list->size == 1);
	assert(n->value == &v1);
	assert(n->next == NULL);
	assert(n->previous == NULL);
	// cleanup
	free_list(list);
}

void test_ll_remove_middle_item(void) {
	// arrange
	linked_list *list = qll();
	int v1 = 1, v2 = 2, v3 = 3;
	node *n = add_node(list, &v1);
	node *n2 = add_node(list, &v2);
	node *n3 = add_node(list, &v3);
	// act
	int *value = (int *) remove_node(list, n2);
	//assert
	assert(*value == 2);
	assert(list->head == n);
	assert(list->tail == n3);
	assert(list->size == 2);
	assert(n->value == &v1);
	assert(n->next == n3);
	assert(n->previous == NULL);
	assert(n3->value == &v3);
	assert(n3->next == NULL);
	assert(n3->previous == n);
	// cleanup
	free_list(list);
}

void test_ll_remove_item_at(void) {
	// arrange
	linked_list *list = qll();
	int v1 = 1, v2 = 2, v3 = 3;
	node *n = add_node(list, &v1);
	__attribute__((unused)) node *n2 = add_node(list, &v2);
	node *n3 = add_node(list, &v3);
	// act
	int *value = (int *) remove_node_at(list, 1);
	//assert
	assert(*value == 2);
	assert(list->head == n);
	assert(list->tail == n3);
	assert(list->size == 2);
	assert(n->value == &v1);
	assert(n->next == n3);
	assert(n->previous == NULL);
	assert(n3->value == &v3);
	assert(n3->next == NULL);
	assert(n3->previous == n);
	// cleanup
	free_list(list);
}

void test_ll_node_exists(void) {
	// arrange
	linked_list *list = qll();
	int v = 1;
	node *n = add_node(list, &v);
	node *n2 = {0};
	//assert
	assert(node_exists(list, n));
	assert(!node_exists(list, n2));
	// cleanup
	free_list(list);
}

void test_ll_swap_nodes__first_and_last(void) {
	// arrange
	const int values[5] = {1, 2, 3, 4, 5};
	int expected[5] = {5, 2, 3, 4, 1};
	linked_list *list = create_int_list(values, 5);
	node *first = list->head;
	node *last = list->tail;
	// act
	swap_nodes(list, first, last);
	// assert
	assert(list->head == last);
	assert(list->tail == first);
	for (int i = 0; i < 5; i++) {
		assert(*(int *) list->head->value == expected[i]);
		list->head = list->head->next;
	}
	for (int i = 4; i >= 0; i--) {
		assert(*(int *) list->tail->value == expected[i]);
		list->tail = list->tail->previous;
	}
	// cleanup
	free_list(list);
}

void test_ll_swap_nodes__second_and_second_last(void) {
	// arrange
	const int values[5] = {1, 2, 3, 4, 5};
	const int expect[5] = {1, 4, 3, 2, 5};
	linked_list *list = create_int_list(values, 5);
	node *second = list->head->next;
	node *second_last = list->tail->previous;
	// act
	swap_nodes(list, second, second_last);
	// assert
	assert(list->head->next == second_last);
	assert(list->tail->previous == second);
	for (int i = 0; i < 5; i++) {
		assert(*(int *) list->head->value == expect[i]);
		list->head = list->head->next;
	}
	for (int i = 4; i >= 0; i--) {
		assert(*(int *) list->tail->value == expect[i]);
		list->tail = list->tail->previous;
	}
	// cleanup
	free_list(list);
}

void test_ll_swap_nodes__nodes_the_same(void) {
	// arrange
	const int values[5] = {1, 2, 3, 4, 5};
	const int expect[5] = {1, 2, 3, 4, 5};
	linked_list *list = create_int_list(values, 5);
	node *n1 = list->head->next->next;
	node *n2 = list->tail->previous->previous;
	// act
	swap_nodes(list, n1, n2);
	// assert
	assert(list->head->next->next == n2);
	assert(list->tail->previous->previous == n1);
	for (int i = 0; i < 5; i++) {
		assert(*(int *) list->head->value == expect[i]);
		list->head = list->head->next;
	}
	for (int i = 4; i >= 0; i--) {
		assert(*(int *) list->tail->value == expect[i]);
		list->tail = list->tail->previous;
	}
	// cleanup
	free_list(list);
}

void test_ll_sort_list_bubble(void) {
	// arrange
	const int values[5] = {5, 4, 3, 2, 1};
	const int expect[5] = {1, 2, 3, 4, 5};
	linked_list *list = create_int_list(values, 5);
	// act
	sort_list_bubble(list, compare_ints);
	// assert
	for (int i = 0; i < 5; i++) {
		assert(*(int *) list->head->value == expect[i]);
		list->head = list->head->next;
	}
	for (int i = 4; i >= 0; i--) {
		assert(*(int *) list->tail->value == expect[i]);
		list->tail = list->tail->previous;
	}
	// cleanup
	free_list(list);
}

void test_ll_sort_list_bubble__already_sorted(void) {
	// arrange
	const int values[5] = {1, 2, 3, 4, 5};
	const int expect[5] = {1, 2, 3, 4, 5};
	linked_list *list = create_int_list(values, 5);
	// act
	sort_list_bubble(list, compare_ints);
	// assert
	for (int i = 0; i < 5; i++) {
		assert(*(int *) list->head->value == expect[i]);
		list->head = list->head->next;
	}
	for (int i = 4; i >= 0; i--) {
		assert(*(int *) list->tail->value == expect[i]);
		list->tail = list->tail->previous;
	}
	// cleanup
	free_list(list);
}

void test_ll_sort_list_bubble__empty_list(void) {
	// arrange
	linked_list *list = qll();
	// act
	sort_list_bubble(list, compare_ints);
	// assert
	assert(list->head == NULL);
	assert(list->tail == NULL);
	// cleanup
	free_list(list);
}

void test_ll_sort_list_bubble__one_item(void) {
	// arrange
	const int values[1] = {1};
	const int expect[1] = {1};
	linked_list *list = create_int_list(values, 1);
	// act
	sort_list_bubble(list, compare_ints);
	// assert
	for (int i = 0; i < 1; i++) {
		assert(*(int *) list->head->value == expect[i]);
		list->head = list->head->next;
	}
	for (int i = 0; i >= 0; i--) {
		assert(*(int *) list->tail->value == expect[i]);
		list->tail = list->tail->previous;
	}
	// cleanup
	free_list(list);
}

void test_ll_sort_list_pseudo_random(void) {
	// arrange
	srand(0);
	int values[40];
	for (int i = 0; i < 40; i++) {
		values[i] = rand();
	}
	linked_list *list = create_int_list(values, 100);
	// act
	sort_list_bubble(list, compare_ints);
	// assert
	node *current = list->head;
	for (int i = 0; i < 39; i++) {
		assert(compare_ints(current->value, current->next->value) <= 0);
		current = current->next;
	}
}

void test_ll_merge_sorted(void) {
	// arrange
	int values1[5] = {1, 3, 5, 7, 9};
	int values2[5] = {2, 4, 6, 8, 10};
	int expect[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	linked_list *list1 = create_int_list(values1, 5);
	linked_list *list2 = create_int_list(values2, 5);
	// act
	linked_list *merged = merge_lists_sorted(list1, list2, compare_ints);
	// assert
	node *current = merged->head;
	for (int i = 0; i < 10; i++) {
		assert(*(int *) current->value == expect[i]);
		current = current->next;
	}
}


void linked_list_tests(void) {
	TEST(test_ll_create);
	TEST(test_ll_add);
	TEST(test_ll_remove_first_item);
	TEST(test_ll_remove_second_item);
	TEST(test_ll_remove_middle_item);
	TEST(test_ll_remove_item_at);
	TEST(test_ll_node_exists);
	TEST(test_ll_swap_nodes__first_and_last);
	TEST(test_ll_swap_nodes__second_and_second_last);
	TEST(test_ll_swap_nodes__nodes_the_same);
	TEST(test_ll_sort_list_bubble);
	TEST(test_ll_sort_list_bubble__already_sorted);
	TEST(test_ll_sort_list_bubble__empty_list);
	TEST(test_ll_sort_list_bubble__one_item);
	TEST(test_ll_sort_list_pseudo_random);
	TEST(test_ll_merge_sorted);
}
