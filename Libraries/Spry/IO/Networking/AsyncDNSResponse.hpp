#ifndef SPRY_ASYNCDNSRESPONSE_HPP
#define SPRY_ASYNCDNSRESPONSE_HPP

#include "IPAddress.hpp"

namespace Spry
{
namespace IO
{
namespace Networking
{
	struct AsyncDNSResponse
	{
		IPAddress		Address;
		unsigned int	ID;
		bool			Success;
	};
}
}
}

#ifdef SPRY_AUTO_USING_DECLARATIONS
using Spry::IO::Networking::AsyncDNSResponse;
#endif

#endif