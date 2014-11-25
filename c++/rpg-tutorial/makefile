CC=g++
CFLAGS+=-g -std=c++11 -Wall
LDFLAGS+=

all: game

game:
	$(CC) atlas.cpp main.cpp -o game $(CFLAGS) $(LDFLAGS)

clean:
	rm -f game

.PHONY: all clean
