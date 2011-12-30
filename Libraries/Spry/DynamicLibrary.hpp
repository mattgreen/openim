#ifndef SPRY_DYNAMICLIBRARY_HPP
#define SPRY_DYNAMICLIBRARY_HPP

#include "OSHeaders.hpp"

#include "DynamicLibraryException.hpp"
#include "Utilities.hpp"

namespace Spry
{
	namespace Internal
	{
		inline HMODULE LoadLibraryImpl(const char* filename)
		{
			return LoadLibraryA(filename);
		}

		inline HMODULE LoadLibraryImpl(const wchar_t* filename)
		{
			return LoadLibraryW(filename);
		}
	}

	class DynamicLibrary
	{
	public:
		explicit DynamicLibrary(HMODULE mod = 0) :
			_handle(mod)
		{
		}

		template<typename String>
		explicit DynamicLibrary(const String& filename) :
			_handle(Internal::LoadLibraryImpl(Spry::Internal::GetStringBuffer(filename)))
		{
			if(_handle == 0)
				throw DynamicLibraryException();
		}

		~DynamicLibrary() throw()
		{
			if(_handle == 0)
				return;

			if(_refs.IsLast())
				FreeLibrary(_handle);
		}

		DynamicLibrary& operator=(const HMODULE module)
		{
			if(_handle != 0)
			{
				if(_refs.IsLast())
					FreeLibrary(_handle);
			}

			_handle = module;
			return *this;
		}

		bool operator==(HMODULE rhs) const
		{
			return _handle == rhs;
		}

		bool operator!=(HMODULE rhs) const
		{
			return ! operator==(rhs);
		}

		HMODULE GetHandle()
		{
			return _handle;
		}

		template<typename Symbol>
		Symbol GetSymbol(const char* symbol)
		{
			if(_handle == 0)
				throw DynamicLibraryException();

			return reinterpret_cast<Symbol>(GetProcAddress(_handle, symbol));
		}

		bool HasSymbol(const char* symbol)
		{
			return GetSymbol<void*>(symbol) != 0;
		}

	private:
		HMODULE					_handle;
		Internal::ReferenceLink	_refs;
	};
}

#endif