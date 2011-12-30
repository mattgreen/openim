#ifndef SPRY_INTERNAL_HANDLE_HPP
#define SPRY_INTERNAL_HANDLE_HPP

#include "../Assert.hpp"
#include "../OSException.hpp"

namespace Spry
{
namespace Internal
{
	template<typename T, T InvalidHandleValue>
	struct HandleTraits
	{
		typedef T		HandleType;

		static T GetInvalidHandleValue()
		{
			return InvalidHandleValue;
		}

		static void Close(T handle)
		{
			BOOL result = 0;
			if(IsValid(handle))
				result = CloseHandle(handle);
			Assert(result != 0);
			result;
		}

		static bool IsValid(T handle)
		{
			return handle != InvalidHandleValue;
		}
	};

	template<>
	struct HandleTraits<SOCKET, INVALID_SOCKET>
	{
		static void Close(SOCKET handle)
		{
			if(IsValid(handle))
				closesocket(handle);
		}

		static bool IsValid(SOCKET handle)
		{
			return handle != INVALID_SOCKET;
		}
	};

	template<typename T = HANDLE, T InvalidHandleValue = INVALID_HANDLE_VALUE>
	class Handle
	{
	public:
		Handle() :
			_handle(InvalidHandleValue)
		{
		}

		explicit Handle(T source) :
			_handle(source)
		{
			if(! IsValid())
				throw OSException();
		}

		Handle(const Handle& rhs)
		{
			if(! rhs.IsValid())
				return;

			DuplicateHandle(GetCurrentProcessHandle(),
				rhs._handle,
				GetCurrentProcessHandle(),
				&_handle,
				0,
				FALSE,
				DUPLICATE_SAME_ACCESS);
		}

		~Handle()
		{
			Close();
		}
	
		Handle& operator=(T h)
		{
			Close();
			
			_handle = h;
			return *this;
		}

		operator T() const
		{
			return _handle;
		}

		void Close()
		{
			if(IsValid())
			{
				HandleTraits<T, InvalidHandleValue>::Close(_handle);
				
				_handle = InvalidHandleValue;
			}
		}

		bool IsValid() const
		{
			return HandleTraits<T, InvalidHandleValue>::IsValid(_handle);
		}

	private:
		T _handle;
	};
}
}

#endif