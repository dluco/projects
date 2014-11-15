PROG = cube

CC = g++
CFLAGS = -Wall
LIBS = -lGLU -lGL -lglut

SRCS = main.cpp imageloader.cpp

all: $(PROG)

$(PROG): $(SRCS)
	$(CC) -o $(PROG) $(SRCS) $(CFLAGS) $(LIBS)

clean:
	rm -f $(PROG)
