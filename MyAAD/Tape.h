/* 
 * @Author Shane Keane
 * @File Tape.h
 * @Brief File containing tape class for AAD implementation
 * @Date 16-06-2022
 * @Version 1.0
 */

#include "blocklist.h"
#include "Node.h"

#define BLOCKSIZE 16384
#define DATASIZE  65536
#define ADJSIZE   32768

class Tape {
	/* Blocklist objects to store derivatives
	 * nodes and child adjoint pointers */
	blocklist<double, DATASIZE> myDers;
	blocklist<double*, DATASIZE> myChlPtrs;
	blocklist<Node, BLOCKSIZE> myNodes; 

	friend class Number; 
public:
	/* @Brief Function to build a node with N children */
	template<size_t N>
	Node* recordNode() {
		/* Creating node at the end of the tape */
		Node* node = myNodes.emplace_back(N);
		if(N>0) {
			/* Adding derivatives and child adjoint pointers */
			node->myDerivatives = myDers.emplace_back_multi<N>();
			node->myAdjPtrs  = myChlPtrs.emplace_back_multi<N>();
		}
		return node;
	}

	/* @Brief Function to clear tape */
	void clear() {
		myDers.clear();
		myChlPtrs.clear();
		myNodes.clear();
	}

	/*@Brief Function to rewind tape */
	void rewind() {
		/* For now just clear tape */
		clear();
	}

	/* @Brief Function to set mark */
	void mark() {
		myDers.setmark();
		myChlPtrs.setmark();
		myNodes.setmark();
	}

	/*Brief Function to rewind to mark */
	void rewindToMark() {
		myDers.rewind_to_mark();
		myChlPtrs.rewind_to_mark();
		myNodes.rewind_to_mark();
	}

	/* Functions dealing with iterators */
	using iterator = blocklist<Node, BLOCKSIZE>::iterator;
	/* @Brief return iterator at beginning of myNodes */
	auto begin() {
		return myNodes.begin();
	}
	/* @Brief return iterator at next available space in myNodes */
	auto end() {
		return myNodes.end();
	}
	/* @Brief return iterator at node previosly marked */
	auto MarkIt() {
		return myNodes.mark();
	}
	/* @Brief search from end to start of myNodes to find node */
	auto find(Node* node) {
		return myNodes.find(node);
	}


};
