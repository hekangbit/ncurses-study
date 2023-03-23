LDFLAGS=-lncurses

all: main.c
	gcc -g -o snake main.c -lncurses
clean:
	rm -f snake
