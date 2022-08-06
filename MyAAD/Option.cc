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
#include <string> 
#include <sys/time.h>
#include <sys/resource.h>

size_t GetMemoryUsage() {
	rusage ru;
	getrusage(RUSAGE_SELF, &ru);
	return (size_t)(ru.ru_maxrss*1024L);
}

/* @Brief Class constructor */
template<typename T>
Option<T>::Option(T _S, T _R, T _Y, T _Sig, T _Strike, T _time, const std::string _type):
                        s0{_S}, r{_R},y{_Y}, sigma{_Sig}, k{_Strike}, t{_time}{
                if(_type.compare("ac")){
                        type = true;
			style = true;
                } else if(_type.compare("ap")){
                        type = false;
			style = true;
                } else if(_type.compare("ec")){
			type = true;
			style = false;
		} else if(_type.compare("ep")){
			type = false;
			style = false;
		} else {
                        std::cout << "Option must be Asian (a) or European (e) put (p) or call (c) \n";
                        exit(0);
                }
		/* Initialising payout to be 0 */
                payout = 0;
}

/* @Brief Function to print out details of option */
template<typename T>
void Option<T>::variables() {
        if(type && style){
                std::cout << "Asian Call Option\n";
        } else if(!type && style) {
                std::cout << "Asian Put Option\n";
        } else if(type && !style) {
		std::cout << "European Call Option \n";
	} else if(!type && !style) {
		std::cout << "European Put Optino \n";
	}
        std::cout << "Spot Price = " << s0 << ", Interest rate (r) = "
                << r << ", Dividend Yield = " << y << "\n"
                << "Volatility = " << sigma << ", Strike Price = "
                << k << ", Time(years) = " << t << "\n";
        std::cout << "Option price = " << payout << "\n";
}



/* @Brief Function to initialise Option variables on tape */
template<typename T>
void Option<T>::init(){
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
void Option<T>::simulate(RNG generator){
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
T Option<T>::payoff(){
        /* Averaging price over path of stock price */
        if(style){

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
	return max(s0*path.back()-k, T{0});
}

/* @Brief Function to generate path and accumulate payout of option */
template<typename T>
void Option<T>::pricer(RNG generator, const size_t N){
        for(auto i=0; i<N; ++i){
                /* Generate stock path */
                simulate(generator);
		std::cout << "Memory Usage = " << GetMemoryUsage() << "\n";
                /* Store payout */
                Number res = payoff();
                /* Propagate adjoints to marked part of tape*/
                res.propagateToMark();
                /* Accumulate payout to total payout */
                payout += res;
		std::cout << "Memory Usage = " << GetMemoryUsage() << "\n";
                /* Store payout */
                /* Rewinding tape to mark */
                Number::tape->rewindToMark();
        }
        /* Normalise by number of runs and interest */
        payout /= (N*(exp(r*t)));
}



template class Option<Number>;
