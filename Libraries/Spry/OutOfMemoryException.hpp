#ifndef SPRY_OUTOFMEMORYEXCEPTION_HPP
#define SPRY_OUTOFMEMORYEXCEPTION_HPP

#include "Exception.hpp"

namespace Spry
{
	struct OutOfMemoryException :
		public Exception
	{
		OutOfMemoryException() :
			Exception("OutOfMemoryException")
		{
		}
	};
}

#endif