#Makefile for first AAD program
#

CXX := g++
CXXFLAGS := -std=c++17 -lgsl -lm -lblas -O3#-Wall -Wextra
EXEC = main BumpReval basicEx
all: $(EXEC)

Option.o: Option.cc Option.h PRNG.h
	$(CXX) -c Option.cc $(CXXFLAGS)

main: main.cc Option.o Portfolio.h 
	$(CXX) -o main main.cc Option.o $(CXXFLAGS)

BumpReval: BumpAndReval.cc Option.o Portfolio.h
	$(CXX) -o BumpReval BumpAndReval.cc Option.o $(CXXFLAGS)

basicEx: basicExample.cc
	$(CXX) -o basicEx basicExample.cc $(CXXFLAGS)

#.PHONY:clean
clean:
	rm $(EXEC) *.o
