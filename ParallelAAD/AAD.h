/*
 * @Author Shane Keane
 * @File AAD.h
 * @Brief Header File containing necessary files and functions to use
 * 	  Numbers and use AAD to find adjoints. 
 * @Date 16-06-2022
 * @Version 1.0
 */

#include <memory>
#include "Number.h"

/* Function to put every item in a container on tape */
template <class IT>
inline void putOnTape(IT begin, IT end){
	for_each(begin, end, [](Number& n) {n.putOnTape(); });
}

