#include "AAD.h"
#include "EuroOption.h"
#include <iostream>
#include <cmath>
#include <numeric>


template<typename T>
EuropeanCall<T>::EuropeanCall(T _S, T _R, T _Y, T _Sig, T _Strike, T _time):
                        s0{_S}, r{_R},y{_Y}, sigma{_Sig}, k{_Strike}, t{_time}{
                std::cout << "Option Construtor \n";
                payout = 0;
		int Ndays = int(t.value()*255);
		path.resize(Ndays);
		rngs.resize(Ndays);
		dt = r*t/Ndays;
        	dx = sigma*sqrt(t/Ndays);
}

template<typename T>
void EuropeanCall<T>::variables(){
        std::cout << "Spot Price = " << s0 << ", Interest rate (r) = " << r << ", Dividend Yield = " << y << "\n" << "Volatility = " << sigma << ", Strike Price = " << k << ", Time(years) = " << t << "\n";
        std::cout << "Option price = " << payout << "\n";
}

template<typename T>
void EuropeanCall<T>::init(){
	auto Ndays = t*255;
	path.resize(Ndays.value());
	rngs.resize(Ndays.value());
	s0.putOnTape();
        r.putOnTape();
        sigma.putOnTape();
        k.putOnTape();
        t.putOnTape();
        putOnTape(path.begin(), path.end());
}



template<typename T>
void EuropeanCall<T>::simulate(RNG generator){
        auto Ndays{t*255};
        rngs = generator.gaussian(int(Ndays.value()));
	dt = r*t/Ndays.value();
        dx = sigma*sqrt(t/Ndays.value());
        path[0]=T{1};
        for(auto j=1;j<int(Ndays.value());++j){
                path[j]=path[j-1]*(exp(dt) + dx*rngs[j-1]);
        }
}

template<typename T>
T EuropeanCall<T>::payoff(){
        return max((s0*path.back())-k,T{0});
}


template<typename T>
void EuropeanCall<T>::pricer(RNG generator, const size_t N){
	for(auto i=0; i<N; ++i){
		Number::tape->rewindToMark();
		simulate(generator);
		Number res = payoff();
		res.propagateToMark();
		payout += res;
		//payout.propagateToMark();
		//putOnTape(path.begin(), path.end());
		//s0.putOnTape();
		//result = payoff();
		//payout += double(res);
		//std::cout << "Delta = " << s0.adjoint() << "\n";
		//Number::tape->propagateToMark();
        }
        payout /= (N*(exp(r*t)));
}


template class EuropeanCall<Number>;
