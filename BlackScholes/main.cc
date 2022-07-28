#include <vector>
#include <memory>
#include <string>
#include <queue>
#include <iostream>
#include <cmath>
#include "Number.h"


/* Templated Black Scholes function with approx normal distribution */
template <class T>
/* x[S0, r, y, sigma, K, T] */
T f(T x[6]) {
	std::cout << "df \n";
        auto df = exp(-1*x[1]*x[5]);
	std::cout << "F \n";
        auto F = x[0]*exp((x[1] - x[2])*x[5]);
	std::cout << "std \n";
        auto std = x[3] * sqrt(x[5]);
	std::cout << "d \n";
        auto d = log(F/x[4])/std;
	std::cout << "d1,d2 \n";
        auto d1 = d+0.5*std, d2 = d1-std;
        /* Approximating normal distribution CDF (Ordaz 1991) */
	//auto nd1 = 1 - 0.6931*exp(-1*(((-9*d1-8)/14)*((-9*d1-8)/14)));
	//auto nd2 = 1 - 0.6931*exp(-1*(((-9*d2-8)/14)*((-9*d2-8)/14)));
	/* Exact version using nodes */
	std::cout << "nd1 \n";
	auto nd1 = normal(d1), nd2 = normal(d2);
	std::cout << "c \n";
	auto c = df*(F*nd1 - x[4]*nd2);
        return c;
}




int main() {
		      /* S0,  r ,  y ,sigma,  K , t */
        Number x[6] = { 100., .05, 0.0,  0.2, 100., 1. };

	//std::vector<string> Greeks[6] = { "Delta", "Rho", "a3", "Vega", "strike", "Theta" }
        Number y = f(x);

        y.propagateAdjoints();
	std::cout << "Spot Price = " << x[0].getVal() << ", Interest Rate = " << x[1].getVal() <<
		", Dividend Rate = " << x[2].getVal() << ", Volatility = " << x[3].getVal() <<
		", Strike = " << x[4].getVal() << ", Time = " << x[5].getVal() << "\n";
	std::cout << "Option price = " << y.node()->getResult() << "\n";
	std::cout << "Delta = " << x[0].getAdjoint()  << std::endl;
	std::cout << "Vega = "  << x[3].getAdjoint()  << std::endl;
	std::cout << "Rho = "   << x[1].getAdjoint()  << std::endl;
	std::cout << "Theta = " << x[5].getAdjoint()  << std::endl;
        return 0;
}
