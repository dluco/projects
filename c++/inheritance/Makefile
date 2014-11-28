# Makefile for "Introduction to Inheritance in C++" Example

# ***************************************************************
# Parameters to control Makefile operation

CXX = g++
CXXFLAGS = -ansi -pedantic -Wall -g

# ***************************************************************
# Entries to bring the executable up to date

empltest: empltest.o employee.o manager.o supervisor.o
	$(CXX) $(CXXFLAGS) -o empltest \
            empltest.o employee.o manager.o supervisor.o

empltest.o: employee.h manager.h supervisor.h

employee.o: employee.h

manager.o: manager.h employee.h

# supervisor.o target depends on where you put Supervisor in class hierarchy.
# YOU FILL THIS IN!
