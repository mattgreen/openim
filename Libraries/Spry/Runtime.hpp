#ifndef SPRY_RUNTIME_HPP
#define SPRY_RUNTIME_HPP

#include "Internal/Runtime.hpp"
#include "Internal/MessageReactorImpl.hpp"
#include "IO/Networking/Internal/AsyncDNSResolverImpl.hpp"

namespace Spry
{
	class Runtime
	{
	public:
		Runtime(HMODULE module)
		{
			Spry::Internal::Runtime::Host(module);
			
			Spry::Internal::Runtime::AddFactory<Internal::MessageReactorImpl>();
#ifndef SPRY_NO_ASYNC_DNS_RESOLUTION
			Spry::Internal::Runtime::AddFactory<IO::Networking::Internal::AsyncDNSResolverImpl>();
#endif
		}

		~Runtime()
		{
			Spry::Internal::Runtime::Shutdown();
		}
	};
}

#endif