#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "linked_list.h"

#define MAX_DATA_SIZE 100



node *create_node(void *data){
    node *n = malloc(sizeof(node));

    n -> data = data;
    n -> next = NULL;
    n -> prev = NULL;
    return n;
}



linked_list *create_linked_list(){
    linked_list *l = malloc(sizeof(linked_list));
    l -> current = NULL;
    l -> head = NULL;
    l -> tail = NULL;
    l -> size = 0;
    return l;
}

void free_linked_list(linked_list *l){
    node *n = l -> head;
    while(n != NULL){
	    node *next = n -> next;
	    free(n);
	    n = next;
    }
    free(l);
}


void add_node(linked_list *l, node *n){
    if(l -> head == NULL){
	l -> head = n;
	l -> tail = n;
	l -> current = n;
    } else {
	l -> tail -> next = n;
	n -> prev = l -> tail;
	l -> tail = n;
     }
     l -> size++;
}



void add_node_to_front(linked_list *l, node *n){
    if(l -> head == NULL){
	l -> head = n;
	l -> tail = n;
	l -> current = n;
    } else {
	l -> head -> prev = n;
	n -> next = l -> head;
	l -> head = n;
     }
     l -> size++;
}

void remove_node(linked_list *l, node *n){
    if(n -> prev == NULL){
	l -> head = n -> next;
    } else {
	n -> prev -> next = n -> next;
    }
    if(n -> next == NULL){
	l -> tail = n -> prev;
    } else {
	n -> next -> prev = n -> prev;
    }
    l -> size--;
    free(n);
}

void visualize_list(linked_list *l, print_function f){

    node *n = l -> head;
    while(n != NULL){
        f(n -> data);
	n = n -> next;
    }
    printf("\n");
}
