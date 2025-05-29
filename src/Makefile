# Compiler and flags
CC      := gcc
CFLAGS  := -Wall -g -Isrc/vendor -Isrc/include

# Directories and files
SRC_DIR := src
SRCS    := $(wildcard $(SRC_DIR)/*.c)
BIN     := exefile

.PHONY: all build run clean

# Default: build and run
all: build run

# Build the binary from all sources
build: $(BIN)

$(BIN): $(SRCS)
	$(CC) $(CFLAGS) $^ -o $@

# Run the program
run: $(BIN)
	./$(BIN)

# Clean up the binary
clean:
	$(RM) $(BIN)




