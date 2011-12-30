#ifndef SPRY_REAL_HPP
#define SPRY_REAL_HPP

#include <cerrno>
#include <cfloat>
#include <cstdlib>

#include "FormatException.hpp"
#include "OverflowException.hpp"
#include "Utilities.hpp"

namespace Spry
{
namespace Internal
{
	struct RealImpl
	{
		static double Convert(const char* str, char** endPtr)
		{
			return ::strtod(str, endPtr);
		}

		static double Convert(const wchar_t* str, wchar_t** endPtr)
		{
			return ::wcstod(str, endPtr);
		}
	};

	template<typename T>
	struct Real
	{
		template<typename Char>
		static T Parse(const Char* str)
		{
			Assert(str != 0);

			Char* stoppingChar = const_cast<Char*>(str);

			double result = RealImpl::Convert(str, &stoppingChar);

			if(result == 0 && stoppingChar == str)
				throw FormatException();
			else if(errno == ERANGE)
				throw OverflowException();
			else
			{
				return static_cast<T>(result);
			}
		}

		template<typename String>
		static T Parse(const String& str)
		{
			return Parse(Internal::GetStringBuffer(str));
		}
	};

namespace FloatImpl
{
	struct FloatConstants
	{
		static const float Epsilon;
		static const float MaximumValue;
		static const float MinimumValue;
	};

	__declspec(selectany) const float FloatConstants::Epsilon = FLT_EPSILON;
	__declspec(selectany) const float FloatConstants::MaximumValue = FLT_MAX;
	__declspec(selectany) const float FloatConstants::MinimumValue = FLT_MIN;

}

namespace DoubleImpl
{
	struct DoubleConstants
	{
		static const double Epsilon;
		static const double MaximumValue;
		static const double MinimumValue;
	};

	__declspec(selectany) const double DoubleConstants::Epsilon = DBL_EPSILON;
	__declspec(selectany) const double DoubleConstants::MaximumValue = DBL_MAX;
	__declspec(selectany) const double DoubleConstants::MinimumValue = DBL_MIN;
}

}

	struct Single :
		public Internal::Real<float>,
		public Internal::FloatImpl::FloatConstants
	{
	};

	struct Double :
		public Internal::Real<double>,
		public Internal::DoubleImpl::DoubleConstants
	{
	};
}

#endif