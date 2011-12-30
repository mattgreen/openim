#ifndef SPRY_NETWORKING_ASYNCDNSRESPONSEHANDLER_HPP
#define SPRY_NETWORKING_ASYNCDNSRESPONSEHANDLER_HPP

#include "../../Delegate.hpp"

#include "AsyncDNSResponse.hpp"

namespace Spry
{
namespace IO
{
namespace Networking
{
	typedef Delegate<void (const AsyncDNSResponse&)>	AsyncDNSResponseHandler;
}
}
}

#ifdef SPRY_AUTO_USING_DECLARATIONS
using Spry::IO::Networking::AsyncDNSResponse;
#endif

#endif