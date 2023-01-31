#include "linked_list.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


/* INTERN ONLY------------------------------------------------ */
typedef struct {
    node *left;
    node *right;
} pair_intern;

// these functions are only used internally
pair_intern get_pair(linked_list *list, node *a, node *b);

void swap_nodes_intern(linked_list *list, node *left, node *right);

/* -------------------------------------------------------------*/

linked_list *create_linked_list(void (*free_value)(void *)) {
    linked_list *list = malloc(sizeof(linked_list));
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    list->free_value = free_value;
    return list;
}

node *add_node(linked_list *list, void *value) {
    node *new_node = malloc(sizeof(node));
    new_node->value = value;
    new_node->next = NULL;
    new_node->previous = list->tail;

    if (list->head == NULL) {
        list->head = new_node;
        list->tail = new_node;
    } else {
        list->tail->next = new_node;
        list->tail = new_node;
    }
    list->size++;
    return new_node;
}

void print_list(linked_list *list, void (*print_value)(void *)) {
    node *current = list->head;
    while (current != NULL) {
        print_value(current->value);
        current = current->next;
    }
    printf("\n");
}

void free_list(linked_list *list) {
    node *current = list->head;
    while (current != NULL) {
        node *next = current->next;
        if (list->free_value != NULL) {
            list->free_value(current->value);
        }
        free(current);
        current = next;
    }
    free(list);
}

void *remove_node(linked_list *list, node *n) {
    assert(node_exists(list, n));
    node *previous = n->previous;
    node *next = n->next;
    void *value = n->value;
    if (previous == NULL) {
        list->head = next;
    } else {
        previous->next = next;
    }
    if (next == NULL) {
        list->tail = previous;
    } else {
        next->previous = previous;
    }
    list->size--;
    free(n);
    return value;
}

void *remove_node_at(linked_list *list, int index) {
    assert(index >= 0);
    assert(index < list->size);
    node *current = list->head;
    for (int i = 0; i < index; i++) {
        current = current->next;
    }
    return remove_node(list, current);
}

void sort_list(linked_list *list, int (*compare)(const void *, const void *)) {
    // TODO: implement quicksort
}

void sort_list_bubble(linked_list *list, int (*compare)(const void *, const void *)) {
    node *current;
    bool swapped = true;
    while (swapped) {
        swapped = false;
        current = list->head;
        while (current != NULL && current->next != NULL) {
            if (compare(current->value, current->next->value) > 0) {
                /* internal swap function can be used here since the nodes
                 * are guaranteed to be in the list and in the correct order */
                swap_nodes_intern(list, current, current->next);
                // assign current to the previous node since current was pushed forward
                current = current->previous;
                swapped = true;
            }
            current = current->next;
        }
    }
}


void swap_nodes_intern(linked_list *list, node *left, node *right) {
    if (left == right) {
        return;
    }

    if (left->previous) {
        left->previous->next = right;
    } else {
        list->head = right;
    }
    if (right->next) {
        right->next->previous = left;
    } else {
        list->tail = left;
    }

    if (left->next == right) {
        assert(right->previous == left);
        left->next = right->next;
        right->previous = left->previous;
        right->next = left;
        left->previous = right;
        return;
    }
    left->next->previous = right;
    right->previous->next = left;
    node *tmp = left->next;
    left->next = right->next;
    right->next = tmp;
    tmp = right->previous;
    right->previous = left->previous;
    left->previous = tmp;
}

pair_intern get_pair(linked_list *list, node *a, node *b) {
    pair_intern pair;
    if (get_index(list, a) < get_index(list, b)) {
        pair.left = a;
        pair.right = b;
    } else {
        pair.left = b;
        pair.right = a;
    }
    return pair;
}

void swap_nodes(linked_list *list, node *a, node *b) {
    assert(list->size > 1);
    assert(node_exists(list, a));
    assert(node_exists(list, b));
    pair_intern pair = get_pair(list, a, b);
    swap_nodes_intern(list, pair.left, pair.right);
}

int get_index(linked_list *list, node *n) {
    node *current = list->head;
    int index = 0;
    while (current != NULL) {
        if (current == n) {
            return index;
        }
        current = current->next;
        index++;
    }
    return -1;
}

bool node_exists(linked_list *list, node *n) {
    return get_index(list, n) >= 0;
}
