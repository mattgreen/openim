#ifndef SPRY_STRINGTRAITS_HPP
#define SPRY_STRINGTRAITS_HPP

#include <cstdlib>
#include <cwchar>

#include "../String.hpp"

namespace Spry
{
namespace Metaprogramming
{
	namespace Internal
	{
		struct LengthImpl
		{
			inline static int GetLength(const char* s)
			{
				return static_cast<int>(strlen(s));
			}
			inline static int GetLength(const wchar_t* s)
			{
				return static_cast<int>(wcslen(s));
			}
		};
	}

	struct StringTraits
	{
		static int Compare(const char* s1, const char* s2)
		{
			return strcmp(s1, s2);
		}

		static int Compare(const wchar_t* s1, const wchar_t* s2)
		{
			return wcscmp(s1, s2);
		}

		template<typename T>
		static int GetLength(const std::basic_string<T>& s)
		{
			return static_cast<int>(s.length());
		}

		template<typename T>
		static int GetLength(const T* s)
		{
			return Internal::LengthImpl::GetLength(s);
		}
	};
}
}


#endif