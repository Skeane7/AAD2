/*
 * @Author Shane Keane
 * @File EuroOption.cc
 * @Brief Class functions for Options and Portfolio classes
 * @Date 08-06-2022
 * @Version 1.0
 * */

#include "AAD.h"
#include "Option.h"
#include <iostream>
#include <cmath>
#include <numeric>

/* @Brief Class constructor */
template<typename T>
EuropeanOption<T>::EuropeanOption(T _S, T _R, T _Y, T _Sig, T _Strike, T _time, char _type):
                        s0{_S}, r{_R},y{_Y}, sigma{_Sig}, k{_Strike}, t{_time}{
                if(_type == 'c'){
                        type = true;
                } else if (_type == 'p'){
                        type = false;
                } else {
                        std::cout << "Option must be put (p) or call (c)\n";
                        exit(0);
                }		
		payout = 0;
}

/* @Brief Function to print out details of option */
template<typename T>
void EuropeanOption<T>::variables(){
        if(type){
                std::cout << "European Call Option\n";
        } else {
                std::cout << "European Put Option\n";
        }
	
	std::cout << "Spot Price = " << s0 << ", Interest rate (r) = " 
		<< r << ", Dividend Yield = " << y << "\n" 
		<< "Volatility = " << sigma << ", Strike Price = " 
		<< k << ", Time(years) = " << t << "\n";
        std::cout << "Option price = " << payout << "\n";
}

/* @Brief Function to initialise Option variables on tape */
template<typename T>
void EuropeanOption<T>::init(){
	/* Allocating space for vectors to store path and rngs */
	auto Ndays = t.value()*255;
	path.resize(Ndays);
	rngs.resize(Ndays);
	/* Putting variables on tape */
	s0.putOnTape();
        r.putOnTape();
        sigma.putOnTape();
        k.putOnTape();
        t.putOnTape();
        putOnTape(path.begin(), path.end());
	/* Calculating useful constants to reduct operations on tape */
	dt = r*t/Ndays;
        dx = sigma*sqrt(t/Ndays);
}


/* @Brief Function to create path of stock price */
template<typename T>
void EuropeanOption<T>::simulate(RNG generator){
        /* Getting Gaussian RVs */
	auto Ndays{t.value()*255};
        rngs = generator.gaussian(int(Ndays));
        /* Setting path first step to be 1, we will later multiply by S0 */
	path[0]=T{1};
	/* Creating path */
        for(auto j=1;j<int(Ndays);++j){
                path[j]=path[j-1]*(exp(dt) + dx*rngs[j-1]);
        }
}

/* @Brief Payoff function for European Option */
template<typename T>
T EuropeanOption<T>::payoff(){
	if(type){
        	return max((s0*path.back())-k,T{0});
	} else {
		return max(k-(s0*path.back()),T{0});
	}
}

/* @Brief Function to generate path and accumulate payout of option */
template<typename T>
void EuropeanOption<T>::pricer(RNG generator, const size_t N){
	for(auto i=0; i<N; ++i){
		/* Generate stock path */
		simulate(generator);
		/* Store payout */
		Number res = payoff();
		/* Propagate adjoints to marked part of tape*/
		res.propagateToMark();
		/* Accumulate payout to total payout */
		payout += res;
		/* Rewinding tape to mark */
		Number::tape->rewindToMark();
        }
	/* Normalise by number of runs and interest */
        payout /= (N*(exp(r*t)));
}

/* @Brief Class constructor */
template<typename T>
AsianOption<T>::AsianOption(T _S, T _R, T _Y, T _Sig, T _Strike, T _time, char _type):
                        s0{_S}, r{_R},y{_Y}, sigma{_Sig}, k{_Strike}, t{_time}{
                if(_type == 'c'){
                        type = true;
                } else if (_type == 'p'){
                        type = false;
                } else {
                        std::cout << "Option must be put (p) or call (c)\n";
                        exit(0);
                }
                payout = 0;
}

/* @Brief Function to print out details of option */
template<typename T>
void AsianOption<T>::variables(){
        if(type){
                std::cout << "Asian Call Option\n";
        } else {
                std::cout << "Asian Put Option\n";
        }

        std::cout << "Spot Price = " << s0 << ", Interest rate (r) = "
                << r << ", Dividend Yield = " << y << "\n"
                << "Volatility = " << sigma << ", Strike Price = "
                << k << ", Time(years) = " << t << "\n";
        std::cout << "Option price = " << payout << "\n";
}



/* @Brief Function to initialise Option variables on tape */
template<typename T>
void AsianOption<T>::init(){
        /* Allocating space for vectors to store path and rngs */
        auto Ndays = t.value()*255;
        path.resize(Ndays);
        rngs.resize(Ndays);
        /* Putting variables on tape */
        s0.putOnTape();
        r.putOnTape();
        sigma.putOnTape();
        k.putOnTape();
        t.putOnTape();
        putOnTape(path.begin(), path.end());
        /* Calculating useful constants to reduct operations on tape */
        dt = r*t/Ndays;
        dx = sigma*sqrt(t/Ndays);
}


/* @Brief Function to create path of stock price */
template<typename T>
void AsianOption<T>::simulate(RNG generator){
        /* Getting Gaussian RVs */
        auto Ndays{t.value()*255};
        rngs = generator.gaussian(int(Ndays));
        /* Setting path first step to be 1, we will later multiply by S0 */
        path[0]=T{1};
        /* Creating path */
        for(auto j=1;j<int(Ndays);++j){
                path[j]=path[j-1]*(exp(dt) + dx*rngs[j-1]);
        }
}




/* @Brief Payoff function for European Option */
template<typename T>
T AsianOption<T>::payoff(){
        /* Averaging price over path of stock price */
        T average{0};
        for(auto& i: path){
                average += i;
        }
        average *= (s0/(255*t.value()));
        if(type){
                return max(average-k,T{0});
        } else {
                return max(k-average,T{0});
        }
}

/* @Brief Function to generate path and accumulate payout of option */
template<typename T>
void AsianOption<T>::pricer(RNG generator, const size_t N){
        for(auto i=0; i<N; ++i){
                /* Generate stock path */
                simulate(generator);
                /* Store payout */
                Number res = payoff();
                /* Propagate adjoints to marked part of tape*/
                res.propagateToMark();
                /* Accumulate payout to total payout */
                payout += res;
                /* Rewinding tape to mark */
                Number::tape->rewindToMark();
        }
        /* Normalise by number of runs and interest */
        payout /= (N*(exp(r*t)));
}


template class AsianOption<Number>;
template class EuropeanOption<Number>;
