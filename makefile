CC = gcc
DEBUG_FLAGS     = -Wpedantic -g -Wall -Wextra -Wconversion -Wsign-conversion -fsanitize=address -fsanitize=undefined -fsanitize=leak -std=c11
CFLAGS          = $(DEBUG_FLAGS)

.PHONY: all clean

all: ccp

ccp: main.c
	$(CC) $(CFLAGS) main.c -o ccp

clean:
	rm -f ccp
