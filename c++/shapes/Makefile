CC = g++
CFLAGS += -g -Wall
LDFLAGS += -Wall

SRC = $(wildcard *.cpp)
OBJ = ${SRC:.cpp=.o}

all: shapes

.cpp.o:
	${CC} -c ${CFLAGS} $<

shapes: ${OBJ}
	${CC} -o $@ ${OBJ} ${LDFLAGS}

clean:
	${RM} shapes ${OBJ}

.PHONY: all clean
