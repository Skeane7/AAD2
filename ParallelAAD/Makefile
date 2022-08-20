#Makefile for first AAD program
#

CXX := mpic++
CXXFLAGS := -std=c++17 -lgsl -lm #-Wall #Wextra
EXEC = main main2
all: $(EXEC)

#Option.o: Option.cc Option.h
#	$(CXX) -c Option.cc $(CXXFLAGS)

Option.o: Option.cc Option.h
	$(CXX) -c Option.cc $(CXXFLAGS)

main: main.cc AAD.h Option.o
	$(CXX) -o main main.cc Option.o $(CXXFLAGS)


main2: main2.cc AAD.h Option.o
	$(CXX) -o main2 main2.cc Option.o $(CXXFLAGS)

#.PHONY:clean
clean:
	rm $(EXEC) *.o 
