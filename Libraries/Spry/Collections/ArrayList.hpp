#ifndef SPRY_COLLECTIONS_ARRAYLIST_HPP
#define SPRY_COLLECTIONS_ARRAYLIST_HPP

#include <deque>

namespace Spry
{
namespace Collections
{
	template<typename T>
	class ArrayList
	{
	public:
		typedef typename std::deque<T>::iterator	Iterator;

		ArrayList()
		{
		}

		explicit ArrayList(int initialSize) :
			_arrayList(initialSize)
		{
		}

		T& operator[](int index)
		{
			return _arrayList[index];
		}

		void Add(const T& item)
		{
			_arrayList.push_back(item);
		}

		void Clear()
		{
			_arrayList.clear();
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
			return _arrayList.begin();
		}

		Iterator GetEndIterator()
		{
			return _arrayList.end();
		}

		int GetCapacity() const
		{
			return (int)_arrayList.capacity();
		}

		int GetSize() const
		{
			return (int)_arrayList.size();
		}

		void Insert(const T& item, int index)
		{
			if(index < 0 || index > GetSize())
				return;

			_arrayList.insert(_arrayList.begin() + index, item);
		}

		bool IsEmpty() const
		{
			return _arrayList.empty();
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
			_arrayList.erase(i);
		}

		void RemoveAt(int index)
		{
			if(index < 0 || index > GetSize())
				return;

			RemoveAt(_arrayList.begin() + index);
		}

		void Resize(int newSize)
		{
			if(newSize < 0)
				return;

			_arrayList.resize((size_type)newSize);
		}

		operator std::deque<T>&()
		{
			return _arrayList;
		}

		__declspec(property(get=GetCapacity))		int			Capacity;
		__declspec(property(get=GetBeginIterator))	Iterator	Begin;
		__declspec(property(get=GetEndIterator))	Iterator	End;
		__declspec(property(get=GetSize))			int			Size;

	private:
		std::deque<T>	_arrayList;
	};
}
}


#ifdef SPRY_AUTO_USING_DECLARATIONS
using Spry::Collections::ArrayList;
#endif

#endif