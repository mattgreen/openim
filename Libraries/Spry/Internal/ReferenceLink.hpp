#ifndef SPRY_INTERNAL_REFERENCELINK_HPP
#define SPRY_INTERNAL_REFERENCELINK_HPP

namespace Spry
{
namespace Internal
{
	class ReferenceLink
	{
	public:
		ReferenceLink()
		{
			_previous = this;
			_next = this;
		}

		ReferenceLink(const ReferenceLink& link)
		{
			Acquire(link);
		}

		~ReferenceLink() throw()
		{
			Release();
		}

		ReferenceLink& operator=(const ReferenceLink& rhs)
		{
			if(this != &rhs)
			{
				Release();
				Acquire(rhs);
			}

			return *this;
		}

		bool IsLast() const throw()
		{
			if(_previous == 0)
				return true;

			return _previous == this;
		}

	private:
		void Acquire(const ReferenceLink& link)
		{
			_next = link._next;
			_next->_previous = this;
			_previous = &link;
			link._next = this;
		}

		void Release() throw()
		{ 
			if (! IsLast())
			{
				_previous->_next = _next;
				_next->_previous = _previous;

				_next = 0;
				_previous = 0;
			}
		}

	private:
		mutable const ReferenceLink*	_previous;
		mutable const ReferenceLink*	_next;
	};
}
}

#endif