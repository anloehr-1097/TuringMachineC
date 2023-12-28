
# What is this about? 
This is an implementation of a universal turing machine (UTM) which accepts as an input another turing machine specified in a given format and yields as an output the result when simulating the other turing machine.
The turing machine provided to the universal turing machine is provided as a text file with a structure as specified below in the 'Input' section.

## Input
### Initial tape of the turing machine
The first line of the input in the text file consists of the string 'input: ' followed by the initial content of the tape. The contents of the single tape cells are seperated by commas. 

The '-' symbol indicates a blank in the respective cell.
### Initial state of the turing machine
The next line in the text file consists of the string 'init: ' followed by the initial state of the turing machine.


### Accepting states of the turing machine
The 3rd line of the file is made up of the string 'accept: ' followed by the accepting states, seperated by commas.

### Transition function
The remainder of the text file specifies the transition function for the turing machine. For each transition, the input tuple __state,tape_cell_content__ makes up one line and the following line contains the evaluation of the transition function of the form __state,new_tape_cell_content,direction__ where direction is one of '<', '>' or '-' indicating that the read-write-head of the turing machine moves to left, right, or remains at the same position.
The next line is empty and followed by the next input to the transition function.


## Example input
---------------------------
input: 1,0,0,1,0,1,0,1
init: q0
accept: qAccept

q0,0
q0,0,>

q0,1
q0,0,>

q0,_
qAccept,_,-
---------------------------
Describes a turing machine with input tape '10010101' flipping each bit to 0.

## Output
The output yields the simulation of the turing machine on the provided input. The UTM is supposed to yield the state of the tape when terminating, separated by ','. 
The UTM is supposed to halt, either if an accepting state is entered or if the input to the transition function is not defined.


# State of the project
In the midst of parsing the input and building the appropriate data structures for the UTM.
## Next steps: 
- parse and create the transition function using a dictionary / hash table.
- build the alphabet. (integrate into function parsing the input)
