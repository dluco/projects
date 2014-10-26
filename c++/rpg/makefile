CC = g++
CFLAGS += -g -std=c++11 -Wall
LDFLAGS +=

SRC = $(wildcard *.cpp)
OBJ = $(SRC:.cpp=.o)

all: last-stand

.cpp.o:
	$(CC) -c $< $(CFLAGS)

last-stand: $(OBJ)
	$(CC) -o $@ $(OBJ) $(LDFLAGS)

clean:
	${RM} last-stand

.PHONY: all clean
