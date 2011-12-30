#ifndef SPRY_INTEGER_HPP
#define SPRY_INTEGER_HPP

#include <cerrno>
#include <cstdlib>

#include "Assert.hpp"
#include "Metaprogramming/Metaprogramming.hpp"
#include "FormatException.hpp"
#include "OverflowException.hpp"
#include "Utilities.hpp"

namespace Spry
{
	namespace Internal
	{
		struct IntegerImpl
		{
			static unsigned long Convert(const char* s, char** end, int base, Spry::Metaprogramming::IntToType<false>)
			{
				return ::strtoul(s, end, base);
			}

			static unsigned long Convert(const wchar_t* s, wchar_t** end, int base, Spry::Metaprogramming::IntToType<false>)
			{
				return ::wcstoul(s, end, base);
			}

			static long Convert(const char* s, char** end, int base, Spry::Metaprogramming::IntToType<true>)
			{
				return ::strtol(s, end, base);
			}

			static long Convert(const wchar_t* s, wchar_t** end, int base, Spry::Metaprogramming::IntToType<true>)
			{
				return ::wcstol(s, end, base);
			}
		};

		template<typename T>
		struct Integer
		{
			enum
			{
				MinimumValue = Spry::Metaprogramming::NumericTraits<T>::MinimumValue,
				MaximumValue = Spry::Metaprogramming::NumericTraits<T>::MaximumValue
			};

			template<typename Char>
			static T Parse(const Char* s, int base = 10)
			{
				Assert(base >= 0);

				Char* endPtr = const_cast<Char*>(s);

				long result = IntegerImpl::Convert(s, &endPtr, base,
					Spry::Metaprogramming::IntToType<Spry::Metaprogramming::NumericTraits<T>::IsSigned>());

				if(result == 0 && endPtr == s)
					throw FormatException();
				if(errno == ERANGE)
					throw OverflowException();
				if(result < MinimumValue || result > MaximumValue)
					throw OverflowException();

				return static_cast<T>(result);
			}

			template<typename String>
			static T Parse(const String& s, int base = 10)
			{
				return Parse(GetStringBuffer(s), base);
			}
		};
	}

	struct Int8 :
		public Internal::Integer<signed __int8>
	{
	};

	struct UInt8 :
		public Internal::Integer<unsigned __int8>
	{
	};

	struct Int16 :
		public Internal::Integer<signed __int16>
	{
	};

	struct UInt16 :
		public Internal::Integer<unsigned __int16>
	{
	};

	struct Int32 :
		public Internal::Integer<signed __int32>
	{
	};

	struct UInt32 :
		public Internal::Integer<unsigned __int32>
	{
	};
}

#endif