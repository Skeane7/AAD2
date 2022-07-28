/**
 * \file Node.h
 * \brief File containing code to set up nodes
 *        with arguments and operations
 * \author Shane Keane 17329836
 * \version 1.0
 * \date 2022-05-19
 */

#include <memory>
#include <vector>
#include <cmath>

/* @Brief Node class for our operations that make our tapes */
class Node {
        protected:
                /* Each node has arguments which are pointers to nodes */
                std::vector<Node*> myArguments;
                /* Variables to store result and adjoint */
                double myResult;
                double myAdjoint = 0.0;
        public:
                /* @Brief Constructor */
		Node() { 
			std::cout << "Constructing Node object \n";
		}
		/* @Brief Functions to return result and adjoint */
                double getResult() {
                        return myResult;
                }
                double& getAdjoint() {
                        return myAdjoint;
                }
                /* @Brief Function to reset Adjoints of all nodes*/
                void resetAdjoints() {
                        for(auto i: myArguments) {
                                i->resetAdjoints();
                        }
                        myAdjoint = 0.0;
                }
                /* @Brief Function virtual to be overridden */
                virtual void propagateAdjoint() = 0;
};


/* Now we want to make classes for each operation we need in the calculation */

/* @Brief Addition class */
class PlusNode : public Node {
        public:
                /* @Brief Constructor */
                PlusNode(Node* lhs, Node* rhs) {
                        myArguments.resize(2);
                        myArguments[0] = lhs;
                        myArguments[1] = rhs;

                        /* Evaluaton */
                        myResult = lhs->getResult() + rhs->getResult();
                }

                void propagateAdjoint() override {
                        myArguments[0]->getAdjoint() += myAdjoint;
                        myArguments[1]->getAdjoint() += myAdjoint;
                }
};

/* @Brief Subtraction node */
class SubNode : public Node { 
        public:
                /* @Brief Constructor */
                SubNode(Node* lhs, Node* rhs) {
                        myArguments.resize(2);
                        myArguments[0] = lhs;
                        myArguments[1] = rhs;

                        /* Evaluaton */
                        myResult = lhs->getResult() - rhs->getResult();
                }

                void propagateAdjoint() override {
                        myArguments[0]->getAdjoint() += myAdjoint;
                        myArguments[1]->getAdjoint() -= myAdjoint;
                }
};

/* @Brief Multiplication class */
class TimesNode : public Node {
        public:
                /* @Brief Constructor */
                TimesNode(Node* lhs, Node* rhs) {
                        myArguments.resize(2);
                        myArguments[0] = lhs;
                        myArguments[1] = rhs;

                        /* Evaluaton */
                        myResult = lhs->getResult() * rhs->getResult();
                }

                void propagateAdjoint() override {
                        myArguments[0]->getAdjoint() += myAdjoint*myArguments[1]->getResult();
                        myArguments[1]->getAdjoint() += myAdjoint*myArguments[0]->getResult();
                }
};


/* @Brief Division class */
class DivNode : public Node {
        public:
                /* @Brief Constructor */
                DivNode(Node* lhs, Node* rhs) {
                        myArguments.resize(2);
                        myArguments[0] = lhs;
                        myArguments[1] = rhs;

                        /* Evaluaton */
                        myResult = lhs->getResult() / rhs->getResult();
                }

                void propagateAdjoint() override {
                        myArguments[0]->getAdjoint() += myAdjoint/myArguments[1]->getResult();
                        myArguments[1]->getAdjoint() -= myAdjoint*myArguments[0]->getResult()/(myArguments[1]->getResult()*myArguments[1]->getResult());
                }
};


class SqrtNode : public Node {
	public: 
                /* @Brief Constructor */
                SqrtNode(Node* arg) {
                        myArguments.resize(1);
                        myArguments[0] = arg;

                        /* Evaluaton */
                        myResult = std::sqrt(arg->getResult());
                }

                void propagateAdjoint() override {
                        myArguments[0]->getAdjoint() += 0.5*myAdjoint/myResult;
                }
};
/* @Brief Logarithm Node */
class LogNode : public Node {
        public:
                LogNode(Node* arg) {
                        myArguments.resize(1);
                        myArguments[0] = arg;

                        /* Evaluation */
                        myResult = std::log(arg->getResult());
                }

                void propagateAdjoint() override {
                        myArguments[0]->getAdjoint() += myAdjoint/myArguments[0]->getResult();
                }
};

/* @Brief Exp node */
class ExpNode: public Node {
        public:
                ExpNode(Node *arg) {
                        myArguments.resize(1);
                        myArguments[0] = arg;

                        myResult = std::exp(arg->getResult());
                }

		void propagateAdjoint() override { 
			myArguments[0]->getAdjoint() += myAdjoint*myResult;
		}
};

/* @Brief Normal distribution */
class NormalNode: public Node {
        public:
                NormalNode(Node *arg) {
                        myArguments.resize(1);
                        myArguments[0] = arg;

                        myResult = 0.5 * std::erfc(-arg->getResult() * M_SQRT1_2);;
                }

                void propagateAdjoint() override {
                        myArguments[0]->getAdjoint() += myAdjoint*(1/sqrt(2*M_PI))*exp(-0.5*myArguments[0]->getResult()*myArguments[0]->getResult());
                }
};
/* @Brief Leaf class */
class Leaf : public Node {
        public:
                /* @Brief Constructor */
                Leaf(double val) {
                        myResult = val;
                }
                /* @Brief Function to return result */
                double getVal() {
                        return myResult;
                }
                /* @Brief Function to set value */
                void setVal(double val) {
                        myResult = val;
                }

                void propagateAdjoint() override {}

};
