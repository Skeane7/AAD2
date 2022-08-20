#include <memory>
#include <string>
#include <queue>
#include <cmath>
#include <iostream>
#include "AAD.h"
//#include "Option.h"
#include "Portfolio.h"


size_t Node::numAdj = 1;
Tape globalTape;
thread_local Tape* Number::tape = &globalTape;

int main() {
	/* Ensuring tape is clear */
	Number::tape->rewind();
	/* Creating Portfolio of Options */
	RNG generator2{1875};
	int port_size = 1;
	Portfolio<Number> portfolio{port_size};	
	portfolio.init(generator2);
	std::vector<double> port_value(5,0);
	/* Intialising PRNG generator and number of paths */
	const size_t N = 1000000;
	RNG generator{43121};
	/* Looping over options and accumulating Greeks */
	for(auto i=0;i<port_size;++i){
		/* Putting variables on tape */	
		portfolio.options[i].init();
		/* Mark tape to be able to rewind to */
		Number::tape->mark();
		/* Princing option */
		portfolio.options[i].pricer(generator, N);
		/* Printing data about option */
		portfolio.options[i].variables();
		/* Propagate mark to start */
		Number::propagateMarkToStart();
		/* Accumulate sensitivities */
                port_value[0] += portfolio.options[i].payout.value();
                port_value[1] += portfolio.options[i].s0.adjoint()/N;
                port_value[2] += portfolio.options[i].sigma.adjoint()/N;
                port_value[3] += portfolio.options[i].r.adjoint()/N;
                port_value[4] += portfolio.options[i].t.adjoint()/N;
                Number::tape -> clear();

	}
	std::cout << "Portfolio Value = " << port_value[0] << std::endl;
	std::cout << "Delta = " << port_value[1] << std::endl;
	std::cout << "Vega = "  << port_value[2] << std::endl;
	std::cout << "Rho = "   << port_value[3] << std::endl;
	std::cout << "Theta = " << port_value[4] << std::endl; 
	std::cout << "Using Bump and Reval \n";
        return 0;
}
