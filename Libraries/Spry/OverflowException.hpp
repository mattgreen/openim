#ifndef SPRY_OVERFLOWEXCEPTION_HPP
#define SPRY_OVERFLOWEXCEPTION_HPP

#include "Exception.hpp"

namespace Spry
{
	struct OverflowException :
		public Exception
	{
		OverflowException() :
			Exception("OverflowException")
		{
		}
	};
}

#endif