#ifndef SPRY_STACK_HPP
#define SPRY_STACK_HPP

namespace Spry
{
namespace Collections
{
	template<typename T>
	class Stack
	{
	public:
		void Clear()
		{
			while(! IsEmpty())
				_stack.pop();
		}

		int GetSize() const
		{
			return _stack.size();
		}

		bool IsEmpty() const
		{
			return _stack.empty();
		}

		T Pop()
		{
			try
			{
				T item = _stack.top();
				_stack.pop();
				return item;
			}
			catch(...)
			{
				throw Spry::Exception("Stack item type's constructor caused an exception.");
			}
		}

		void Push(const T& item)
		{
			_stack.push(item);
		}

		__declspec(property(get=GetSize))	int Size;

	private:
		std::stack<T> _stack;
	};
}
}

#ifdef SPRY_AUTO_USING_DECLARATIONS
using Spry::Collections::Stack;
#endif

#endif