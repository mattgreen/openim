#ifndef SPRY_IO_IOEXCEPTION_HPP
#define SPRY_IO_IOEXCEPTION_HPP

#include "../OSException.hpp"

namespace Spry
{
namespace IO
{
	struct IOException :
		public OSException
	{
		IOException(const char* name = "IOException") :
			OSException(name)
		{
		}
	};
}
}

#ifdef SPRY_AUTO_USING_DECLARATIONS
using Spry::IO::IOException;
#endif

#endif