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
	friend Tape;
	friend Number;
	/* Storing adjoints for unary and multi input case*/
	double myAdjoint = 0;
	std::vector<double> myAdjoints;
	/* Storing derivatives and pointers to other adjoints */
	std::vector<double> myDerivatives;
	std::vector<std::vector<double>> myAdjPtrs;
	/* Storing number of adjoints and children */
	static size_t numAdj;
	const size_n numChl;
public: 
	/* @Brief Constructor */
	Node(const size_t N) : numChl(N) {
		std::cout << "Node constructed with " << N << "children \n";
	}
	/* @Brief Function to access adjoints for unary and multi input cases */
	double adjoint(){
		return myAdjoint;
	}
	double& adjoint(const size_t n){
		return myAdjoints[n];
	}
	/* @Brief Function to propagate adjoints for unary Node */
	void propagateOne(){
		if(myAdjoint == 0 || numChl == 0) return;
		/* Change arithmetic for cblas function? */
		for(auto i=0; i<numChl; ++i){
			*(myAdjPtrs[i]) += myDerivatives[i]*myAdjoint;
			//cblas_daxpy(numChl, myAdjoint, myDerivatives, 1, *(myAdjPtrs), 1);
		}
	}
	/* @Brief Function to propagate adjoints for Node with Multiple children */
	void propagateMulti() {
		if(numAdj == 0 || std::all_of(myAdjoints.begin(),
		myAdjoints.begin()+numAdj, [](const double& x) {return !(*x)})) return;
		
		for(auto i=0; i<numChl; ++i){
			for(auto j=0; j<numAdj; ++j){
				myAdjPtrs[i][j] += myAdjoints[j]*myDerivatives[i];
				//cblas_daxpy(numAdj, myDerivatives[i], myAdjoints, 1, myAdjPtrs[i], 1);
			}
		}
	}
};
