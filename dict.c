// creating a dictionary

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "dict.h"

#ifndef MAX_SIZE
#define MAX_SIZE 1013
#endif

#ifndef ALPHABET_SIZE
#define ALPHABET_SIZE 100
#endif



// set dict array to null
dict *init_dict() {
    // initialize dict
    dict *d = malloc(sizeof(dict));
    d->item_count = 0;
    for (int i = 0; i < MAX_SIZE; i++) {
	d->data[i] = NULL; 
    }
    return d;
}


int char_to_ascii(char c) {
    return (int)c;
}


int hash_function(char *key){
    // hash key
    int key_length = strlen(key);
    int hash = 0;
    int ascii_value;
    for (int i = 0; i < key_length; i++) {
	ascii_value = char_to_ascii(key[i]);
	hash += ((int) (pow((double)ALPHABET_SIZE, (double) i) * ascii_value)) % MAX_SIZE;
	    }
    return hash % MAX_SIZE;
}


// visualize dict
void print_dict(dict *d) {

    // print dict
    printf("index\tkey\tvalue\n");
    for (int i = 0; i < MAX_SIZE; i++) {
	if (d->data[i] != NULL) {

	    printf("%d\t%s\t%s\n", i, d->data[i]->key, d->data[i]->value);
	}
    }
}


// insert item into dict with linear probing
int insert_dict(dict* d, kvp *item) {
    // check if element already in dict
    if (look_up_dict(d, item) != NULL) {
  	printf("item already in dict\n");
    return -1;
    }
  
    // insert item
    int hash = hash_function(item->key);
    printf("hash: %d\n", hash);
    int number_of_trials = 0;  // keep track when to abbort insertion
    int i = hash; // index to insert item at

    while (d->data[i] != NULL && number_of_trials < MAX_SIZE) {
	printf("collision\n");
	i = (i + 1) % MAX_SIZE;
	number_of_trials++;
    }

    if (number_of_trials == MAX_SIZE) {
      	printf("dict full\n");
	return -1;
    }
    d->data[i] = item;
    printf("inserted @ %d\n", i);
    d->item_count++;
    return 0;
} 


// retrieve item from dict in a linear probing fashion
kvp *look_up_dict(dict *d, kvp *item){
  int hash = hash_function(item->key);
  int i = hash;
  
  while (d->data[i] != NULL){
    // for as long as no empty slot encountered search for item
    // i.e. we use lineare provbing
    if (strcmp(d->data[i]->key, item->key) == 0){
	// found item in dict
      printf("item found.\n%d\t%s\t%s", i, d->data[i]->key, d->data[i]->value);
	return d->data[i];
	}
    // not found, continue search in adjacent slots 
    i = (i + 1) % MAX_SIZE;
  }
  return NULL;
}


// int main() {
// dict *d = init_dict();
// kvp *item = malloc(sizeof(kvp));
// item->key = "hello";
// item->key_length = 5;
// item->value = "world";
// item->value_length = 5;
// 
// kvp *item_2 = malloc(sizeof(kvp));
// item_2->key = "this";
// item_2->key_length = 4;
// item_2->value = "string";
// item_2->value_length = 6;
// 
// insert_dict(d, item);
// insert_dict(d, item);
// insert_dict(d, item_2);
// print_dict(d);
// look_up_dict(d, item);
// 
// return 0;
// }
// TODO delete items from dict and recalculate hash values and reinsert items
