/*
 * @Author Shane Keane
 * @File singleOption.cc
 * @Brief Applying parallel AAD implementation to 
 * 	  calculate Greeks for a single Option
 * @Date 08-06-2022
 * @Version 1.0
 * */

#include <memory>
#include <string>
#include <queue>
#include <cmath>
#include <iostream>
#include <mpi.h>
#include "AAD.h"
#include "Option.h"

size_t Node::numAdj = 1;

Tape globalTape;
thread_local Tape* Number::tape = &globalTape;

int main(int argc, char **argv) {
	/* Initialising MPI arguments */
	int rank, nprocs;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
	Number::tape->rewind();
        /* S0,  r ,  y ,sigma,  K , t */
	Number S0{100};
	Number r{0.01};
	Number y{0.00};
	Number sigma{0.1};
	Number K{105};
	Number t{0.5};
	/* Option constructor */
	Option<Number> Call{S0, r, y, sigma, K, t, "ec"};
	/* Mark tape to be able to rewind to */
	const size_t N = 1000000;
	RNG generator{42};
	generator.RNGskip(N/nprocs, rank);
	/* Putting variables on tape */
	Call.init();
	/* Marking tape */
	Number::tape->mark();
	/* Pricing option */
	Call.pricer(generator, N/nprocs);
	/* Propagate mark to start */
	Call.payout.propagateToStart();
	/* Print sensitivities */
	double Delta = (Call.s0).adjoint()/N;
	double Vega = (Call.sigma).adjoint()/N;
	double Rho = (Call.r).adjoint()/N;
	double Theta = (Call.t).adjoint()/N;
	
	double globalDelta = 0;
	double globalVega = 0;
	double globalRho = 0;
	double globalTheta = 0;
	
	MPI_Reduce(&Delta, &globalDelta, 1, MPI_DOUBLE,
		   MPI_SUM, 0, MPI_COMM_WORLD);
	MPI_Reduce(&Vega, &globalVega, 1, MPI_DOUBLE,
                   MPI_SUM, 0, MPI_COMM_WORLD);
	MPI_Reduce(&Rho, &globalRho, 1, MPI_DOUBLE,
                   MPI_SUM, 0, MPI_COMM_WORLD);
	MPI_Reduce(&Theta, &globalTheta, 1, MPI_DOUBLE,
                   MPI_SUM, 0, MPI_COMM_WORLD);

	/* Printing Greeks to screen */
	if(rank == 0) {
		Call.variables();
		std::cout << "Delta = " << globalDelta << std::endl;
		std::cout << "Vega = "  << globalVega  << std::endl;
		std::cout << "Rho = "   << globalRho   << std::endl;
		std::cout << "Theta = " << globalTheta << std::endl; 
		MPI_Finalize();
	}
	
        return 0;
}
