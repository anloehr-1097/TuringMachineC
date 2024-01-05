// simulate a given turing machine provided as a text file

/*
  The input file is a text file.
  - Each symbol has a description as well as an index which makes the implementation of the transition function easier.
  - The alphabet is a list of unique symbols.
  - The input is a list of symbols frt letter.

  
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
#define VISUALIZE_TAPE 1 
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
    state *s;
    symbol *sym;
    char mov;
} state_symbol_mov_pair;

void print_ssm_pair(state_symbol_mov_pair *pair) {
    printf("State: %s\n", pair->s->desc);
    printf("Symbol: %s\n", pair->sym->desc);
    printf("Movement: %c\n", pair->mov);
}

typedef struct {
  // alphabet of the tape with unqiue symbols
  symbol *symbols[MAX_ALPHABET_SIZE];
  size_t size;
} alphabet;


int search_in_alphabet(char *symbol_name, alphabet a){
    // search for symbol in alphabet
    int i = 0;
    while (a.symbols[i] != NULL){
	if (strcmp(a.symbols[i]->desc, symbol_name) == 0){
	    return i;
	}
	i++;
    }
    return -1;
}


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


int cut_one_of_substrings(char *str, char **substr, int num_substr){
    // return len to cut substring from string starting at index 0
    int idx = 0;
    int len;

    while (idx < num_substr){
        len = cut_substring(str, substr[idx]);
	if(len != -1){
	  return len;
	}
	idx++;
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
        states[idx] = s;
	num_states ++;
    }

    // determine key for transition
    int inp_comb = cut_substring(line, "\n");
    char *key = (char *) malloc(sizeof(char) * inp_comb);
    strlcpy(key, line, inp_comb);
    // check if tape symbol already exists in alphabet
    // if not create new symbol and add to alphabet
    if (search_in_alphabet(key, tm->a) == -1){
	// symbol not existent yet, create new state
	symbol *symb = create_symbol(tm->a.size, key);
	tm->a.symbols[tm->a.size] = symb;
	tm->a.size++;
    }

    // determine value for transition
    char *value = (char *) malloc(sizeof(char) * (strlen(line) - inp_comb + 1));

    // find last comma
    char *move_substring[3] = {">", "<", "-"};
    int movement = cut_one_of_substrings(line, move_substring, 3);

    // copy into value
    strlcpy(value, line + inp_comb, movement + 1 - inp_comb);
     
    kvp *item = (kvp *) malloc(sizeof(kvp));
    item -> key = key;
    item -> value = value;
    item -> key_length = strlen(key);
    item -> value_length = strlen(value);
    #if DEBUG
    printf("key: %s\n", item->key);
    printf("value: %s\n", item->value);
    #endif
    insert_dict(tm->transfun.transitions, item);
    // print_dict(tm->transfun.transitions);
    // add transition to transition function
}


int check_if_intermediate_line(char *line) {
    // check if line is first line of a transition specification
    int has_comma = FALSE;
    int has_left_move = FALSE;
    int has_right_move = FALSE;
    int has_remain_move = FALSE;
    
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
        }
	return is_intermediate;
    }
    // neither of keywords found either newline or part of transition function
    is_intermediate = check_if_intermediate_line(line);
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
	    free(intermediate);
        } else {
	  ;
	}


    }
   
}

void split_string_to_state_symbol_mov_pair(char *str, int str_len,
                                       state *states[MAX_STATES],
                                       state_symbol_mov_pair *pair,
				       turing_machine *tm)
    {
         // given string, return state symbol pair
	int comma_idx = cut_substring(str, ",");

	char *state_name = (char *) malloc(sizeof(char) * comma_idx);
	strlcpy(state_name, str, comma_idx);

	// TODO the following should actually go into the parsing part of the program
	// check if state already exists in states
	int state_idx = search_state_in_states(state_name, states);

	if (state_idx == -1){
	    // state not existent yet, create new state
	    state *s = create_state(num_states, 0, 0, state_name);
	    num_states++;
	    states[num_states] = s;
	    pair->s = s;
	} else {
	    pair->s = states[state_idx];
	}

	// symbol is second part until next comma
	int next_comma_idx = cut_substring(str + comma_idx, ",");
	char *symbol_name = (char *) malloc(sizeof(char) * next_comma_idx);
	strlcpy(symbol_name, str + comma_idx, next_comma_idx);

	int symbol_idx = search_in_alphabet(symbol_name, tm->a);

	if (symbol_idx == -1){
	    // symbol not existent yet, create new state
	    symbol *symb = create_symbol(tm->a.size, symbol_name);
	    tm->a.symbols[tm->a.size] = symb;
	    tm->a.size++;
	    pair->sym = symb;
	} else {
	    pair->sym = tm->a.symbols[symbol_idx];
	}


	// movement is third part, right after 3rd comma
	// pair -> mov = str[comma_idx + next_comma_idx + 1];
	pair -> mov = str[str_len - 1];
}


state *extract_state_from_str(char *str, int str_len, state *states[MAX_STATES]) {
    // given a string as found in key-value pair, extract state
    // state is first part until comma
    int comma_idx = cut_substring(str, ",");

    char *state_name = (char *) malloc(sizeof(char) * comma_idx);
    strlcpy(state_name, str, comma_idx);


    // TODO the following should actually go into the parsing part of the program
    // check if state already exists in states
    int state_idx = search_state_in_states(state_name, states);
   
    if (state_idx == -1){
      	// state not existent yet, create new state
	state *s = create_state(num_states, 0, 0, state_name);
	num_states++;
	states[num_states] = s;
	return s;
    } else {
        return states[state_idx];
    }
}


void simulate_turing_machine(turing_machine *tm, state *states[MAX_STATES]) {
    // simulate turing machine
    // only stop if accept or reject state reached  
    // reject states are all the states not in the transition function

    state *current_state;
    symbol *current_symbol = tm->tape.current->data;
    char current_state_symb[100] = "";
    state_symbol_mov_pair pair;
    print_symbol(current_symbol);

    printf("Simulating turing machine.\n");
    printf("Visualizing tape.\n");
    visualize_tape(tm);

    while (TRUE) {
      	// get current state and symbol
        
        memset(current_state_symb, 0, sizeof(current_state_symb));

        current_state = &(tm->s);
	current_symbol = tm->tape.current->data;
	strncat(current_state_symb, current_state -> desc, strlen(current_state -> desc));
	strcat(current_state_symb, ",");
	strcat(current_state_symb, current_symbol->desc);
	// get transition from transition function
	kvp *transition = look_up_dict(tm->transfun.transitions, current_state_symb);
	if (transition == NULL) {
	    // no transition found, reject
	    printf("No transition found, reject.\n");
	    break;
        }
	// split transition into state, symbol, movement
	split_string_to_state_symbol_mov_pair(transition->value, transition->value_length, states, &pair, tm);
	// update tape
	tm->tape.current->data = pair.sym;
	// update stat
	tm->s = *pair.s;
	// move tape
	if (pair.mov == '>') {
	    tm->tape.current = tm->tape.current->next;
	} else if (pair.mov == '<') {
	    tm->tape.current = tm->tape.current->prev;
	} else {
	    // remain
	    ;
        }
	// check if accept or reject state
	if (tm->s.is_accepting == 1) {
	    printf("Accept.\n");
	    break;
	}

    }
    // print tape
    printf("Visualizing tape.\n");
    visualize_tape(tm);
}

int main(int argc, char *argv[]){
  // FILE *file = read_file("input.txtt");
  FILE *file = read_file("input_2.txt");
  turing_machine *tm = malloc(sizeof(turing_machine));
  tm ->transfun.transitions = init_dict();
  state *states[MAX_STATES] = {0};


  parse_file(file, tm, states);
  print_dict(tm->transfun.transitions);
  state_symbol_mov_pair pair ;
  print_ssm_pair(&pair);


  // split_string_to_state_symbol_mov_pair("q0,1,>\0", strlen("q0,1,>\0"), states, &pair, tm);
  split_string_to_state_symbol_mov_pair("q0,1wer,-\0", strlen("q0,1wer,-\0"), states, &pair, tm);

  // symbol *s = tm -> tape.current -> data;
  // print_symbol(s);
  simulate_turing_machine(tm, states);

  //  print_ssm_pair(&pair);

  // parse_input("input: 1,0,1,0,1,0,1,0,1,0,1,0,1,0", tm);
  // parse_transitions("q0,0\nq0,0,>", tm, states);
  // parse_transitions("q0,1\nq0,0,>", tm, states);
  // parse_transitions("q0,_\nqAccept,_,-", tm, states);

  // check_if_intermediate_line("q0,0\n");
  //  
  // printf("Testing function 'check_if_intermediate_line' from sim_turing.c.\n");
  // char *line_success = "q0, 0\n";
  // char *line_failure_1= "\n";
  // char *line_failure_2= "q0, 0,>\n";
  // char *line_failure_3= "q1, 4anbsw,>\n";
  //  
  // assert(check_if_intermediate_line(line_success) == 1);
  // assert(check_if_intermediate_line(line_failure_1) == 0);
  // assert(check_if_intermediate_line(line_failure_2) == 0);
  // assert(check_if_intermediate_line(line_failure_3) == 0);
  // 
  // 

  // printf("Contains init: %s\n", find_substring("init: blablabalbal", "init: "));
  // printf("Contains input: %s\n", find_substring("input: blablabalbal", "input: "));
  // printf("Contains input: %s\n", find_substring("init: blablabalbal", "input: "));
  // printf("Contains init: %s\n", find_substring("input: blablabalbal", "init: "));
  return 0;



}
