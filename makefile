# Makefile
# Compiler
CC = clang 

# Compiler flags
CFLAGS = -Wall -Werror -g

# Source files
# SRC_FILES = sim_turing.c linked_list.c dict.c 
SRC_FILES = sim_turing.c linked_list.c dict.c 

# Object files
OBJ_FILES = $(SRC_FILES:.c=.o)

# Executable name
EXECUTABLE = test_sim

# Rule to build the executable
$(EXECUTABLE): $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $@ $^

# Rule to build object files
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

# Clean rule
clean:
	rm -f $(OBJ_FILES) $(EXECUTABLE)

easy:
	$(CC) $(CFLAGS) -o $(EXECUTABLE) $(SRC_FILES)
