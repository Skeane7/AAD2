/* 
 * @Author Shane Keane
 * @File Option.h
 * @Brief File containing classes for European and Asian options
 * @Version 1.0
 * @Date 09-06-2022
 */

#include "PRNG.h"

template<class T>
class Option {
public: 
	/* @Brief Class constructor */
	Option(T _S, T _R, T _Y, T _Sig, T _Strike, T _time);
	/* @Brief Function to simulate using Monte Carlo, overwrite later */
	std::vector<T> simulate(RNG generator);
	/* @Brief Function to print option price and other variables to screen */
	void variables();
	void tapeLoad();
//protected:	
	const T s0; /* Spot price */
	const T r; /* Interest rate */
	const T y; /* Dividend rate */
	const T sigma; /* Volatility */
	const T k; /* Strike */
	T t; /* Time */
	//T payout; /* Payoff 
};


template <class T>
class EuropeanCall : public Option<T> {
public:
	/* @Brief Class constructor */
	EuropeanCall(T _S, T _R, T _Y, T _Sig, T _Strike, T _time);
	/* @Brief Overwriting payoff function */
	//void payofddf(std::vector<T> path);
	void payoff(std::vector<T> path);
	/* @Brief Function to price option */
	void pricer(RNG generator, const size_t N, Tape tape);
	T payout;
	std::vector<T> path;
	std::vector<double> rngs;
};

template<class T>
class AsianCall: public Option<T> {
public:
	/* @Brief Class constructor */
	AsianCall(T _S, T _R, T _Y, T _Sig, T _Strike, T _time);
	/* @Brief Overwriting payoff function */
	void payoff(std::vector<T> path);
	/* @Brief Function to price option */
	void pricer(RNG generator, const size_t N);
	T payout;
};
