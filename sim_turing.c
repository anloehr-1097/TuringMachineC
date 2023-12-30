// simulate a given turing machine provided as a text file

/*
  The input file is a text file.
  - Each symbol has a description as well as an index which makes the implementation of the transition function easier.
  - The alphabet is a list of unique symbols.
  - The input is a list of symbols from the alphabet.
  - each state has an index, two booleans (accepting and rejecting) and a description.
  - the tape is a list of letters from the alphabet implemented as a doubly linked list and the current position is a pointer to the current letter.

  
*/



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dict.h"
#include "linked_list.h"

#define MAX_ALPHABET_SIZE 100
#define MAX_LINE_LENGTH 200
#define TRUE 1
#define FALSE 0
#define MAX_STATES 100
#define VISUALIZE_TAPE 0 
#define VISUALIZE_STATES 1 
#define DEBUG 0 
int num_states = 0;


typedef struct t_state {
  // state has name and index to use it with function more easily
  int idx;
  int is_accepting;
  int is_rejecting;
  char *desc;

} state;


state *create_state(int idx, int is_accepting, int is_rejecting, char *desc){
    state *s = malloc(sizeof(state));
    s -> idx = idx;
    s -> is_accepting = is_accepting;
    s -> is_rejecting = is_rejecting;
    s -> desc = desc;
    return s;

}

void visualize_states(state** states) {
    // print states
    int i = 0;
    printf("index\tdesc\t\accepting\trejecting\t\n");
    while (states[i] != NULL){
	printf("%d\t%s\t%d\t%d\n", states[i]->idx, states[i]->desc, states[i]->is_accepting, states[i]->is_rejecting);
	i ++;
    }
}

void free_state(state *s){
	free(s);
}


typedef struct tape_symbol {
  // the alphabet of the tape
  int idx;
  char *desc;
} symbol;


symbol *create_symbol(int idx, char *desc){
	symbol *s = malloc(sizeof(symbol));
	s -> idx = idx;
	s -> desc = desc;
	return s;
}



void free_symbol(symbol *s){
	free(s);
}



typedef struct {
  // alphabet of the tape with unqiue symbols
  symbol *symbols[MAX_ALPHABET_SIZE];
  size_t size;
} alphabet;


typedef struct transition_func {
  // transition dynamics
  dict *transitions;
  size_t size;
} transitions;

typedef struct transition_inp {

  state *state;

} trans_inp;

