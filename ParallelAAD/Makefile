#Makefile for first AAD program
#

CXX := mpic++
CXXFLAGS := -std=c++17 -lgsl -lm #-Wall #Wextra
EXEC = main
all: $(EXEC)

#Option.o: Option.cc Option.h
#	$(CXX) -c Option.cc $(CXXFLAGS)

EuroOption.o: EuroOption.cc EuroOption.h
	$(CXX) -c EuroOption.cc $(CXXFLAGS)

main: main.cc AAD.h EuroOption.o
	$(CXX) -o main main.cc EuroOption.o $(CXXFLAGS)


#main: main.cc AAD.h Option.o
#	$(CXX) -o main main.cc Option.o $(CXXFLAGS)

#.PHONY:clean
clean:
	rm $(EXEC) *.o 
