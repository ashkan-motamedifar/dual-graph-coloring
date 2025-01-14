# The source code file
output := dual_graph_of_mesh_st1_final.c
# The executable name
exefile := exefile

# Compiler and flags
CC := gcc
CFLAGS := -Wall -g  # Add debugging info and warnings

# Default target to build and execute the project
all: build execute

# Build target: Compile the source code
build:
	$(CC) $(CFLAGS) $(output) -o $(exefile)

# Execute the compiled program
execute:
	./$(exefile)

# Clean target: Remove generated executable and other compiled files
.PHONY: clean
clean:
	# Platform-specific clean-up command
	$(RM) $(exefile)


