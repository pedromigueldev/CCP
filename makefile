CC = gcc
CFLAGS = -Wall -Wextra -g -std=c11

.PHONY: all clean

all: ccp

ccp: main.c
	$(CC) $(CFLAGS) main.c -o ccp

clean:
	rm -f ccp
