#pragma once

#include <functional> // std::less
#include <iterator> // std::iterator_traits

namespace sort {

	// This is C++ magic which will allows our function
	// to default to using a < b if the comparator arg
	// is unspecified. It uses defines std::less<T>
	// for the iterator's value_type.
	//
	// For example: if you have a vector<float>, the 
	// iterator's value type will be float. std::less 
	// will select the < for sorting floats as the 
	// default comparator.
	template<typename RandomIter>
	using less_for_iter = std::less<typename std::iterator_traits<RandomIter>::value_type>;

	/* Efficiently swap two items - use this to implement your sorts */
	template<typename T>
	void swap(T& a, T& b) noexcept { 
		T temp = std::move(b);
		b = std::move(a);
		a = std::move(temp);
	}

	template<typename RandomIter, typename Comparator = less_for_iter<RandomIter>>
	void bubble(RandomIter begin, RandomIter end, Comparator comp = Comparator{}) {
		// Random access iterators have the same traits you defined in the Vector class
		// For instance, difference_type represents an iterator difference
		// You may delete the types you don't use to remove the compiler warnings

		if(begin == end) return;

		bool swapped = true;
		RandomIter last_unsorted = end;

		while(swapped) {
			swapped = false;
			RandomIter curr = begin;
			RandomIter next = std::next(curr);

			while(next != last_unsorted) {
				if(comp(*next, *curr)) {
					sort::swap(curr, next);
					swapped = true;
				}
				curr = next;
				next = std::next(next);
			}
			last_unsorted = curr;
		}
	}

	template<typename RandomIter, typename Comparator = less_for_iter<RandomIter>>
	void insertion(RandomIter begin, RandomIter end, Comparator comp = Comparator{}) { 
		if(begin == end) return;
	}

	template<typename RandomIter, typename Comparator = less_for_iter<RandomIter>>
	void selection(RandomIter begin, RandomIter end, Comparator comp = Comparator{}) { 
		for(RandomIter i = begin; i != end; ++i) {
			RandomIter currMin = i;

			for(RandomIter j = std::next(i); j != end; ++j) {
				if(comp(*j, *currMin)){
					currMin = j;
				}
			}

			if(currMin != i) sort::swap(i, currMin);
		}
	}
}