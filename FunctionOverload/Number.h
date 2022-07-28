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
#include "Node.h"

/* @Brief Number class to use to hold adjoints and result */
class Number {
        Node* myNode;
        public:
                /* Vector to hold the tape */
                static std::vector<std::unique_ptr<Node>> tape;

                /* Creating node and adding it to tape */
                Number(double val) : myNode(new Leaf(val)) {
                        tape.push_back(std::unique_ptr<Node>(myNode));
                }

                /* @Brief Constructor */
                Number(Node* node) : myNode(node) {
			std::cout << "Constructing Number \n";
		}

                /* @Brief Function to return Node */
                Node* node() {
                        return myNode;
                }
                /* @Brief Function to set value of Node */
                void setVal(double val) {
                        /* Cast to leaf or else Node can't be changed */
                        dynamic_cast<Leaf*>(myNode)->setVal(val);
                }
                /* @Brief return value from node */
                double getVal() {
                        /* Again must cast to leaf so have ability to read */
                        return dynamic_cast<Leaf*>(myNode)->getVal();
                }
                /* @Brief Adjoint accessor */
                double& getAdjoint() {
                        return myNode->getAdjoint();
                }
                /* @Brief Adjoint propagator */
                void propagateAdjoints() {
                        /* Reset adjoints which sets them to 0 */
                        myNode -> resetAdjoints();
                        /* Set adjoint of result to 1 */
                        myNode -> getAdjoint() = 1.0;
                        /* Finding node on the tape searching from last */
                        auto it = tape.rbegin();
                        while(it->get() != myNode) {
                                /* ++ means -- for reverse iterator */
                                ++it;
                        }
                        /* Now propogate the other way */
                        while (it != tape.rend()) {
                                (*it)->propagateAdjoint();
                                ++it;
                        }
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
