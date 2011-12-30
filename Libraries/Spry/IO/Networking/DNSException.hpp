#ifndef SPRY_NETWORKING_DNSEXCEPTION_HPP
#define SPRY_NETWORKING_DNSEXCEPTION_HPP

#include "../IOException.hpp"

namespace Spry
{
namespace IO
{
namespace Networking
{
	struct DNSException :
		public Spry::IO::IOException
	{
		DNSException() :
			Spry::IO::IOException("DNSException")
		{
		}
	};
}
}
}

#ifdef SPRY_AUTO_USING_DECLARATIONS
using Spry::IO::Networking::DNSException;
#endif

#endif