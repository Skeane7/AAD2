#ifndef PORTFOLIO

#define PORTFOLIO


#include "Option.h"
#include <vector>
#include <string> 

template <class T>
class Portfolio {
public:
	/* Class Constructor */
	Portfolio(int size) : N{size} {

	}

	/* Initialise vector with random options */
	void init(RNG generator) {
		std::string type;
        	Number s0{floor(generator.uniform(50,150))};
 	        Number r{floor(generator.uniform(0,5))/100};
        	Number sigma{floor(generator.uniform(10, 50))/100};
        	Number y{0};
		for(auto i=0;i<N;++i){
			switch(int(generator.uniform(0,4))){
				case 0:
					type = "ac";
				case 1:
					type = "ap";
				case 2: 
					type = "ec";
				case 3:
					type = "ep";
			}
			Number t{generator.uniform(1,24)/12};
			double strike = 5*round(s0.value()/5) + 5*round(generator.uniform(-10,10)/5);
			options.emplace_back(Option(s0, r, y, sigma, Number{strike}, t, type));
		}
	}
	/* Variables */
	std::vector<Option<T>> options; /* Vector to store options */
	int N; /* Number of optons */
};

template class Portfolio<Number>;

#endif // PORTFOLIO
