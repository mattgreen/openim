#ifndef SPRY_INTERNAL_ASYNCDNSRESOLVER_HPP
#define SPRY_INTERNAL_ASYNCDNSRESOLVER_HPP

#include "../AsyncDNSResponseHandler.hpp"
#include "../../../Internal/SingletonComponent.hpp"

namespace Spry
{
namespace IO
{
namespace Networking
{
namespace Internal
{
	using Spry::Internal::SingletonComponent;
	using Spry::IO::Networking::AsyncDNSResponseHandler;

	class AsyncDNSResolver :
		public SingletonComponent<AsyncDNSResolver, 0xe2c87b97>
	{
	public:
		virtual int		Resolve(const char* host, AsyncDNSResponseHandler handler) = 0;
		virtual void	Cancel(int token) = 0;
	};
}
}
}
}

#endif