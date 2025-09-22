CFLAGS=-Wall -Wextra -std=c11 -pedantic -g

heap: main.c heap.c heap.h
	gcc $(CFLAGS) -o heap main.c heap.c