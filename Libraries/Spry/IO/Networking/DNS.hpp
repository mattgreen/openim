#ifndef SPRY_IO_NETWORKING_DNS_HPP
#define SPRY_IO_NETWORKING_DNS_HPP

#include "../../Delegate.hpp"
#include "Internal/AsyncDNSResolver.hpp"

#include "AsyncDNSResponse.hpp"
#include "AsyncDNSResponseHandler.hpp"
#include "DNSException.hpp"
#include "IPAddress.hpp"

namespace Spry
{
namespace IO
{
namespace Networking
{
	class DNS
	{
	public:
		static IPAddress Resolve(const char* hostname)
		{
			hostent* entries = gethostbyname(hostname);
			if(entries == 0)
			{
				switch(WSAGetLastError())
				{
				case WSAHOST_NOT_FOUND:
					return IPAddress::None;
					break;
				default:
					throw DNSException();
					break;
				}
			}

			unsigned long address = 0;
			memcpy(&address, entries->h_addr_list[0], entries->h_length);
			return IPAddress(address);
		}

		static int ResolveAsync(const char* address, AsyncDNSResponseHandler handler)
		{
			if(Internal::AsyncDNSResolver* resolver = Internal::AsyncDNSResolver::GetInstance())
				return resolver->Resolve(address, handler);
			else
				return 0;
		}

		static void Cancel(int token)
		{
			if(Internal::AsyncDNSResolver* resolver = Internal::AsyncDNSResolver::GetInstance())
				resolver->Cancel(token);
		}

	private:
		DNS();
		~DNS();
	};
}
}
}

#ifdef SPRY_AUTO_USING_DECLARATIONS
using Spry::IO::Networking::DNS;
#endif

#endif