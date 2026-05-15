CC = gcc
CFLAGS = -Wall -Wextra -pedantic -g

.PHONY: all clean

all: ccp

ccp: main.c
	$(CC) $(CFLAGS) main.c -o ccp

clean:
	rm -f ccp
