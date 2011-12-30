#ifndef SPRY_NETWORKING_SOCKETEXCEPTION_HPP
#define SPRY_NETWORKING_SOCKETEXCEPTION_HPP

#include "../../OSException.hpp"

namespace Spry
{
namespace IO
{
namespace Networking
{
	struct SocketException :
		public Spry::OSException
	{
		SocketException() :
			Spry::OSException("SocketException")
		{
		}
	};
}
}
}


#endif