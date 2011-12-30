#ifndef SPRY_REFERENCECOUNT_HPP
#define SPRY_REFERENCECOUNT_HPP

namespace Spry
{
namespace Internal
{
	class ReferenceCount
	{
	public:
		ReferenceCount() :
			_count()
		{
		}

		ReferenceCount(const ReferenceCount& rhs)
		{
			if(rhs._count == 0)
				rhs._count = new unsigned int(1);
			
			_count = rhs._count;
			(*_count)++;
		}

		~ReferenceCount() throw()
		{
			if(_count == 0)
				return;

			Decrement();
		}

		ReferenceCount& operator=(const ReferenceCount& rhs)
		{
			if(_count != 0)
			{
				Decrement();
			}

			if(rhs._count == 0)
				rhs._count = new unsigned int(1);
			_count = rhs._count;
			(*_count)++;

			return *this;
		}

		operator int() const
		{
			if(_count == 0)
				return 1;
			else
				return *_count;
		}

	private:
		void Decrement()
		{
			(*_count)--;
			if(*_count == 0)
				delete _count;
		}

	private:
		mutable unsigned int* _count;
	};
}
}

#endif