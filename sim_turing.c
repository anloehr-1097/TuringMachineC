// simulate a given turing machine provided as a text file

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dict.h>

typedef struct t_state {
  // state has name and index to use it with function more easily
  char *desc;
  int idx;
} state;


typedef struct tape_symbol {
  // the alphabet of the tape
  char *desc;
  int idx;
} symbol;

typedef struct tape_letter {
	// the word that is written on the tape
  struct current_symbol *symbol;
  struct symbol *next;
  struct symbol *prev;
} letter;


typedef struct t_tape {
  // the tape containing the letters from the alphabet as well as the current position
  symbol current_symbol;
} tape;

typedef struct transition_func {
  // transition dynamics

 
} transitions;

typedef struct transition_inp {

  state *state;

} trans_inp;

FILE *read_file(char *filename){
	FILE *fp;
	fp = fopen(filename, "r");
	if(fp == NULL){
		printf("Error: file not found or could not be read.\n");
		exit(1);
	}
	return fp;
}



int main(int argc, char *argv[]){
  return 0;


}
