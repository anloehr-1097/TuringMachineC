// simulate a given turing machine provided as a text file

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dict.h"

#define MAX_ALPHABET_SIZE 100
#define MAX_LINE_LENGTH 200

typedef struct t_state {
  // state has name and index to use it with function more easily
  int idx;
  char *desc;
} state;


typedef struct tape_symbol {
  // the alphabet of the tape
  int idx;
  char *desc;
} symbol;


typedef struct {
  // alphabet of the tape
  symbol *symbols[MAX_ALPHABET_SIZE];
  size_t size;
} alphabet;


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

typedef struct {
  state s;
  tape t;
  alphabet a;
  transitions transfun;
} turing_machine;

  

FILE *read_file(char *filename){
	FILE *fp;
	fp = fopen(filename, "r");
	if(fp == NULL){
		printf("Error: file not found or could not be read.\n");
		exit(1);
	}
	return fp;
}

char *find_substring(char *line, char *substring){
    // find substring in line
    char *has_substring;
    has_substring = strstr(line, substring);
    return has_substring;
}


void parse_input(char *line, turing_machine *tm) {
  char *tokens = line + strlen("input: "); 
  fputs(tokens, stdout);
}


void parse_init(char *line, turing_machine *tm) {
  char *tokens = line + strlen("init: "); 
  fputs(tokens, stdout);
}



void parse_accept(char *line, turing_machine *tm) {
  char *tokens = line + strlen("accept: "); 
  fputs(tokens, stdout);
}


void parse_line(char *line, turing_machine *tm){
    // parse line and add info to turing machine
    char token;
    int len = strlen(line);
    fputs(line, stdout);

    // substrings to probe for 
    char *substrings[3] = {"input: ", "init: ", "accept: "};
    char *has_substring;
    int idx = 0;

    while (idx < 3) {
      // assume only one substring per line matching
	has_substring = find_substring(line, substrings[idx]);
	if (has_substring != NULL) {
	    break; 
	}
	idx++;
    } 
    if (has_substring != NULL) {
	// found a substring
	switch(idx) {
	    case 0:
		// 'input' line
		parse_input(has_substring, tm);
		break;

	    case 1:
		// 'init' line'
		parse_init(has_substring, tm);
		break;

	    case 2:
		// 'accept' line
		parse_accept(has_substring, tm);
		break;
        }
    }
}


void parse_file(FILE *fp, turing_machine *tm){
    // parse file and create turing machine
    char line[MAX_LINE_LENGTH] = {0};
    size_t len = 0;
    // read lines
    while((fgets(line, MAX_LINE_LENGTH, fp)) != NULL){
      parse_line(line, tm);
    };
   
}


int main(int argc, char *argv[]){
  FILE *file = read_file("input.txtt");
  turing_machine *tm = malloc(sizeof(turing_machine));
  parse_file(file, tm);


  // printf("Contains init: %s\n", find_substring("init: blablabalbal", "init: "));
  // printf("Contains input: %s\n", find_substring("input: blablabalbal", "input: "));
  // printf("Contains input: %s\n", find_substring("init: blablabalbal", "input: "));
  // printf("Contains init: %s\n", find_substring("input: blablabalbal", "init: "));
  return 0;


}
