CC=gcc
LIBS=-lncurses -lm
DEPS=*.h
main: $(DEPS) *.c
	$(CC) -std=c99 *.c -o main $(LIBS) -g
