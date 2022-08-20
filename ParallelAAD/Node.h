/* 
 * @Author Shane Keane
 * @File Node.h
 * @Brief Node class for AAD implementation
 * @Date 08-06-2022
 * @Version 1.0
 * */

#include <vector>
#include <iostream>
#include <algorithm>
#include <cblas.h> 

class Node {
	/* Friend classes for easy access to necessary parameters */
	friend class Tape;
	friend class Number;
	/* Storing adjoint*/
	double myAdjoint = 0;
	double *myAdjoints;
	/* Storing derivatives and pointers to other adjoints */
	double * myDerivatives;
	double ** myAdjPtrs;
	/* Storing number of adjoints and children */
	static size_t numAdj;
	const size_t numChl;
public: 
	/* @Brief Constructor */
	Node(const size_t N=0) : numChl(N) {
		//std::cout << "Node constructed with " << N << "children \n";
	}
	/* @Brief Function to access adjoints for unary and multi input cases */
	double& adjoint(){
		return myAdjoint;
	}

	double& adjoint(const size_t n){
		return myAdjoints[n];
	}
	/* @Brief Function to propagate adjoints for unary Node */
	void propagateOne(){
		if(myAdjoint == 0 || numChl == 0) {return;}
		/* Adjoint accumulation according to formula */
		for(auto i=0; i<numChl; ++i){
			*(myAdjPtrs[i]) += myDerivatives[i]*myAdjoint;
		}
		/* Can also use cblas to apply formula */
		//cblas_daxpy(numChl, myAdjoint, myDerivatives, 1, *(myAdjPtrs), 1);
	}
};
