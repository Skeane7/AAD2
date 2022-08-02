#include <memory>
#include <string>
#include <queue>
#include <cmath>
#include <iostream>
#include "AAD.h"
#include "Option.h"


size_t Node::numAdj = 1;
//bool Tape::multi = false;

Tape globalTape;
thread_local Tape* Number::tape = &globalTape;


/* Templated Black Scholes function with approx normal distribution */
template <class T>
/* x[S0, r, y, sigma, K, T] */
T f(T S0, T r, T y, T sigma, T K, T t) {
        //std::cout << "df \n";
        auto df = exp(-r*t);
        //std::cout << "F \n";
        auto F = S0*exp((r - y)*t);
        //std::cout << "std \n";
        auto std = sigma * sqrt(t);
        //std::cout << "d \n";
        auto d = log(F/K)/std;
        //std::cout << "d1,d2 \n";
        auto d1 = d+0.5*std, d2 = d1-std;
        //std::cout << "nd1 \n";
        auto nd1 = normalCdf(d1), nd2 = normalCdf(d2);
        //std::cout << "c \n";
        auto c = df*(F*nd1 - K*nd2);
        return c;
}


int main() {
	Number::tape->rewind();
        /* S0,  r ,  y ,sigma,  K , t */
	Number S0{100};
	Number r{0.01};
	Number y{0.00};
	Number sigma{0.1};
	Number K{105};
	Number t{1};
	/* Option constructor */
	AsianOption<Number> Call{S0, r, y, sigma, K, t, 'c'};
	/* Mark tape to be able to rewind to */
	const size_t N = 250000;
	RNG generator{43121};
	/* Putting variables on tape */
	Call.init();
	/* Marking tape */
	Number::tape->mark();
	/* Pricing option */
	Call.pricer(generator, N);
	/* Printing data about option */
	Call.variables();
	/* Propagate mark to start */
	Number::propagateMarkToStart();
	/* Print sensitivities */
	auto Delta = (Call.s0).adjoint()/N;
	auto Vega = (Call.sigma).adjoint()/N;
	auto Rho = (Call.r).adjoint()/N;
	auto Theta = (Call.t).adjoint()/N;
	std::cout << "Delta = " << Delta << std::endl;
	std::cout << "Vega = "  << Vega  << std::endl;
	std::cout << "Rho = "   << Rho   << std::endl;
	std::cout << "Theta = " << Theta << std::endl; 
        return 0;
}
