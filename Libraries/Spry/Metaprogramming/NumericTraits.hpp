#ifndef SPRY_METAPROGRAMMING_NUMERICTRAITS_HPP
#define SPRY_METAPROGRAMMING_NUMERICTRAITS_HPP

#include <climits>
#include <cfloat>

namespace Spry
{
namespace Metaprogramming
{
	template<typename T>
	struct NumericTraits
	{
	private:
		template<typename U>
		struct LimitTraits
		{
			enum
			{
				MinimumValue = 0,
				MaximumValue = 0
			};
		};

		template<>
		struct LimitTraits<signed char>
		{
			enum
			{
				MinimumValue = CHAR_MIN,
				MaximumValue = CHAR_MAX
			};
		};

		template<>
		struct LimitTraits<unsigned char>
		{
			enum
			{
				MinimumValue = 0,
				MaximumValue = UCHAR_MAX
			};
		};

		template<>
		struct LimitTraits<signed short>
		{
			enum
			{
				MinimumValue = SHRT_MIN,
				MaximumValue = SHRT_MAX
			};
		};

		template<>
		struct LimitTraits<unsigned short>
		{
			enum
			{
				MinimumValue = 0,
				MaximumValue = USHRT_MAX
			};
		};

		template<>
		struct LimitTraits<signed int>
		{
			enum
			{
				MinimumValue = INT_MIN,
				MaximumValue = INT_MAX
			};
		};

		template<>
		struct LimitTraits<unsigned int>
		{
			enum
			{
				MinimumValue = 0,
				MaximumValue = UINT_MAX
			};
		};

		template<>
		struct LimitTraits<signed long>
		{
			enum
			{
				MinimumValue = LONG_MIN,
				MaximumValue = LONG_MAX
			};
		};

		template<>
		struct LimitTraits<unsigned long>
		{
			enum
			{
				MinimumValue = 0,
				MaximumValue = ULONG_MAX
			};
		};

/*		template<>
		struct LimitTraits<signed __int64>
		{
			enum
			{
				MinimumValue = -9223372036854775808,
				MaximumValue = 9223372036854775807
			};
		};

		template<>
		struct LimitTraits<unsigned __int64>
		{
			enum
			{
				MinimumValue = 0,
				MaximumValue = 0xFFFFFFFFFFFFFFFF
			};
		};
*/
		template<typename U>
		struct SignedTraits
		{
			enum { Result = true };
		};

		template<>
		struct SignedTraits<unsigned char>
		{
			enum { Result = false };
		};

		template<>
		struct SignedTraits<unsigned short>
		{
			enum { Result = false };
		};

		template<>
		struct SignedTraits<unsigned int>
		{
			enum { Result = false };
		};

		template<>
		struct SignedTraits<unsigned long>
		{
			enum { Result = false };
		};

		template<>
		struct SignedTraits<unsigned __int64>
		{
			enum { Result = false };
		};

	public:
		enum
		{
			IsSigned = SignedTraits<T>::Result,
			IsUnsigned = ! IsSigned,
			MaximumValue = LimitTraits<T>::MaximumValue,
			MinimumValue = LimitTraits<T>::MinimumValue,
		};
	};
}
}

#endif