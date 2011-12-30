#ifndef SPRY_TYPECONVERSION_HPP
#define SPRY_TYPECONVERSION_HPP

#include <cwchar>

#include "NullType.hpp"

namespace Spry
{
namespace Metaprogramming
{
	template<typename T>
	struct TypeConversion
	{
	private:
		template<typename U>
		struct ConstTypeConversion
		{
			typedef const U	Type;
		};

		template<>
		struct ConstTypeConversion<char*>
		{
			typedef const char*	Type;
		};

		template<>
		struct ConstTypeConversion<wchar_t*>
		{
			typedef const wchar_t* Type;
		};

		template<typename U>
		struct ConstTypeConversion<const U>
		{
			typedef U		Type;
		};

		template<typename U>
		struct PointeeTypeConversion
		{
			typedef NullType Type;
		};

		template<typename U>
		struct PointeeTypeConversion<const U*>
		{
			typedef U Type;
		};

		template<typename U>
		struct PointeeTypeConversion<U*>
		{
			typedef U Type;
		};

		template<typename U, int N>
		struct PointeeTypeConversion<const U[N]>
		{
			typedef U Type;
		};

		template<typename U>
		struct PointeeTypeConversion<U[]>
		{
			typedef U Type;
		};

		template<typename U>
		struct NonConst
		{
			typedef U Type;
		};

		template<typename U>
		struct NonConst<const U>
		{
			typedef U Type;
		};

		template<typename U>
		struct NonVolatile
		{
			typedef U Type;
		};

		template<typename U>
		struct NonVolatile<volatile U>
		{
			typedef U Type;
		};

	public:
		typedef typename ConstTypeConversion<T>::Type	ConstType;
		typedef typename PointeeTypeConversion<T>::Type	PointeeType;
		typedef typename NonConst<T>::Type				NonConstType;
		typedef typename NonVolatile<T>::Type			NonVolatileType;
	};
}
}

#endif