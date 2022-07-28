/*
 * @Author Shane Keane
 * @File Option.cc
 * @Brief File containing class functions for European and Asian options
 * @Version 1.0
 * @Date 09-06-2022
 */

#include "AAD.h"
#include "Option.h"
#include <iostream>
#include <cmath> 
#include <numeric>

template<typename T>
Option<T>::Option(T _S, T _R, T _Y, T _Sig, T _Strike, T _time):
			s0{_S}, r{_R},y{_Y}, sigma{_Sig}, k{_Strike}, t{_time}{
		std::cout << "Option Construtor \n";
		//T payout{0};
}


template<typename T>
void Option<T>::variables()
{
	std::cout << "Spot Price = " << s0 << ", Interest rate (r) = " << r << ", Dividend Yield = " << y << "\n" << "Volatility = " << sigma << ", Strike Price = " << k << ", Time(years) = " << t << "\n";
	//std::cout << "Option price = " << payout << "\n";
}
/*
template<typename T>
std::vector<double> Option<T>::simulate(RNG generator){
        auto Ndays = t*255.;
	double NdaysCount = double(Ndays);
	std::vector<double> rngs = generator.gaussian(NdaysCount);
        std::vector<double> path(255);
        path[0] = 1.;
        for(auto j=1;j<NdaysCount;++j){
                path[j] = path[j-1]*(exp(this->r.value()/(255*this->t.value())) + this->sigma.value()*sqrt(1/(255*this->t.value()))*rngs[j]);
        }
        return path;
}
*/
template<typename T>
std::vector<T> Option<T>::simulate(RNG generator){
        auto Ndays{t*255.};
        std::vector<double> rngs = generator.gaussian(Ndays.value());
        std::vector<T> path(int(Ndays.value()));
	auto dt{this->r*this->t/Ndays.value()};
	auto dx{this->sigma*sqrt(this->t/Ndays.value())};
        path[0]=T{1};
        for(auto j=1;j<int(Ndays.value());++j){
                path[j]=path[j-1]*(exp(dt) + dx*rngs[j]);
        }
        return path;
}


template<typename T>
EuropeanCall<T>::EuropeanCall(T _S, T _R, T _Y, T _Sig, T _Strike, T _time):
	Option<T>{_S, _R, _Y, _Sig, _Strike, _time}, payout{0}{}

template<typename T>
void EuropeanCall<T>::payoff(std::vector<T> path){
	this->payout += max((path.back()*this->s0)-this->k,T{0});
}

template<typename T>
void EuropeanCall<T>::pricer(RNG generator, const size_t N, Tape tape){
	std::vector<T> path;
        for(auto i=0; i<N; ++i){	
		tape.rewindToMark();
		path = this->simulate(generator);
                payoff(path);
		payout.propagateMarkToStart();
        }
        this->payout /= (N*(exp(this->r*this->t)));
}

template<typename T>
void Option<T>::tapeLoad(){
	//(s0)->putOnTape();
        //Call.r.putOnTape();
        //Call.sigma.putOnTape();
        //Call.k.putOnTape();
        //Call.t.putOnTape();
        //Call.payout.putOnTape();	
}

template<typename T>
AsianCall<T>::AsianCall(T _S, T _R, T _Y, T _Sig, T _Strike, T _time):
        Option<T>{_S, _R, _Y, _Sig, _Strike, _time}, payout{0}{}

template<typename T>
void AsianCall<T>::payoff(std::vector<T> path){
	T average{0};
	for(auto& i: path){
		average += i;
	}
	//std::accumulate(path.begin(), path.end(), average);
	average *= (this->s0/(255.*this->t.value()));
        this->payout += max(average-(this->k),0.0);
}

template<typename T>
void AsianCall<T>::pricer(RNG generator, const size_t N){
        std::vector<T> path;
        for(auto i=0; i<N; ++i){
                path = this->simulate(generator);
                this->payoff(path);
        }
        this->payout /= (N*(exp(this->r*this->t)));
}

//template class Option<double>;
//template class EuropeanCall<double>;
//template class AsianCall<double>;
template class Option<Number>;
template class EuropeanCall<Number>;
template class AsianCall<Number>;
