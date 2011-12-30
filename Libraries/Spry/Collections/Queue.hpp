#ifndef SPRY_QUEUE_HPP
#define SPRY_QUEUE_HPP

#include <queue>

namespace Spry
{
namespace Collections
{
	template<typename T>
	class Queue
	{
	public:
		void Clear()
		{
			while(! IsEmpty())
				_queue.pop();
		}

		T Dequeue()
		{
			try
			{
				T item = _queue.front();
				_queue.pop();

				return item;
			}
			catch(...)
			{
				throw Spry::Exception("Queue Item type's constructor caused an exception.");
			}
		}

		void Enqueue(const T& item)
		{
			_queue.push(item);
		}

		int GetSize() const
		{
			return _queue.size();
		}

		bool IsEmpty() const
		{
			return _queue.empty();
		}

		__declspec(property(get=GetSize))	int Size;

	private:
		std::queue<T> _queue;
	};
}
}

#ifdef SPRY_AUTO_USING_DECLARATIONS
using Spry::Collections::Queue;
#endif

#endif