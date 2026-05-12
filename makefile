CC = cc
CFLAGS = -Wall -Wextra -pedantic

.PHONY: all clean

all: ccp

ccp: main.c
	$(CC) $(CFLAGS) main.c -o ccp

clean:
	rm -f ccp
