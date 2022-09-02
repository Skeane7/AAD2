#Makefile for first AAD program
#

CXX := g++
CXXFLAGS := -Wall -Wextra -std=c++17
EXEC = main
all: $(EXEC)

main: main.cc Node.h Number.h
	$(CXX) -o main main.cc $(CXXFLAGS)

#.PHONY:clean
clean:
	rm $(EXEC)
