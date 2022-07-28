/* 
 * @Author Shane Keane
 * @File Number.h
 * @Brief File containing Number class for AAD implementationt
 * 	  this class is used to overload operations to hold
 * 	  both value and adjoint.
 * @Date 16-06-2022
 * @Version 1.0
 */

#include <algorithm>
#include "Tape.h"
#include <iostream>

class Number {
	double myValue;
	Node* myNode;
	/* @Brief create node on the tape */
	template <size_t N>
	void createNode() {
		myNode = tape->recordNode<N>();
	}
	/* @Brief Giving access to Node for friends */
	Node& node() const {
		/* Find myNode and check if it is found */
		auto it = tape->find(myNode);
		if(it == tape->end()){
			throw runtime_error("Put a breakpoint here");
		}
		/* Why const_cast ? */
		return const_cast<Node&>(*myNode);
	}
	/* Unary operations */
	double& derivative() {
		return myNode->myDerivatives[0];
	}
	/* Binary operations */
	double& leftDer() { 
		return myNode->myDerivatives[0];
	}

	double& rightDer() {
		return myNode->myDerivatives[1];
	}
	/* Unary operations */
	double*& adjPtr() {
		return node->myAdjPtrs[0];
	}
	/* Binary operations */
	double*& leftAdj() {
		return node->myAdjPtrs[0];
	}

	double*& rightAdj() {
		return node->myAdjPtrs[1];
	}

	/* @Brief Private constructors used when overloading operations */
	/* Unary */
	Number(Node& arg, const double val): myValue(val) {
		createNode<1>();
		myNode->myAdjPtrs[0] = arg.myAdjoints;
	}	
	/* Binary */
	Number(Node& lhs, Node& rhs, const double val): myValue(val) {
		createNode<2>();
		myNode->myAdjPtrs[0] = lhs.myAdjoints;
		myNode->myAdjPtrs[1] = rhs.myAdjoints;
	}
public: 
	/* Give static access to tape */
	static thread_local Tape* tape; 
	/* @Brief Public Constructor */
	Number() {}

	/* @Brief Put number on tape with construction */
	explicit Number(const double val) : myValue(val) { 
		createNode<0>();
	}
	/* @Brief Put number on tape with assignment */
	Number* operator=(const double val) {
		myValue = val; 
		createNode<0>();
		return *this;
	}
	/* @Brief Function to expicitly put existing number on tape */
	void putOnTape() {
		createNode<0>();
	}
	/* @Brief Functions to convert Number to double */
	explicit operator double& () { 
		return myValue;
	}
	explicit operator double const () {
		return myValue;
	}
	/* Functions to access value and adjoint */
	double& value() {
		return myValue;
	}
	double value() const {
		return myValue;
	}	
	double& adjoint() {
		return myNode->adjoint();
	}
	double adjoint() const {
		return myNode->adjoint();
	}
	/* @Brief Reset adjoint on tape */
	void resetAdjoints() {
		tape->resetAdjoints();
	}
	/* Functions to propagate Adjoints using equation */
	static void propagateAdjoints(Tape::iterator From,Tape::iterator To) {
		auto it = From;
		/* Going from From to To */
		while(it != From) {
			it -> propagateOne();
			--it;
		}
		it->propagateOne();
	}
	/* @Brief Set adjoint to 1 and propagate from current Node to To */
	void prpagateAdjoints(Tape::iterator To) {
		adjoint() = 1.0;
		auto From = tape->find(myNode);
		propagateAdjoints(From, To);
	}
	void propagateToStart() {
		propagateAdjoints(tape->begin());
	}
	void propagateToMark() {
		propagateAdjoints(tape->markIt());
	}
	static void propagateMarkToStart() {
		propagateAdjoints(prev(tape->MarkIt()), tape->being());
	}

	/* Overloading operators */


};
