#ifndef SPRY_COLLECTIONS_SET_HPP
#define SPRY_COLLECTIONS_SET_HPP

#include <set>

namespace Spry
{
namespace Collections
{
	template<typename T>
	class Set
	{
	public:
		typedef typename std::set<T>::iterator	Iterator;

		void Add(const T& item)
		{
			_set.insert(item);
		}

		void Clear()
		{
			_set.clear();
		}

		bool Contains(const T& item) const
		{
			return _set.find(item) != _set.end();
		}

		Iterator GetBeginIterator()
		{
			return _set.begin();
		}

		Iterator GetEndIterator()
		{
			return _set.end();
		}

		int GetSize() const
		{
			return static_cast<int>(_set.size());
		}

		bool IsEmpty() const
		{
			return _set.empty();
		}

		void Remove(const T& item)
		{
			_set.erase(item);
		}

		__declspec(property(get=GetBeginIterator))	Iterator	Begin;
		__declspec(property(get=GetEndIterator))	Iterator	End;
		__declspec(property(get=GetSize))			int			Size;

	private:
		std::set<T> _set;
	};
}
}

#ifdef SPRY_AUTO_USING_DECLARATIONS
using Spry::Collections::Set;
#endif


#endif