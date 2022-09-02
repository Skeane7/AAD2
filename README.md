# AAD2

This repository contains an implementation of automatic adjoint differentiation. This is part of my submission of my summer project as part of my masters in 
High Performance Computing from Trinity College Dublin. 


This repository has 3 directories:

 Version 1 -> is a naive implementation of AAD, included as a starting point for someone trying to implement their own version. When make is ran an executable is
              returned that differentiates the Black-Scholes-Merton formula. 
 
 EfficientAAD -> AAD implementation with checkpointing as well as other optimisations. This is the main block of code that can be used to differentiate a 
                 target function. When make is ran three executables are returned. basicEx differentiates a simple target function which can be used as an 
                 example of how to use the implementation. main will return price, delta, vega, rho, theta for a single option contract using the AAD implementation.
                 BumpReval returns the same quantities using bump and revalue instead of AAD. 
                  
 ParallelAAD -> A demonstration of how one could parallelise the AAD implementation using MPI. When you run make you will get two executables. 
                singleOption calculates price, delta, vega, rho, theta for a single option contract. multiOption calculates the same properties for 
                a portfolio of options. 
