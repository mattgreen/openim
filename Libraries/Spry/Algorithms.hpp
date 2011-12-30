#ifndef SPRY_ALGORITHMS_HPP
#define SPRY_ALGORITHMS_HPP

#include <algorithm>

namespace Spry
{
	template<typename InputIterator, typename OutputIterator>
	inline OutputIterator Copy(InputIterator begin, InputIterator end, OutputIterator destination)
	{
		return std::copy(begin, end, destination);
	}

	template<typename Container, typename OutputIterator>
	inline OutputIterator Copy(const Container& container, OutputIterator destination)
	{
		return Copy(container.begin(), container.end(), destination);
	}

	template<typename BidirectionalIterator>
	inline void Reverse(BidirectionalIterator begin, BidirectionalIterator end)
	{
		std::reverse(begin, end);
	}

	template<typename Container>
	inline void Reverse(Container& container)
	{
		Reverse(container.begin(), container.end());
	}

	template<typename RandomAccessIterator>
	inline void Sort(RandomAccessIterator begin, RandomAccessIterator end)
	{
		std::sort(begin, end);
	}

	template<typename Container>
	inline void Sort(Container& container)
	{
		Sort(container.begin(), container.end());
	}


	template<typename Predicate, typename RandomAccessIterator>
	inline void Sort(RandomAccessIterator begin, RandomAccessIterator end, Predicate predicate)
	{
		std::sort(begin, end, predicate);
	}

	template<typename Predicate, typename RandomAccessIterator>
	inline void Sort(RandomAccessIterator begin, RandomAccessIterator end)
	{
		Sort(begin, end, Predicate());
	}

	template<typename Predicate, typename Container>
	inline void Sort(Container& container, Predicate predicate)
	{
		Sort<Predicate>(container.begin(), container.end(), predicate);
	}

	template<typename Predicate, typename Container>
	inline void Sort(Container& container)
	{
		Sort<Predicate>(container.begin(), container.end());
	}

	template<typename T>
	inline void Swap(T& l, T& r)
	{
		T temp = l;
		l = r;
		r = temp;
	}	
}


#endif