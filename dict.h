// Dictionary header file
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifndef MAX_SIZE
#define MAX_SIZE 1013
#endif

#ifndef ALPHABET_SIZE
#define ALPHABET_SIZE 100
#endif

typedef struct key_value_pair {
char *key;
int key_length;
char *value;
int value_length;
} kvp;


// definition of dict
typedef struct {
kvp *data[MAX_SIZE];  // pointer to array of kvp pointers
size_t item_count;
} dict;

dict *init_dict();
void print_dict(dict *);
int insert_dict(dict *, kvp *);
kvp * look_up_dict(dict *, char *);
