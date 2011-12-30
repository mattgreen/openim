#ifndef SPRY_COLLECTIONS_LINKEDLIST_HPP
#define SPRY_COLLECTIONS_LINKEDLIST_HPP

#include <list>

namespace Spry
{
namespace Collections
{
	template<typename T>
	class LinkedList
	{
	public:
		void Add(const T& item)
		{
			_list.push_back(item);
		}

		void Clear()
		{
			_list.clear();
		}

		bool IsEmpty() const
		{
			return _list.empty();
		}

	private:
		std::list<T>	_list;
	};
}
}

#endif