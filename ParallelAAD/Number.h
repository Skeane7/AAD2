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
#include <cmath>

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
		return myNode->myAdjPtrs[0];
	}
	/* Binary operations */
	double*& leftAdj() {
		return myNode->myAdjPtrs[0];
	}

	double*& rightAdj() {
		return myNode->myAdjPtrs[1];
	}

	/* @Brief Private constructors used when overloading operations */
	/* Unary */
	Number(Node& arg, const double val): myValue(val) {
		createNode<1>();
		myNode->myAdjPtrs[0] = &arg.myAdjoint;
	}	
	/* Binary */
	Number(Node& lhs, Node& rhs, const double val): myValue(val) {
		createNode<2>();
		myNode->myAdjPtrs[0] = &lhs.myAdjoint;
		myNode->myAdjPtrs[1] = &rhs.myAdjoint;
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
		return this;
	}
	/* @Brief Function to expicitly put existing number on tape */
	void putOnTape() {
		createNode<0>();
	}
	/* @Brief Functions to convert Number to double */
	explicit operator double& () { 
		return myValue;
	}
	explicit operator double() {
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

	double& adjoint(const size_t n){
		return myNode->adjoint(n);
	}	

	double adjoint(const size_t n) const {
		return myNode->adjoint(n);
	}
	/* @Brief Reset adjoint on tape */
	void resetAdjoints() {
		tape->resetAdjoints();
	}
	/* Functions to propagate Adjoints using equation */
	static void propagateAdjoints(Tape::iterator From,Tape::iterator To) {
		auto it = From;
		/* Going from From to To */
		while(it != To) {
			it->propagateOne();
			--it;
		}
		it->propagateOne();
	}
	/* @Brief Set adjoint to 1 and propagate from current Node to To */
	void propagateAdjoints(Tape::iterator To) {
		adjoint() = 1.0;
		auto From = tape->find(myNode);
		propagateAdjoints(From, To);
	}
	void propagateToStart() {
		propagateAdjoints(tape->begin());
	}
	void propagateToMark() {
		propagateAdjoints(tape->MarkIt());
	}
	static void propagateMarkToStart() {
		propagateAdjoints(prev(tape->MarkIt()), tape->begin());
	}

	/* Overloading operators */
	friend Number operator+(const Number& lhs, const Number& rhs){
		/* Computing result of node */
		const double res = lhs.value() + rhs.value();
		/* Making Node on tape to store result */
		Number result(lhs.node(), rhs.node(), res);
		/* Computing derivative */
		result.leftDer() = 1.;
		result.rightDer() = 1.;
		/* Return Node with derivative and result set */
		return result;
	}

        friend Number operator+(const Number& lhs, const double& rhs){
                /* Computing result of node */
                const double res = lhs.value() + rhs;
                /* Making Node on tape to store result */
                Number result(lhs.node(), res);
                /* Computing derivative */
                result.derivative() = 1.;
                /* Return Node with derivative and result set */
                return result;
        }

        friend Number operator+(const double& lhs, const Number& rhs){
                return rhs + lhs;
        }


        friend Number operator-(const Number& lhs, const Number& rhs){
                /* Computing result of node */
                const double res = lhs.value() - rhs.value();
                /* Making Node on tape to store result */
                Number result(lhs.node(), rhs.node(), res);
                /* Computing derivative */
                result.leftDer() = 1.;
                result.rightDer() = -1.;
                /* Return Node with derivative and result set */
                return result;		
	}

        friend Number operator-(const double& lhs, const Number& rhs){
                /* Computing result of node */
                const double res = lhs - rhs.value();
                /* Making Node on tape to store result */
                Number result(rhs.node(), res);
                /* Computing derivative */
                result.derivative() = -1.;
                /* Return Node with derivative and result set */
                return result;
        }

        friend Number operator-(const Number& lhs, const double& rhs){
                /* Computing result of node */
                const double res = lhs.value() - rhs;
                /* Making Node on tape to store result */
                Number result(lhs.node(), res);
                /* Computing derivative */
                result.derivative() = 1.;
                /* Return Node with derivative and result set */
                return result;
        }

	friend Number operator*(const Number& lhs, const Number& rhs){
		/* Computing result of node */
		const double res = lhs.value() * rhs.value();
		/* Making Node on tape to store */
		Number result(lhs.node(), rhs.node(), res);
		/* Computing derivatives */
		result.leftDer() = rhs.value();
		result.rightDer() = lhs.value();
                /* Return Node with derivative and result set */
                return result;
	}

        friend Number operator*(const Number& lhs, const double& rhs){
                /* Computing result of node */
                const double res = lhs.value() * rhs;
                /* Making Node on tape to store */
                Number result(lhs.node(), res);
                /* Computing derivatives */
                result.derivative() = rhs;
		/* Return Node with derivative and result set */
                return result;
        }

	friend Number operator*(const double& lhs, const Number& rhs){
  		return rhs * lhs;
	}

	
        friend Number operator/(const Number& lhs, const Number& rhs){
                /* Computing result of node */
                const double res = lhs.value() / rhs.value();
                /* Making Node on tape to store */
                Number result(lhs.node(), rhs.node(), res);
                /* Computing derivatives */
                result.leftDer() = 1.0/rhs.value();
                result.rightDer() = -lhs.value()/(rhs.value()*rhs.value());
                /* Return Node with derivative and result set */
                return result;
        }

	friend Number operator/(const Number& lhs, const double& rhs){
                /* Computing result of node */
                const double res = lhs.value() / rhs;
                /* Making Node on tape to store */
                Number result(lhs.node(), res);
                /* Computing derivatives */
		result.derivative() = 1.0/rhs;
                /* Return Node with derivative and result set */
                return result;
        }

        friend Number operator/(const double& lhs, const Number& rhs){
                /* Computing result of node */
                const double res = lhs / rhs.value();
                /* Making Node on tape to store */
                Number result(rhs.node(), res);
                /* Computing derivatives */
                result.derivative() = -lhs/(rhs.value()*rhs.value());
                /* Return Node with derivative and result set */
                return result;
        }

	friend Number max(const Number& lhs, const Number& rhs){
		/* Check to see which Number is bigger */
		const bool lhs_maximum = lhs.value() >= rhs.value();
		if(lhs_maximum){
			/* Create Node and set derivatives */
			Number result(lhs.node(), rhs.node(), lhs.value());
			result.leftDer() = 1.0;
			result.rightDer() = 0.0;
			return result;
		} else {
                        Number result(lhs.node(), rhs.node(), rhs.value());
                        result.leftDer() = 0.0;
                        result.rightDer() = 1.0;
			return result;
		}
	}

        friend Number max(const Number& lhs, const double& rhs){
                /* Check to see which Number is bigger */
                const bool lhs_maximum = lhs.value() >= rhs;
                if(lhs_maximum){
			/* Create Node and set derivatives */
                        Number result(lhs.node(),lhs.value());
                        result.derivative() = 1.0;
                	return result;
                } else {
                        Number result(lhs.node(), rhs);
                        result.derivative() = 0.0;
                	return result;
                }
	}

	friend Number max(const double& lhs, const Number& rhs){
                /* Check to see which Number is bigger */
                const bool lhs_maximum = lhs >= rhs.value();
                if(lhs_maximum){
                        /* Create Node and set derivatives */
                        Number result(rhs.node(),lhs);
                        result.derivative() = 0.0;
                	return result;
                } else {
                        Number result(rhs.node(), rhs.value());
                        result.derivative() = 1.0;
                	return result;
                }
        }


	friend Number min(const Number& lhs, const Number& rhs){
                /* Check to see which Number is bigger */
                const bool lhs_minimum = lhs.value() <= rhs.value();
                if(lhs_minimum){
                        /* Create Node and set derivatives */
                        Number result(lhs.node(), rhs.node(), lhs.value());
                        result.leftDer() = 1.0;
                        result.rightDer() = 0.0;
			return result;
                } else {
                        Number result(lhs.node(), rhs.node(), rhs.value());
                        result.leftDer() = 0.0;
                	return result;
                }
        }

        friend Number min(const Number& lhs, const double& rhs){
                /* Check to see which Number is bigger */
                const bool lhs_minimum = lhs.value() <= rhs;
                if(lhs_minimum){
                        /* Create Node and set derivatives */
                        Number result(lhs.node(),lhs.value());
                        result.derivative() = 1.0;
                	return result;
                } else {
                       Number result(lhs.node(), rhs);
                        result.derivative() = 0.0;
                	return result;
                }
        }

	/* More overloading useful operations */
	Number& operator+=(const Number& arg){
		*this = *this + arg;
		return *this;
	}

	Number& operator+=(const double& arg){
		*this = *this + arg;
		return *this;
	}

	Number& operator-=(const Number& arg){
		*this = *this - arg;
		return *this;
	}

	Number& operator-=(const double& arg){
		*this = *this - arg;
		return *this;
	}

	Number& operator*=(const Number& arg){
		*this = *this * arg;
		return *this;
	}

	Number& operator*=(const double& arg){
		*this = *this * arg;
		return *this;
	}

	Number& operator/=(const Number& arg){
		*this = *this / arg;
		return *this;
	}

	Number& operator/=(const double& arg){
		*this = *this / arg;
		return *this;
	}

	/* Unary - to set a number to be negative */
	Number operator-() const{
		return 0.0 - *this;
	}

	Number operator+() const{
		return *this;
	}

	/* Unary operators, easier to code as there is only one case per operation */
	friend Number exp(const Number& arg){
		/* Computing result of operation */
		const double res = std::exp(arg.value());
		/* Creating node */
		Number result(arg.node(), res);
		/* Computing derivative and returning */
		result.derivative() = res;
		return result;
	}

	friend Number sqrt(const Number& arg){
		/* Computing result of operation */
		const double res = std::sqrt(arg.value());
                /* Creating node */
                Number result(arg.node(), res);
                /* Computing derivative and returning */
		result.derivative() = 1./(2*res);
		return result;
	}

	friend Number fabs(const Number& arg){
                /* Computing result of operation */
                const double res = std::fabs(arg.value());
		/* Creating node */
                Number result(arg.node(), res);
		/* Computing derivative */
		if(res>0){
			result.derivative() = 1.0;
		} else { 
			result.derivative() = -1.0;
		}
		return result;
	}


	inline friend bool operator==(const Number& lhs, const Number& rhs){
		return lhs.value() == rhs.value();
	}

	inline friend bool operator==(const Number& lhs, const double& rhs){
		return lhs.value() == rhs;
	}

	inline friend bool operator==(const double& lhs, const Number& rhs){
		return lhs == rhs.value();
	}

	inline friend bool operator!=(const Number& lhs, const Number& rhs){
		return lhs.value() != rhs.value();
	}
	
	inline friend bool operator!=(const Number& lhs, const double& rhs){
		return lhs.value() != rhs;
	}

	inline friend bool operator!=(const double& lhs, const Number& rhs){
		return lhs != rhs.value();
	}

	inline friend bool operator<(const Number& lhs, const Number& rhs){
		return lhs.value() < rhs.value();
	}

	inline friend bool operator<(const Number& lhs, const double& rhs){
		return lhs.value() < rhs;
	}

	inline friend bool operator<(const double& lhs, const Number& rhs){
		return lhs < rhs.value();
	}

	inline friend bool operator>(const Number& lhs, const Number& rhs){
		return lhs.value() > rhs.value();
	}

	inline friend bool operator>(const Number& lhs, const double& rhs){
		return lhs.value() > rhs;
	}

	inline friend bool operator>(const double& lhs, const Number& rhs){
		return lhs > rhs.value();
	}

	inline friend bool operator<=(const Number& lhs, const Number& rhs){
		return lhs.value() <= rhs.value();
	}

	inline friend bool operator<=(const Number& lhs, const double& rhs){
		return lhs.value() <= rhs;
	}

	inline friend bool operator<=(const double& lhs, const Number& rhs){
		return lhs <= rhs.value();
	}

	inline friend bool operator>=(const Number& lhs, const Number& rhs){
		return lhs.value() >= rhs.value();
	}

	inline friend bool operator>=(const Number& lhs, const double& rhs){
		return lhs.value() >= rhs;
	}

	inline friend bool operator>=(const double& lhs, const Number& rhs)	{
		return lhs >= rhs.value();
	}

	inline friend std::ostream& operator<<(std::ostream& os, Number arg){
		os << arg.myValue;
		return os;
	}

};
