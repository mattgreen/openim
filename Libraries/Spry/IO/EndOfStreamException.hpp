#ifndef SPRY_IO_ENDOFSTREAMEXCEPTION_HPP
#define SPRY_IO_ENDOFSTREAMEXCEPTION_HPP

#include "../Exception.hpp"

namespace Spry
{
namespace IO
{
	struct EndOfStreamException :
		public Exception
	{
		EndOfStreamException() :
			Exception("EndOfStreamException")
		{
		}
	};
}
}

#endif