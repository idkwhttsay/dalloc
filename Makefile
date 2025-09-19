CFLAGS=-Wall -Wextra -std=c11 -pedantic -g

heap: main.c
	cc $(CFLAGS) -o heap main.c