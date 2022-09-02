#include <iostream>
#include "AAD.h"

/* Initialise Tape for use in Node Class */
Tape globalTape;
thread_local Tape* Number::tape = &globalTape;

/* Templated Target Function */
template<typename T>
T f(T X, T Y, T Z){
	return X*Y + 2*Z*Z;
}

int main(){
	/* Clear the tape (Not Necessary at start of program but good practice */
	Number::tape->clear();
	/* Initialise Variables */
	Number X{1.0};
	Number Y{2.0};
	Number Z{3.0};
	/* Call Function (Forward Sweep of Calculation Graph) */
	Number result = f(X, Y, Z);
	/* Propagate Adjoints (Reverse Sweep) */
	result.propagateToStart();
	/* Print Adjoints */
	std::cout << X.adjoint() << std::endl;
	std::cout << Y.adjoint() << std::endl;
	std::cout << Z.adjoint() << std::endl;
	return 0;
}
