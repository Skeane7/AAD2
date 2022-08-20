/* 
 * @Author Shane Keane 17329836
 * @File BumpAndReval.cc
 * @Brief File to implement bump and revalue method to get option greeks 
 * @Date 08-08-2022
 * @Version 1.0
 */

#include <memory>
#include <string>
#include <queue>
#include <cmath>
#include <iostream>
#include "AAD.h"
#include "Portfolio.h"
#include <chrono>

size_t Node::numAdj = 1;
Tape globalTape;
thread_local Tape* Number::tape = &globalTape;


int main() {
	using namespace std;
	using namespace std::chrono;
	Number::tape->rewind();
	/* Intialising Option and PRNG generators */
	int seed = 4221938474;
	RNG generator{seed};
	RNG generator1{seed};
	size_t N = 25000;
	/* Initialise Option and price*/
	Option<double> EuroCall(100, 0.01, 0.0,0.1, 105, 0.5, "ec");
	//Option<Number> EuroCall(Number{100}, Number{0.01}, Number{0.0},
	//		Number{0.1}, Number{105}, Number{0.5}, "ec");
	EuroCall.init();
	//Number::tape->mark();
	auto start = high_resolution_clock::now();
	EuroCall.pricer(generator, N);
	//EuroCall.variables();
	//Number::propagateMarkToStart();
	/* Calculate Greeks */
	bumpReval(EuroCall,seed, N);
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(stop - start);
        /*std::cout << "Price = " << EuroCall.payout << std::endl;
        std::cout << "Delta = " << EuroCall.s0.adjoint()/N << std::endl;
        std::cout << "Vega = "  << EuroCall.sigma.adjoint()/N << std::endl;
        std::cout << "Rho = "   << EuroCall.r.adjoint()/N << std::endl;
        std::cout << "Theta = " << EuroCall.t.adjoint()/N << std::endl;	
	
	std::cout << EuroCall.payout << std::endl;
        std::cout << EuroCall.s0.adjoint()/N << std::endl;
        std::cout << EuroCall.sigma.adjoint()/N << std::endl;
        std::cout << EuroCall.r.adjoint()/N << std::endl;
        std::cout << EuroCall.t.adjoint()/N << std::endl;
	*/
	cout << duration.count() << endl;



	N = 1000000;
	//Number::tape->clear();
	EuroCall.payout = 0;
	EuroCall.init();
	//Number::tape->mark();
	start = high_resolution_clock::now();
	//Number::tape->mark();
	EuroCall.pricer(generator1, N);
	//EuroCall.variables();
	//Number::propagateMarkToStart();
	bumpReval(EuroCall,seed, N);
        stop = high_resolution_clock::now();
	/*std::cout << "Price = " << EuroCall.payout << std::endl;
	std::cout << "Delta = " << EuroCall.s0.adjoint()/N << std::endl;
        std::cout << "Vega = "  << EuroCall.sigma.adjoint()/N << std::endl;
        std::cout << "Rho = "   << EuroCall.r.adjoint()/N << std::endl;
        std::cout << "Theta = " << EuroCall.t.adjoint()/N << std::endl;
        

	std::cout << EuroCall.payout << std::endl;
        std::cout << EuroCall.s0.adjoint()/N << std::endl;
        std::cout << EuroCall.sigma.adjoint()/N << std::endl;
        std::cout << EuroCall.r.adjoint()/N << std::endl;
        std::cout << EuroCall.t.adjoint()/N << std::endl; */
	duration = duration_cast<microseconds>(stop - start);
        cout << duration.count() << endl;
	
	return 0;
}