typedef struct {
  state s;
  linked_list tape;
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


int cut_substring(char *str, char *substr){
    // return len to cut substring from string starting at index 0
    char *has_substring = NULL;
    has_substring = strstr(str, substr);

    if(has_substring != NULL){
      int ret = strlen(str) - strlen(has_substring) + 1;
      return ret;
    }
    return -1;
  }

node *create_symbol_node(symbol *s) {

    node *n = malloc(sizeof(node));

    n -> data = (symbol *) s;
    n -> next = NULL;
    n -> prev = NULL;
    return n;
};

void print_symbol(void *data) {
    // needed to print the linked list of symbols
    symbol *sym = (symbol *) data;
    // visualize tape of turing machine
    printf("Symbol %d: %s\n", sym->idx, sym -> desc);
}


void visualize_tape(turing_machine *tm) {
  visualize_list(&tm->tape, print_symbol);
}


void parse_input(char *line, turing_machine *tm) {
    // TODO add new input characters to alphabet
    char *first_token = line + strlen("input: "); 
    char tokens[strlen(line)] ;

    fputs(tokens, stdout);
    strcpy(tokens ,first_token); 
    // create linked list of letters on tape
    char substring[MAX_LINE_LENGTH] = "";
    int len = cut_substring(tokens, ",");

    symbol *current_symb;
    int idx = 0; 
    node *n;
  
    while (len > -1){
	strlcpy(substring, tokens, len);
	// create new letter
	current_symb = create_symbol(idx, strdup(substring));
	n = create_symbol_node(current_symb); 
	add_node(&tm->tape, n);

	// modify token sequence to be further parsed 
	first_token += len;
	strcpy(tokens, first_token);

	#if DEBUG
	printf("Substring = %s\n", substring);
	printf("Tokens = %s\n", tokens);
	printf("Len = %d\n", len);
	#endif
	len = cut_substring(tokens, ",");
	idx++;

    }

    // last letter remaining in tokens, need to parse
    current_symb = create_symbol(idx, tokens);
    n = create_node(current_symb); 
    add_node(&tm->tape, n);
    #if VISUALIZE_TAPE 
    visualize_tape(tm);
    #endif
}


void parse_init(char *line, turing_machine *tm, state *states[MAX_STATES]) {
    char *first_token = line + strlen("init: "); 
    first_token[2] = '\0';
    char *tokens = (char *) malloc(strlen(line) * sizeof(char));
    strcpy(tokens, first_token);
    // tokens contains one valid state which is the init state
    // init state is first state ever added to states
    // remove newline char

    state *init_state = create_state(0, 0, 0, tokens);
    states[0] = init_state;
    tm ->s = *init_state;
}


void parse_accept(char *line, turing_machine *tm, state *states[MAX_STATES]) {
    char *first_token = line + strlen("accept: "); 
    char *tokens = (char *) malloc(strlen(line) * sizeof(char));
    strcpy(tokens, first_token);

    // accept states starting from index 1
    int idx = 1; 

    char substring[MAX_LINE_LENGTH] = "";
    // split substring based on ','
    int len = cut_substring(tokens, ",");
    state *s;
  
    while (len > -1){
	strlcpy(substring, tokens, len);
	// create new letter
	
	s = create_state(idx, 1, 0, strdup(substring));
	states[idx] = s;

	// modify token sequence to be further parsed 
	first_token += len;
	strcpy(tokens, first_token);

	#if DEBUG
	printf("Substring = %s\n", substring);
	printf("Tokens = %s\n", tokens);
	printf("Len = %d\n", len);
	#endif
	len = cut_substring(tokens, ",");
	idx++;

    }

    // last letter remaining in tokens, need to parse, remove newline
    tokens[strlen("qAccept")] = '\0';
    s = create_state(idx, 1, 0, tokens);
    states[idx] = s;
    #if VISUALIZE_STATES
    visualize_states(states);
    #endif
    num_states = idx;
}


int search_state_in_states(char *state_name, state *states[MAX_STATES]){
    // search for state in states
    int i = 0;
    while (states[i] != NULL){
	if (strcmp(states[i]->desc, state_name) == 0){
	    return i;
	}
	i++;
    }
        return -1;
}


void parse_transitions(char *line, turing_machine *tm, state *states[MAX_STATES]){
    // receive concatenation of two transition lines corresponding to one transition
    // parse transitions, add new states
    int idx = num_states;

    // get inp state
    int first_comma = cut_substring(line, ",");
    char *inp_state = (char *) malloc(sizeof(char) * first_comma);
    strlcpy(inp_state, line, first_comma);

    // check if state already exists in states
    if (search_state_in_states(inp_state, states) == -1){
        // state not existent yet, create new state
        state *s = create_state(idx, 0, 0, inp_state);
    }

    // check if tape symbol already exists in alphabet
    // TODO later
    // if not create new symbol and add to alphabet

    // determine key for transition
    int inp_comb = cut_substring(line, "\n");
    char *key = (char *) malloc(sizeof(char) * inp_comb);
    strlcpy(key, line, inp_comb);

    // determine value for transition
    char *value = (char *) malloc(sizeof(char) * inp_comb);
    strlcpy(value, line + inp_comb, strlen(line) - inp_comb + 1);
     
    kvp *item = (kvp *) malloc(sizeof(kvp));
    item -> key = key;
    item -> value = value;
    item -> key_length = strlen(key);
    item -> value_length = strlen(value);
    printf("key: %s\n", item->key);
    printf("value: %s\n", item->value);
    insert_dict(tm->transfun.transitions, item);
    print_dict(tm->transfun.transitions);
    // add transition to transition function


}


int check_if_intermediate_line(char *line) {
    // check if line is first line of a transition specification
    int has_comma;
    int has_left_move;
    int has_right_move;
    int has_remain_move;
    
    if (cut_substring(line, ",") > -1) has_comma = TRUE;
    if (cut_substring(line, ">") > -1) has_right_move = TRUE;
    if (cut_substring(line, "<") > -1) has_left_move = TRUE;
    if (cut_substring(line, "-") > -1) has_remain_move = TRUE;


    return has_comma && !(has_left_move || has_right_move || has_remain_move);
}


int parse_line(char *line, turing_machine *tm, state *states[MAX_STATES]){
    // parse line and add info to turing machine
    #if DEBUG
    fputs(line, stdout);
    #endif

    // substrings to probe for 
    char *substrings[3] = {"input: ", "init: ", "accept: "};
    char *has_substring;
    int idx = 0;
    int is_intermediate = FALSE;

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
	        parse_init(has_substring, tm, states);
		break;

	    case 2:
		// 'accept' line
	        parse_accept(has_substring, tm, states);
		break;
	    default:
	        // neither of keywords found either newline or part of transition function
	        is_intermediate = check_if_intermediate_line(line);
	        break;
        }
    }
    return is_intermediate;
}


void parse_file(FILE *fp, turing_machine *tm, state *states[MAX_STATES]){
    // parse file and create turing machine
    char line[MAX_LINE_LENGTH] = {0};
    char *intermediate; // needed for transition parsing
    int is_intermediate;

    // read lines
    while((fgets(line, MAX_LINE_LENGTH, fp)) != NULL){
        if ((is_intermediate = parse_line(line, tm, states)) == 1){
	    // intermediate line, need to parse further 2 lines in a row
	    intermediate = strdup(line); // copy line to intermediate
	    fgets(line, MAX_LINE_LENGTH, fp); // read next line
	    strcat(intermediate, line); // concatenate intermediate and line
	    printf("intermediate: %s\n", intermediate);
	    parse_transitions(intermediate, tm, states); // parse intermediate line
        } else {
	  ;
	}

    }
   
}


int main(int argc, char *argv[]){
  FILE *file = read_file("input.txtt");
  turing_machine *tm = malloc(sizeof(turing_machine));
  tm ->transfun.transitions = init_dict();
  state *states[MAX_STATES] = {0};


  // parse_file(file, tm, states);
  //parse_input("input: 1,0,1,0,1,0,1,0,1,0,1,0,1,0", tm);
  parse_transitions("q0,0\nq0,0,>", tm, states);
  parse_transitions("q0,1\nq0,0,>", tm, states);
  parse_transitions("q0,_\nqAccept,_,-", tm, states);



  // printf("Contains init: %s\n", find_substring("init: blablabalbal", "init: "));
  // printf("Contains input: %s\n", find_substring("input: blablabalbal", "input: "));
  // printf("Contains input: %s\n", find_substring("init: blablabalbal", "input: "));
  // printf("Contains init: %s\n", find_substring("input: blablabalbal", "init: "));
  return 0;


}
