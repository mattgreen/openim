#ifndef SPRY_FORMATEXCEPTION_HPP
#define SPRY_FORMATEXCEPTION_HPP

#include "Exception.hpp"

namespace Spry
{
	struct FormatException :
		public Exception
	{
		FormatException() :
			Exception("FormatException")
		{
		}
	};
}

#endif