#ifndef SPRY_INTERNAL_SHIMS_HPP
#define SPRY_INTERNAL_SHIMS_HPP

#include <cwchar>
#include <string>

#include "../Metaprogramming/Metaprogramming.hpp"
#include "../String.hpp"

namespace Spry
{
namespace Internal
{
	inline const char* GetStringBuffer(const char* s)
	{
		return s;
	}

	inline const wchar_t* GetStringBuffer(const wchar_t* s)
	{
		return s;
	}

	template<int N>
	inline char* GetStringBuffer(char s[N])
	{
		return &s[0];
	}

	template<int N>
	inline wchar_t* GetStringBuffer(wchar_t s[N])
	{
		return &s[0];
	}

	template<typename T, int N>
	inline T* GetStringBuffer(T s[N])
	{
		return &s[0];
	}

	template<typename T>
	inline const T* GetStringBuffer(const std::basic_string<T>& s)
	{
		return s.c_str();
	}
	inline int GetStringLength(const char* s)
	{
		return static_cast<int>(strlen(s));
	}

	inline int GetStringLength(const wchar_t* s)
	{
		return static_cast<int>(wcslen(s));
	}

	template<typename T, typename N>
	inline int GetStringLength(T s[N])
	{
		return N;
	}

	template<typename T>
	inline int GetStringLength(const std::basic_string<T>& s)
	{
		return static_cast<int>(s.length());
	}

	template<typename T>
	inline const T& MakeStreamable(const T& item)
	{
		return item;
	}

}
}


#endif