#ifndef SPRY_IO_NETWORKING_NETWORKING_HPP
#define SPRY_IO_NETWORKING_NETWORKING_HPP

#include "../../OSHeaders.hpp"

#pragma comment(lib, "wsock32.lib")

namespace Spry
{
namespace IO
{
namespace Networking
{
	inline signed short NetworkToHostOrder(signed short n)
	{
		return ntohs(static_cast<unsigned short>(n));
	}

	inline unsigned short NetworkToHostOrder(unsigned short n)
	{
		return ntohs(n);
	}

	inline signed int NetworkToHostOrder(signed int n)
	{
		return ntohl(static_cast<unsigned int>(n));
	}

	inline unsigned int NetworkToHostOrder(unsigned int n)
	{
		return ntohl(n);
	}

	inline signed long NetworkToHostOrder(signed long n)
	{
		return ntohl(static_cast<unsigned long>(n));
	}

	inline unsigned long NetworkToHostOrder(unsigned long n)
	{
		return ntohl(n);
	}

	inline signed __int64 NetworkToHostOrder(signed __int64 n)
	{
		char* num = reinterpret_cast<char*>(&n);
		char result[sizeof(n)] = { 0, };

		for(int i = 0; i < sizeof(n); i++)
			result[i] = num[(sizeof(n)-1)-i];

		return *reinterpret_cast<signed __int64*>(result);
	}

	inline unsigned __int64 NetworkToHostOrder(unsigned __int64 n)
	{
		return static_cast<unsigned __int64>(NetworkToHostOrder(static_cast<signed __int64>(n)));
	}


	inline signed short HostToNetworkOrder(signed short n)
	{
		return htons(static_cast<unsigned short>(n));
	}

	inline unsigned short HostToNetworkOrder(unsigned short n)
	{
		return htons(n);
	}

	inline signed int HostToNetworkOrder(signed int n)
	{
		return htonl(static_cast<unsigned int>(n));
	}

	inline unsigned int HostToNetworkOrder(unsigned int n)
	{
		return htonl(n);
	}

	inline signed long HostToNetworkOrder(signed long n)
	{
		return htonl(static_cast<unsigned long>(n));
	}

	inline unsigned long HostToNetworkOrder(unsigned long n)
	{
		return htonl(n);
	}
}
}
}


#endif