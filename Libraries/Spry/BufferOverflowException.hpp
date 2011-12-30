#ifndef SPRY_BUFFEROVERFLOWEXCEPTION_HPP
#define SPRY_BUFFEROVERFLOWEXCEPTION_HPP

#include "Exception.hpp"

namespace Spry
{
	struct BufferOverflowException :
		public Exception
	{
		BufferOverflowException() :
			Exception("BufferOverflowException")
		{
		}
	};
}

#endif