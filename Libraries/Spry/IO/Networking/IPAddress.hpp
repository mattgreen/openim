#ifndef SPRY_IO_NETWORKING_IPADDRESS_HPP
#define SPRY_IO_NETWORKING_IPADDRESS_HPP

#include "../../Assert.hpp"
//#include "../String.hpp"

namespace Spry
{
namespace IO
{
namespace Networking
{
	class IPAddress
	{
	public:
		static const unsigned long Any = INADDR_ANY;
		static const unsigned long Broadcast = INADDR_BROADCAST;
		static const unsigned long Loopback = INADDR_LOOPBACK;
		static const unsigned long None = INADDR_NONE;

		static const unsigned long AddressFamily = AF_INET;

		IPAddress()
		{
			_addr[0] = 0;
			_addr[1] = 0;
			_addr[2] = 0;
			_addr[3] = 0;
		}

		IPAddress(unsigned char a, unsigned char b, unsigned char c, unsigned char d)
		{
			_addr[0] = a;
			_addr[1] = b;
			_addr[2] = c;
			_addr[3] = d;
		}

		IPAddress(unsigned long n)
		{
			_addr[0] = static_cast<unsigned char>(n & 0xFF);
			_addr[1] = static_cast<unsigned char>(((n & 0xFF00) >> 8) & 0xFF);
			_addr[2] = static_cast<unsigned char>(((n & 0xFF0000) >> 16) & 0xFF);
			_addr[3] = static_cast<unsigned char>(((n & 0xFF000000) >> 24) & 0xFF);
		}

		operator const char*() const
		{
			return reinterpret_cast<const char*>(_addr);
		}

		operator unsigned long() const
		{
			return *reinterpret_cast<const unsigned long*>(_addr);
		}

		unsigned char operator[](int index)
		{
			Assert(index >= 0);
			Assert(index < 4);

			return _addr[index];
		}

/*		String ToString() const
		{
			return String::Format("%d.%d.%d.%d",
				_addr[0],
				_addr[1],
				_addr[2],
				_addr[3]);

		}
*/
	private:
		unsigned char _addr[4];
	};
}
}
}

#ifdef SPRY_AUTO_USING_DECLARATIONS
using Spry::IO::Networking::IPAddress;
#endif

#endif