#Makefile for first AAD program
#

CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -lgsl -lm
EXEC = main 
all: $(EXEC)

Option.o: Option.cc Option.h PRNG.h
	$(CXX) -c Option.cc $(CXXFLAGS)

main: main.cc Option.o PRNG.h
	$(CXX) -o main main.cc Option.o $(CXXFLAGS)

#.PHONY:clean
clean:
	rm $(EXEC)
