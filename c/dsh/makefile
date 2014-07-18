CC=gcc
CFLAGS+=-g -Wall
LDFLAGS+=-lreadline

all: dsh

dsh: dsh.c
	$(CC) $(CFLAGS) -o dsh dsh.c $(LDFLAGS)

clean:
	rm -rf dsh

.PHONY: all clean
