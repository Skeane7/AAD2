#include <memory>
#include <string>
#include <queue>
#include <cmath>
#include <iostream>
#include <mpi.h>
#include "AAD.h"
#include "EuroOption.h"

size_t Node::numAdj = 1;
bool Tape::multi = false;

Tape globalTape;
thread_local Tape* Number::tape = &globalTape;

int decomp1d(int port_size, int nprocs, int rank, int *start, int *end){
	*start = rank*port_size/nprocs;
	*end   = (rank+1)*port_size/nprocs;
	return 0;
}

int main(int argc, char **argv) {
	/* Initialising MPI arguments */
	int rank, nprocs;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
	/* Rewind tape to make sure it is clear */
	Number::tape->rewind();
	/* Initailise and generate portfolio of options */
	RNG generator2{1234};
	int portfolio_size = 100;
	std::vector<EuropeanOption<Number>> port = portfolio(portfolio_size, generator2);
	/* Vector to hold portfolio value and Greeks */
	std::vector<double> port_value(5,0);	
	double skip = portfolio_size/nprocs;	
	const size_t N = 100000;
	RNG generator{43121};
	int start;
	int end;
	if(decomp1d(portfolio_size, nprocs, rank, &start, &end)!=0) exit(0);
	std::cout << "Rank = " << rank << ", Start = " << start << ", End = " << end << std::endl; 
	if(rank != 0){
		for(auto i=0;i<start;++i){
			generator.RNGskip(int(255*port[i].t.value())*N, 1);
		}
	}
	/* Looping over options to calculate price and Greeks */
	for(auto i=start;i<end;++i){
		/* Putting variables on tape */
		port[i].init();
		//generator.RNGskip(port[i].t.value()*N/nprocs, rank);
		/* Marking tape */
		Number::tape->mark();
		/* Pricing option */
		port[i].pricer(generator, N);
		/* Propagate mark to start */
		Number::propagateMarkToStart();
		/* Print sensitivities */
		std::cout << "Rank = " << rank << "\n";
		port[i].variables();
		port_value[0] += port[i].payout.value();
		port_value[1] += port[i].s0.adjoint()/N;
		port_value[2] += port[i].sigma.adjoint()/N;
		port_value[3] += port[i].r.adjoint()/N;
		port_value[4] += port[i].t.adjoint()/N;
		Number::tape -> clear();
	}
	/* Printing Greeks to screen */
	MPI_Barrier(MPI_COMM_WORLD);
	double globalPrice{0};
	double globalDelta{0};
	double globalVega{0};
	double globalRho{0};
	double globalTheta{0};

	MPI_Reduce(&port_value[0], &globalPrice, 1, MPI_DOUBLE,
			MPI_SUM, 0, MPI_COMM_WORLD);
	MPI_Reduce(&port_value[1], &globalDelta, 1, MPI_DOUBLE,
		   MPI_SUM, 0, MPI_COMM_WORLD);
	MPI_Reduce(&port_value[2], &globalVega, 1, MPI_DOUBLE,
                   MPI_SUM, 0, MPI_COMM_WORLD);
	MPI_Reduce(&port_value[3], &globalRho, 1, MPI_DOUBLE,
                   MPI_SUM, 0, MPI_COMM_WORLD);
	MPI_Reduce(&port_value[4], &globalTheta, 1, MPI_DOUBLE,
                   MPI_SUM, 0, MPI_COMM_WORLD);

	if(rank == 0) {
		/* Printing data about option */
		std::cout << "Portfolio value = " << globalPrice << std::endl;
		std::cout << "Delta = " << globalDelta<< std::endl;
		std::cout << "Vega = "  << globalVega  << std::endl;
		std::cout << "Rho = "   << globalRho   << std::endl;
		std::cout << "Theta = " << globalTheta << std::endl; 
	}
	MPI_Finalize();
        return 0;
}
