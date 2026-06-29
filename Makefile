# Compiler and flags
CC      := gcc
CFLAGS  := -Wall -g -Isrc/vendor -Isrc/include

# Directories and files
SRC_DIR := src
SRCS    := $(wildcard $(SRC_DIR)/*.c)
BIN     := exefile

.PHONY: all build run clean

all: build run

build: $(BIN)

$(BIN): $(SRCS)
	$(CC) $(CFLAGS) $^ -o $@

run: $(BIN)
	./$(BIN)

clean:
	$(RM) $(BIN)
