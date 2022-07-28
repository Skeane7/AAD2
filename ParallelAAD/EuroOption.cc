#include "AAD.h"
#include "EuroOption.h"
#include <iostream>
#include <cmath>
#include <numeric>


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
		//std::cout << "Option Construtor \n";
                payout = 0;
		int Ndays = int(t.value()*255);
		path.resize(Ndays);
		rngs.resize(Ndays);
		dt = r*t/Ndays;
        	dx = sigma*sqrt(t/Ndays);
}

template<typename T>
void EuropeanOption<T>::variables(){
	if(type){
		std::cout << "Call Option\n";
	} else {
		std::cout << "Put Option\n";
	}
        std::cout << "Spot Price = " << s0 << ", Interest rate (r) = " << r << ", Dividend Yield = " << y << "\n" << "Volatility = " << sigma << ", Strike Price = " << k << ", Time(years) = " << t << "\n";
        std::cout << "Option price = " << payout << "\n";
}

template<typename T>
void EuropeanOption<T>::init(){
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
void EuropeanOption<T>::simulate(RNG generator){
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
T EuropeanOption<T>::payoff(){
	if(type == true){
		 return max((s0*path.back())-k,T{0});
	} 
        return max((k-s0*path.back()),T{0});
}


template<typename T>
void EuropeanOption<T>::pricer(RNG generator, const size_t N){
	for(auto i=0; i<N; ++i){
		Number::tape->rewindToMark();
		simulate(generator);
		Number res = payoff();
		res.propagateToMark();
		payout += res;
        }
        payout /= (N*(exp(r*t)));
}

/* @Brief Function to initialise vector of Options */
std::vector<EuropeanOption<Number>> portfolio(const int N, RNG generator){
        std::vector<EuropeanOption<Number>> port;
        char type;
	/* Setting stock parameters */
        Number s0{floor(generator.uniform(50,150))};
        Number r{floor(generator.uniform(0,5))/100};
        Number sigma{floor(generator.uniform(10, 50))/100};
        Number y{0};
	/* Setting option to be a put or a call */
        for(auto i=0;i<N;++i){
                if(generator.uniform(0,1) < 0.5){
                        type = 'p';
                } else {
                        type = 'c';
                }
        	Number t{generator.uniform(1,24)/12};
                double strike = 5*round(s0.value()/5) + 5*round(generator.uniform(-10,10)/5);
                port.emplace_back(EuropeanOption(s0, r, y, sigma, Number{strike}, t, type));
        }
        return port;
}
template class EuropeanOption<Number>;
