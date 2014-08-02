CC=gcc
CFLAGS+=-c -g -Wall
LDFLAGS+=

all: dog

dog.o: dog.c
	$(CC) $(CFLAGS) dog.c

dog: dog.o
	$(CC) $(LDFLAGS) dog.o -o dog

clean:
	rm -rf *.o dog

.PHONY: all clean
