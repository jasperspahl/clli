#ifndef PROJEKT_LINKED_LIST_H
#define PROJEKT_LINKED_LIST_H

#include <stddef.h>
#include <stdbool.h>

/**
 * A node in a linked list.
 * @param value The value of the node.
 * @param next The next node in the linked list.
 * @param previous The previous node in the linked list.
 */
typedef struct node_ node;
struct node_ {
	void *value;
	node *next;
	node *previous;
};

/**
 * A linked list.
 * @param head The first node in the linked list.
 * @param tail The last node in the linked list.
 * @param size The size of the linked list.
 * @param free_value_fn A function to free the value of a node.
 */
typedef struct linked_list {
	node *head;
	node *tail;
	int size;

	void (*free_value_fn)(void *);
} linked_list;

/**
 * Creates a new linked list.
 * @return A pointer to the new linked list.
 */
linked_list *create_linked_list(void (*free_value)(void *));

/**
 * Adds a new node to the linked list.
 * @param list The linked list to add the node to.
 * @param value The value of the new node.
 * @return A pointer to the new node.
 */
node *add_node(linked_list *list, void *value);

/**
 * Prints the linked list.
 * @param list The linked list to print.
 */
void print_list(linked_list *list, void (*print_value)(void *));

/**
 * Frees the memory allocated for the linked list.
 * If `list->free_value_fn` is not NULL, it will be called for each node in the linked list.
 * @param list The linked list to free.
 */
void free_list(linked_list *list);

/**
 * Merges two linked lists.
 * attaches the nodes of the second list to the first one
 * @param list pointer to the first list
 * @param list2
 * @return a pointer to the merged list.
 * This should be the same as list only if list is NULL, it will return list2
 */
linked_list *merge_lists(linked_list *list, linked_list *list2);

/**
 * Removes a node from the linked list.
 * frees the memory allocated for the node.
 * Precondition: The node must exist in the linked list else program fail in `assert`.
 * @param list The linked list to remove the node from.
 * @return the value of the removed node.
 */
void *remove_node(linked_list *list, node *n);

/**
 * Removes the node at the given index and returns its value.
 * Precondition: The index must be within the bounds of the linked list else program fail in `assert`.
 * @param list The linked list to get the node from.
 * @param index The index of the node to get.
 * @return The value of the removed node.
 */
void *remove_node_at(linked_list *list, int index);

/**
 * Sorts the linked list using the quick sort algorithm.
 * @param list The linked list to sort.
 * @param compare The compare function to use. (See `man qsort` for more information on compare function.)
 */
void sort_list(linked_list *list, int (*compare)(const void *, const void *));

/**
 * Sorts the linked list using the bubble sort algorithm.
 * @param list The linked list to sort.
 * @param compare The compare function to use. (See `man qsort` for more information on compare function.)
 */
void sort_list_bubble(linked_list *list, int (*compare)(const void *, const void *));

/**
 * Swaps two nodes in the linked list.
 * Be aware that the nodes are
 * Precondition: Both nodes must exist in the linked list else program fail in `assert`.
 * @param list The linked list to swap the nodes in.
 * @param a The first node.
 * @param b The second node.
 */
void swap_nodes(linked_list *list, node *a, node *b);

/**
 * Checks if a node exists in a linked list.
 * @param list The linked list to check.
 * @param n The node to check.
 * @return True if the node exists in the linked list, false otherwise.
 */
bool node_exists(linked_list *list, node *n);

/**
 * Gets the node at the given index.
 * @param list The linked list to get the node from.
 * @param index The index of the node to get.
 * @return The node at the given index (-1 if index not in list).
 */
int get_index(linked_list *list, node *n);

#endif //PROJEKT_LINKED_LIST_H
