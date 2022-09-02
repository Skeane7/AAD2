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
	/* Intialising Option and PRNG generators */
	size_t seed = 42;
	RNG generator{seed};
	RNG generator1{seed};
	size_t N = 100000;
	/* Initialise Option and price*/
	Option<double> EuroCall(100, 0.01, 0.0,0.1, 105, 0.5, "ec");
	EuroCall.init();
	auto start = high_resolution_clock::now();
	EuroCall.pricer(generator, N);
	EuroCall.variables();
	/* Calculate Greeks */
	bumpReval(EuroCall,seed, N);
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(stop - start);
        
	cout << "Time taken = " << duration.count() << endl;
	return 0;
}
