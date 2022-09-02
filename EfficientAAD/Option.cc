/*
 *
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

/* Functions to return value of Number and double to help with templates */
double getvalue(Number x){
	return x.value();
}

double getvalue(double x){
	return x;
}

/* @Brief Class constructor */
template<typename T>
Option<T>::Option(T _S, T _R, T _Y, T _Sig, T _Strike, T _time, const std::string _type):
                        s0{_S}, r{_R},y{_Y}, sigma{_Sig}, k{_Strike}, t{_time}{
                if(!_type.compare("ac")){
                        type = true;
			style = true;
                } else if(!_type.compare("ap")){
                        type = false;
			style = true;
                } else if(!_type.compare("ec")){
			type = true;
			style = false;
		} else if(!_type.compare("ep")){
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
        } else if((!type) && style) {
                std::cout << "Asian Put Option\n";
        } else if(type && (!style)) {
		std::cout << "European Call Option \n";
	} else if((!type) && (!style)) {
		std::cout << "European Put Option \n";
	}
        std::cout << "Spot Price = " << s0 << ", Interest rate (r) = "
                << r << ", Dividend Yield = " << y << "\n"
                << "Volatility = " << sigma << ", Strike Price = "
                << k << ", Time(years) = " << t << "\n";
        std::cout << "Option price = " << payout << "\n";
}



/* @Brief Function to initialise Option variables on tape */
template<>
void Option<Number>::init(){
        /* Allocating space for vectors to store path and rngs */
        int Ndays = int(t.value()*252);
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
        dt = (r-sigma*sigma/2.0)*t/Ndays;
        dx = sigma*sqrt(t/Ndays);
}

template<>
void Option<double>::init(){
        /* Allocating space for vectors to store path and rngs */
        int Ndays = int(t*252);
        path.resize(Ndays);
        rngs.resize(Ndays);
        /* Calculating useful constants to reduct operations on tape */
        dt = (r-sigma*sigma/2.0)*t/Ndays;
        dx = sigma*sqrt(t/Ndays);
	payout = 0;
}


/* @Brief Function to create path of stock price */
template<class T>
void Option<T>::simulate(RNG generator){
        /* Getting Gaussian RVs */
        auto Ndays{t*252};
        rngs = generator.gaussian(int(getvalue(Ndays)));
        /* Setting path first step to be 1, we will later multiply by S0 */
        path[0]=T{1};
        /* Creating path */
        for(auto j=1;j<int(getvalue(Ndays));++j){
		path[j]=path[j-1]*exp(dt + dx*rngs[j-1]);
	}
}

/* @Brief Payoff function for Option */
template<typename T>
T Option<T>::payoff(){
        /* Averaging price over path of stock price */
	auto Ndays = t*252;
	if(style){
		T average{0};
		for(auto& i: path){
			average += i;
		}
		average *= (s0/getvalue(Ndays));
		if(type){
			return max(exp(-r*t)*(average-k),T{0});
		} else {
			return max(exp(-r*t)*(k-average),T{0});
		}
	}

	if(type){
		return max(exp(-r*t)*(s0*path.back()-k), T{0});
	} else if(!type) {
		return max(exp(-r*t)*(k-s0*path.back()), T{0});
	}
	throw invalid_argument("Option Type not Valid");
	return T{0};
}

/* @Brief Function to generate path and accumulate payout of option for Number class */
template<>
void Option<Number>::pricer(RNG generator, const size_t N){
        for(size_t i=0; i<N; ++i){
                /* Generate stock path */
                simulate(generator);
                /* Store payout */
                Number res = payoff();
                /* Propagate adjoints to marked part of tape*/
                res.propagateToMark();
		/* Accumulate payout to total payout */
                payout += res;
		//std::cout << "Memory Usage = " << GetMemoryUsage() << "\n";
                /* Rewinding tape to mark */
                Number::tape->rewindToMark();
        }
        /* Normalise by number of iterations and interest */
        payout /= N;
}

template<>
void Option<double>::pricer(RNG generator, const size_t N){
        for(size_t i=0; i<N; ++i){
                /* Generate stock path */
		simulate(generator);
                payout += payoff();
        }
        /* Normalise by number of runs and interest */
        payout /= N;
}

template<typename T> 
void bumpReval(Option<T> product, const size_t seed, const size_t N){
	RNG generator{seed};
	Option<T> temp1 = product;
	Option<T> temp2 = product;
	Option<T> temp3 = product;
	Option<T> temp4 = product;
	
	temp1.s0 *= 1.01;	
	temp1.init();
	temp1.pricer(generator, N);
	std::cout << "Delta = " << (temp1.payout-product.payout)/(product.s0*0.01) << "\n"; 

        RNG generator2{seed};
        temp3.sigma *= 1.01;
        temp3.init();
        temp3.pricer(generator2, N);
        std::cout << "Vega = " << (temp3.payout-product.payout)/(temp3.sigma*.01) << "\n";

	RNG generator1{seed};
	temp2.r *= 1.01;
        temp2.init();
	temp2.pricer(generator1, N);
	std::cout << "Rho = " << (temp2.payout-product.payout)/(temp2.r*.01) << "\n";

	RNG generator3{seed};
        temp4.t *= 1.01;
        temp4.init();
        temp4.pricer(generator3, N);
        std::cout << "Theta = " << (temp4.payout-product.payout)/(temp4.t*.01) << "\n";
}


template class Option<Number>;
template class Option<double>;
template void bumpReval<Number>(Option<Number>, const size_t, const size_t);
template void bumpReval<double>(Option<double>, const size_t, const size_t);
