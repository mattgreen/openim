#ifndef SPRY_COLLECTIONS_VECTOR_HPP
#define SPRY_COLLECTIONS_VECTOR_HPP

#include <vector>

namespace Spry
{
namespace Collections
{
	template<typename T>
	class Vector
	{
	public:
		typedef typename std::vector<T>::iterator	Iterator;

		Vector()
		{
		}

		T& operator[](int index)
		{
			return _vec[index];
		}

		void Add(const T& item)
		{
			_vec.push_back(item);
		}

		void Clear()
		{
			_vec.clear();
		}

		bool Contains(const T& item) const
		{
			for(Iterator i = GetBeginIterator(); i != GetEndIterator(); ++i)
			{
				if(*i == item)
					return true;
			}

			return false;
		}

		Iterator GetBeginIterator()
		{
			return _vec.begin();
		}

		Iterator GetEndIterator()
		{
			return _vec.end();
		}

		int GetCapacity() const
		{
			return (int)_vec.capacity();
		}

		int GetSize() const
		{
			return (int)_vec.size();
		}

		void Insert(const T& item, int index)
		{
			if(index < 0 || index > GetLength())
				return;

			_vec.insert(_vec.begin() + index, item);
		}

		void Remove(const T& item)
		{
			for(Iterator i = GetBeginIterator(); i != GetEndIterator(); ++i)
			{
				if(*i == item)
					RemoveAt(i);
			}

			return false;
		}
		
		void RemoveAt(Iterator i)
		{
			_vec.erase(i);
		}

		void RemoveAt(int index)
		{
			if(index < 0 || index > GetLength())
				return;

			RemoveAt(_vec.begin() + index);
		}

		void Reserve(int capacity)
		{
			_vec.reserve((size_type)capacity);
		}

		void Resize(int newSize)
		{
			_vec.resize((size_type)newSize);
		}

		__declspec(property(get=GetCapacity))		int			Capacity;
		__declspec(property(get=GetBeginIterator))	Iterator	Begin;
		__declspec(property(get=GetEndIterator))	Iterator	End;
		__declspec(property(get=GetSize))			int			Size;

	private:
		std::vector<T>	_vec;
	};
}
}

#ifdef SPRY_AUTO_USING_DECLARATIONS
using Spry::Collections::Vector;
#endif

#endif