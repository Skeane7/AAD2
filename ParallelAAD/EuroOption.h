#include "PRNG.h"

template<class T>
class EuropeanOption {
public:
        /* @Brief Class constructor */
        EuropeanOption(T _S, T _R, T _Y, T _Sig, T _Strike, T _time, char _type);
        /* @Brief Function to simulate using Monte Carlo, overwrite later */
        void simulate(RNG generator);
        /* @Brief Function to print option price and other variables to screen */
        void variables();
	/* @Brief Function to load variables to tape */
        void tapeLoad();
	/* @Brief Payoff function */
	T payoff();
	/* @Brief Function to price option */
	void pricer(RNG generator, const size_t N);
	/* @Brief Intialisation function */
	void init();
//protected:
        T s0; /* Spot price */
        T r; /* Interest rate */
        T y; /* Dividend rate */
        T sigma; /* Volatility */
        T k; /* Strike */
        T t; /* Time */
        T payout; /* Payoff */
	T result;
	std::vector<T> path;
	T dt;
	T dx;
	std::vector<double> rngs; 
	bool type; 
};

std::vector<EuropeanOption<Number>> portfolio(const int N, RNG generator);
