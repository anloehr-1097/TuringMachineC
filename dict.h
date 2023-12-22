// Dictionary header file
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct key_value_pair {} kvp;
typedef struct {} dict;

dict *init_dict();
void print_dict(dict *);
int insert(dict *, kvp *);
kvp * look_up_dict(dict *, kvp *);
