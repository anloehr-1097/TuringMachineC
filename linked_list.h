#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct node {
        void *data;
	struct node *next;
        struct node *prev;
} node;

node *create_node(void *data);


typedef struct linked_list {
    node *current;
    node *head;
    node *tail;
    int size;
} linked_list;

typedef void (*print_function)(void *);
// pointer to arbitrary print function receiving a pointer to the data

linked_list *create_linked_list();

void free_linked_list(linked_list *l);

void add_node(linked_list *l, node *n);
void add_node_to_front(linked_list *l, node *n);

void remove_node(linked_list *l, node *n);

void visualize_list(linked_list *l, print_function f);

#endif
