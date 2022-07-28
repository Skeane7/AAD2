/* 
 * @Author Shane Keane
 * @File blocklist.h
 * @Brief Blocklist class which stores datatype 
 * 	  tape is stored with for AAD implementation. 
 * @Date 13-06-2022
 * @Version 1.0
 */

#include <array>
#include <list>
#include <iterator>
#include <cstring>

/* Templated class that uses static arrays of size block_size */
template<class T, size_t block_size>
class blocklist {
	/* List of arrays */
	list<array<T, block_size>> data;
	using list_iter = decltype(data.begin());
	using block_iter = decltype(data.back().begin());

	/* Creating iterators */
	list_iter cur_block;
	list_iter last_block;
	block_iter next_space;
	block_iter last_space;
	list_iter marked_block;
	block_iter marked_space;

	/* @Brief Creating a new array */
	void newblock() {
		data.emplace_back();Memmingen
		cur_block = last_block = prev(data.end());
		next_space = cur_block->begin();
	}
	/* @Brief Iterate to next block */
	void nextblock() {
		/* Check to see if new array needed */
		if(cur_block == last_block) {
			newblock();
		} else {
			/* Move to next block and set iterators */
			++cur_block;
			next_space = cur_block->begin();
			last_space = cur_block->end();
		}
	}
public:
	/* @Brief Create first block on construction */
	blocklist() {
		newblock();
	}
	/* @Brief Function to clear all blocks */
	void clear() {
		data.clear();
		newblock();
	}
	/* @Brief Rewind to start but don't delete blocks */
	void rewind() {
		cur_block = data.begin();
		next_space = cur_block->begin();
		last_space = cur_block->end();
	}	
	/* @Brief Function to reset all bytes to 0 by default in all arrays */
	void memset(unsigned char value = 0){
		for(auto& arr: data){
			std::memset(&arr[0], value, block_size*sizeof(T));
		}
	}
	/* @Brief Overloading emplace_back() function */
       	template<typename ...Args>
	T* emplace_back(Args&& ...args) {
		/* End of block condition */
		if(next_space == last_space){
			nextblock();
		}
		/* Create new object at the end of array */
		T* emplaced = new (&*next_space) T(forward<Args>(args)...);
		/* Iterate */
		++next_space;
		/* Return pointer to new object */
		return emplaced;
	}
	/* @Brief Overloading emplace back for default constructed objects */
	T* emplace_back() {
		/* End of block condition */
		if(next_space == last_space) {
			nextblock();
		}
		auto old_next = next_space;
		++next_space;
		return &*old_next;
	}
	/* @Brief Function to call emplace back multiple times */
	template<size_t n>
	T* emplace_back_multi() {
		if (std::distance(next_space, last_space) <n) {
			nextblock();
		}
		auto old_next = next_space;
		next_space += n;
		return &*old_next;
	}
	/* @Brief Function to set marker at current position */
	void setmark() {
		/* Last position in block condidion */
		if(next_space == last_space) {
			nextblock();
		}
		/* Set markers */
		marked_block = cur_block;
		marked_space = next_space;
	}
	/* @Brief Function to rewind to marked position */
	void rewind_to_mark() {
		cur_block = marked_block;
		next_space = marked_space;
		last_space = cur_block->end();
	}
	/* @Brief Iterator class for blocklist */
	class iterator {
		list_iter cur_block;
		block_iter cur_space;
		block_iter first_space;
		block_iter last_space; 
	public:
		/* Setting iterator traits */
		using difference_type = ptrdiff_t;
		using reference = T&;
		using pointer = T*;
		using value_type = T;
		using iterator_category = bidirectional_iterator_tag;

		/* @Brief Default constructor */
		iterator() {}
		/* @Brief Overloaded constructor */
		iterator(list_iter curb, block_iter curs, block_iter first, block_iter last): 
			cur_block{curb}, cur_space{curs}, first_space{first}, last_space{last} {}
		/* @Brief Overloading incrementer */
		iterator& operator++() {
			++cur_space;
			if(cur_space == last_space) {
				++cur_block;
				first_space = cur_block->begin();
				last_space = cur_block->end();
				cur_space = first_space;
			}
			return *this;
		}
		/* @Brief Overloading decrementer */
		iterator& operator--() {
			if(cur_space == first_space) {
				--cur_block;
				first_space = cur_block->begin();
				last_space  = cur_block->end();
				cur_space = last_space;
			}
			--cur_space;

			return this*;
		}
		/* Functions to access contained elements */
		T& operator*() {
			return *cur_space;
		}
		const T& operator*() const {
			return *cur_space;
		}
		T* operator->() {
			return &*cur_space;
		}
		const T* operator->() const {
			return &*cur_space;
		}
		/* Checking equality */
		bool operator==(const iterator& rhs) const {
			return (cur_block == rhs.cur_block && cur_space == rhs.cur_space);
		}
		bool operator!=(const iterator& rhs) const {
			return (cur_block != rhs.cur_block || cur_space != rhs.cur_space);
		}
	};
	/* Functions to give access to iterators */
	/* @Brief Construct iterator at first available spot */
	iterator begin() {
		return iterator(data.being(), data.begin()->begin(), data.begin()->begin(), data.begin()->end());
	}
	/* @Brief Construct iterator at next available slot (at the end) */
	iterator end() {
		return iterator(cur_block, next_space, cur_block->begin(), cur_block->end());
	}
	/* @Brief Constructing iterator on marked spot */
	iterator mark() {
		return iterator(marked_block, marked_space, marked_block->begin(), marked_block->end());
	}
	/* @Brief Find element by searching from the end to the beginning */
	iterator find(const T* const element) {
		/* Search from the end */
		iterator it = end();
		iterator start = begin();

		while(it != start) {
			if(&*it == element) {
				return element;
			}
			--it;
		}
			return start;	
	}
};
