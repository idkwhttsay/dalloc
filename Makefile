CFLAGS=-Wall -Wextra -std=c11 -pedantic -g

heap: main.c heap.c heap.h
	cc $(CFLAGS) -o heap main.c heap.c