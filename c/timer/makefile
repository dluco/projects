CC=gcc
CFLAGS=-g -c -ansi -Wall

all: timer

timer: timer.o
	$(CC) timer.o -o timer

timer.o: timer.c
	$(CC) $(CFLAGS) timer.c

clean:
	-rm -f timer timer.o
