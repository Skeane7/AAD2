/**
 * \file Number.h
 * \brief File containing code to set up new
 *        variable to be used instead of doubles
 * \author Shane Keane 17329836
 * \version 1.0
 * \date 2022-05-19
 */


#include <vector>
#include <memory>
#include <cmath>
#include "Tape.h"

/* @Brief Number class to use to hold adjoints and result */
class Number {
        Node* myNode;
	double myValue;
public:
	/* Each thread stores its own tape */
	static thread_local Tape* tape;
	Number() {} /* Default constructor */
	/* @Brief Functions to return value of Number */
	double& value() {
		return myValue;
	}
	double value() const {
		return myValue;
	}
	/* @Brief Functions to return adjoints of Number */
	double& adjoint() {
		return myNode->adjoint;
	} 
	double adjoint() const {
		return myNode->adjoint;
	}
private:
	/* Access to derivatives */
	/* Unary operations */
	double& derivative() {
		return myNode->pDerivatives[0];
	}
	/* For binary operations */
	double& lDer() { 
		return myNode->pDerivatives[0];
	}
        double& rDer() {
                return myNode->pDerivatives[1];
        }
	/* Access to adjoints of children */
	/* Unary operations */
	double *& adjPtr() {
		return myNode->pAdjPtrs[0];
	}
	/* For binary operations */
	double *& leftAdj() {
		return myNode-pAdjPtrs[0];
	}
        double *& rightAdj() {
                return myNode-pAdjPtrs[1];
        }
	/* @Brief Function for creating Node on tape with the right number of children */
	template <size_t N>
	void createNode() {
		myNode = tape->recordNode<N>();
	}
};

/* Defining global tape */
std::vector<std::unique_ptr<Node>> Number::tape;

/* Overloading operators to handle Number class */
Number operator+(Number lhs, Number rhs) {
        /* Create node which evaluates result */
        Node* n = new PlusNode(lhs.node(), rhs.node());
        /* Put this node on to tape */
        Number::tape.push_back(std::unique_ptr<Node>(n));
        /* Return node as result */
        return n;
}

Number operator-(Number lhs, Number rhs) {
        /* Create node which evaluates result */
        Node* n = new SubNode(lhs.node(), rhs.node());
        /* Put this node on to tape */
        Number::tape.push_back(std::unique_ptr<Node>(n));
        /* Return node as result */
        return n;
}

Number operator-(Number arg){   
   	/* Create node which evaluates result */
        Node* n = new SignNode(arg.node());
        /* Put this node on to tape */
        Number::tape.push_back(std::unique_ptr<Node>(n));
        /* Return node as result */
        return n;
		
}

Number operator*(Number lhs, Number rhs) {
        /* Create node which evaluates result */
        Node* n = new TimesNode(lhs.node(), rhs.node());
        /* Put this node on to tape */
        Number::tape.push_back(std::unique_ptr<Node>(n));
        /* Return node as result */
        return n;
}

Number operator*(double lhs, Number rhs) {
        /* Create node which evaluates result */
        Node* n = new TimesNode2(lhs, rhs.node());
        /* Put this node on to tape */
        Number::tape.push_back(std::unique_ptr<Node>(n));
        /* Return node as result */
        return n;
}

Number operator/(Number lhs, Number rhs) {
        /* Create node which evaluates result */
        Node* n = new DivNode(lhs.node(), rhs.node());
        /* Put this node on to tape */
        Number::tape.push_back(std::unique_ptr<Node>(n));
        /* Return node as result */
        return n;
}

Number log(Number arg) {
        /* Create node which evaluates result */
        Node* n = new LogNode(arg.node());
        /* Put this node on to tape */
        Number::tape.push_back(std::unique_ptr<Node>(n));
        /* Return node as result */
        return n;
}

Number exp(Number arg) {
	/* Create node which evaluates result */
	Node* n = new ExpNode(arg.node());
	/* Put this node on to the tape */
	Number::tape.push_back(std::unique_ptr<Node>(n));
	/* Return node as result */
	return n;
}

Number sqrt(Number arg) {
        /* Create node which evaluates result */
        Node* n = new SqrtNode(arg.node());
        /* Put this node on to the tape */
        Number::tape.push_back(std::unique_ptr<Node>(n));
        /* Return node as result */
        return n;
}

Number normal(Number arg) { 
        /* Create node which evaluates result */
        Node* n = new NormalNode(arg.node());
        /* Put this node on to the tape */
        Number::tape.push_back(std::unique_ptr<Node>(n));
        /* Return node as result */
        return n;
}

std::ostream& operator<<(std::ostream& os, Number &arg){
	os << arg.myvalue;
	return os;
}
