#ifndef SPRY_NETWORKING_WINSOCKINITIALIZER_HPP
#define SPRY_NETWORKING_WINSOCKINITIALIZER_HPP

#include "../../../OSHeaders.hpp"

namespace Spry
{
namespace IO
{
namespace Networking
{
namespace Internal
{
	struct Winsock
	{
		static void Initialize()
		{
			WSADATA data;
			WSAStartup(MAKEWORD(1, 1), &data);
		}

		static void Uninitialize()
		{
			WSACleanup();
		}
	};

	struct WinsockInitializer
	{
		WinsockInitializer()
		{
			Winsock::Initialize();
		}

		~WinsockInitializer()
		{
			Winsock::Uninitialize();
		}
	};
	
	__declspec(selectany) WinsockInitializer Initializer;
}
}
}
}

#endif