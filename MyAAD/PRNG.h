/* 
 * @Author Shane Keane
 * @File RNG.h
 * @Brief RNG class to produce random numbers for MC simulations
 * @Version 1.0
 * @Date 09-06-2022
 */

//#ifdef __RNGvar__
//#define __RNGvar__

#include <gsl/gsl_rng.h>
#include <vector>
#include <algorithm>
#include <gsl/gsl_randist.h>

class RNG {
public: 
	RNG(){};
	/* @Brief Constructor */
	RNG(const size_t seed){
		/* Initialising PRNG */
		gen = gsl_rng_alloc( gsl_rng_mt19937 );
		gsl_rng_set( gen, seed);
	}
	/* @Brief Function to return normal(0,1) RV */
	double gaussian(){
		return gsl_ran_gaussian(gen, 1.0);
	}
	/* @Brief Function to return a vector of normal(0,1) RV */
	std::vector<double> gaussian(const size_t n){
		std::vector<double> rngs(n);
		for_each(rngs.begin(), rngs.end(), [&](double& i){ i=gaussian(); });
		return rngs;
	}
private:
	gsl_rng* gen;
};

//#endif
