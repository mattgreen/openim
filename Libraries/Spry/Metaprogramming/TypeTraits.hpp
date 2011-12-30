#ifndef SPRY_TYPETRAITS_HPP
#define SPRY_TYPETRAITS_HPP

#include "Conversion.hpp"

#include <cwchar>

namespace Spry
{
namespace Metaprogramming
{
	template<typename T>
	struct TypeTraits
	{
	private:
		template<typename U>
		struct BooleanTraits
		{
			enum { Result = false };
		};

		template<>
		struct BooleanTraits<bool>
		{
			enum { Result = true };
		};

		// Fundamental traits
		template<typename U>
		struct FundamentalTraits
		{
			enum { Result = false };
		};

		template<>
		struct FundamentalTraits<bool>
		{
			enum { Result = true };
		};

		template<>
		struct FundamentalTraits<char>
		{
			enum { Result = true };
		};

		template<>
		struct FundamentalTraits<signed char>
		{
			enum { Result = true };
		};

		template<>
		struct FundamentalTraits<unsigned char>
		{
			enum { Result = true };
		};

		template<>
		struct FundamentalTraits<signed short>
		{
			enum { Result = true };
		};

		template<>
		struct FundamentalTraits<unsigned short>
		{
			enum { Result = true };
		};

		template<>
		struct FundamentalTraits<signed int>
		{
			enum { Result = true };
		};

		template<>
		struct FundamentalTraits<unsigned int>
		{
			enum { Result = true };
		};

		template<>
		struct FundamentalTraits<signed long>
		{
			enum { Result = true };
		};

		template<>
		struct FundamentalTraits<unsigned long>
		{
			enum { Result = true };
		};

		template<>
		struct FundamentalTraits<float>
		{
			enum { Result = true };
		};

		template<>
		struct FundamentalTraits<double>
		{
			enum { Result = true };
		};

		template<typename U>
		struct VoidTraits
		{
			enum { Result = false };
		};
		
		template<>
		struct VoidTraits<void>
		{
			enum { Result = true };
		};

		// Character traits
		template<typename U>
		struct CharacterTraits
		{
			enum { Result = false };
		};

		template<>
		struct CharacterTraits<char>
		{
			enum { Result = true };
		};

		template<>
		struct CharacterTraits<wchar_t>
		{
			enum { Result = true };
		};

		// Pointer traits
		template<typename U>
		struct PointerTraits
		{
			enum { Result = false };
			typedef U Type;
		};

		template<typename U>
		struct PointerTraits<U*>
		{
			enum { Result = true };
			typedef U Type;
		};

		// Reference traits
		template<typename U>
		struct ReferenceTraits
		{
			enum { Result = false };
		};

		template<typename U>
		struct ReferenceTraits<U&>
		{
			enum { Result = true };
		};


		// Const traits
		template<typename U>
		struct ConstTraits
		{
			enum { Result = false };
		};

		template<typename U>
		struct ConstTraits<const U>
		{
			enum { Result = true };
		};

		// Volatile traits
		template<typename U>
		struct VolatileTraits
		{
			enum { Result = false };
		};

		template<typename U>
		struct VolatileTraits<volatile U>
		{
			enum { Result = true };
		};

		// String traits
		template<typename U>
		struct StringLiteralTraits
		{
			enum { Result = false };
		};

		template<int n>
		struct StringLiteralTraits<char[n]>
		{
			enum { Result = true };
		};

		template<int n>
		struct StringLiteralTraits<const char[n]>
		{
			enum { Result = true };
		};

		template<int n>
		struct StringLiteralTraits<wchar_t[n]>
		{
			enum { Result = true };
		};

		template<int n>
		struct StringLiteralTraits<const wchar_t[n]>
		{
			enum { Result = true };
		};

		template<class U>
		struct Unconst
		{
			typedef U Result;
		};

		template<class U>
		struct Unconst<const U>
		{
			typedef U Result;
		};

	public:

		enum
		{
			IsBoolean		= BooleanTraits<T>::Result,
			IsCharacter		= CharacterTraits<T>::Result,
			IsConst			= ConstTraits<T>::Result,
			IsFundamental	= FundamentalTraits<T>::Result,
			IsPointer		= PointerTraits<T>::Result,
			IsReference		= ReferenceTraits<T>::Result,
			IsString		= IsPointer && CharacterTraits<PointerTraits<T>::Type>::Result || StringLiteralTraits<T>::Result,
			IsVoid			= VoidTraits<T>::Result,
			IsVolatile		= VolatileTraits<T>::Result
		};

		typename typedef Unconst<T>::Result NonConstType;
	};
}
}

#endif