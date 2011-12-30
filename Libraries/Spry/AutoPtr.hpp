#ifndef SPRY_AUTOPTR_HPP
#define SPRY_AUTOPTR_HPP

namespace Spry
{
	template<typename T>
	class AutoPtr
	{
	public:
		AutoPtr(T* ptr = 0) :
			_ptr(ptr)
		{
		}

		AutoPtr(AutoPtr<T>& ap) 
		{
			Reset(ap.Release());
		}

		~AutoPtr()
		{
			delete _ptr;
		}

		AutoPtr<T>& operator=(T* r)
		{
			Reset(r);

			return *this;
		}

		AutoPtr<T>& operator=(AutoPtr<T>& r)
		{
			Reset(r.Release());

			return *this;
		}

		bool operator==(T* ptr) const
		{
			return _ptr == ptr;
		}

		bool operator!=(T* ptr) const
		{
			return ! operator==(ptr);
		}

		T& operator*() const
		{
			return *_ptr;
		}

		T* operator->() const
		{
			return _ptr;
		}

		T* Get()
		{
			return _ptr;
		}

		T* Release()
		{
			T* p = _ptr;
			_ptr = 0;
			return p;
		}

		void Reset(T* ptr)
		{
			if(ptr != _ptr)
				delete _ptr;
			_ptr = ptr;
		}

	private:
		T* _ptr;
	};
}


#endif