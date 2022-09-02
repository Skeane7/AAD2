#Makefile for first AAD program
#

CXX := mpic++
CXXFLAGS := -std=c++17 -lgsl -lm -O3 -Wall 
EXEC = singleOption multiOption
all: $(EXEC)


Option.o: Option.cc Option.h
	$(CXX) -c Option.cc $(CXXFLAGS)

multiOption: multiOption.cc AAD.h Option.o
	$(CXX) -o multiOption multiOption.cc Option.o $(CXXFLAGS)


singleOption: singleOption.cc AAD.h Option.o
	$(CXX) -o singleOption singleOption.cc Option.o $(CXXFLAGS)

#.PHONY:clean
clean:
	rm $(EXEC) *.o 
