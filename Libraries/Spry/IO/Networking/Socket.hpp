#ifndef SPRY_NETWORKING_SOCKET_HPP
#define SPRY_NETWORKING_SOCKET_HPP

#include "../../Internal/SocketHandle.hpp"
#include "../../OSHeaders.hpp"

#include "Internal/WinsockInitializer.hpp"

#include "IPAddress.hpp"
#include "SocketException.hpp"

namespace Spry
{
namespace IO
{
namespace Networking
{
	struct SocketStatus
	{
		enum
		{
			Disconnected,
			Resolving,
			Connecting,
			Connected,
			Disconnecting,
			Bound
		};
	};

	class Socket
	{
	public:
		// Associates a local endpoint with a socket handle.
		// Throws: SocketException
		void Bind(unsigned short port)
		{
			Bind(IPAddress::Any, port);
		}

		// Associates a local endpoint with a socket handle.
		// Throws: SocketException
		virtual void Bind(IPAddress address, unsigned short port)
		{
			sockaddr_in sa;
			sa.sin_family		= static_cast<short>(_family);
			sa.sin_addr.s_addr	= ::htonl(address);
			sa.sin_port			= ::htons(port);

			if(! IsAllocated())
				Allocate();

			int result = bind(_handle, reinterpret_cast<sockaddr*>(&sa), sizeof sa);
			if(result == 0)
			{
				_boundAddress = address;
				_boundPort = port;

				_status = SocketStatus::Bound;
			}
			else
				throw SocketException();
		}

		SOCKET GetHandle() const
		{
			return _handle;
		}

		int GetStatus() const
		{
			return _status;
		}

		__declspec(property(get=GetStatus))	int	Status;

	protected:
		Socket(int family, int type, short protocol) :
			_family(family),
			_type(type),
			_protocol(protocol),
			_status(SocketStatus::Disconnected)
		{
		}

		// Construct socket from existing handle, assuming ownership.
		Socket(SOCKET s, int family, int type, int protocol) :
			_handle(s),
			_family(family),
			_type(type),
			_protocol(protocol),
			_status(SocketStatus::Connected)
		{
			Assert(_handle.IsValid());
		}

		// Allocate the underlying socket handle.
		// Throws: SocketException
		virtual void Allocate()
		{
			if(! IsAllocated())
			{
				_handle = socket(_family, _type, _protocol);
				if(_handle == 0)
					throw SocketException();
			}
		}

		// Returns true if the underlying socket handle has been allocated.
		bool IsAllocated() const
		{
			return _handle.IsValid();
		}

		void SetStatus(int newStatus)
		{
			_status = newStatus;
		}

		// Close the underlying socket handle.
		virtual void Release() throw()
		{
			if(IsAllocated())
			{
				_handle.Close();

				_boundAddress = IPAddress::None;
				_boundPort = -1;
			}
		}

	private:
		Socket(Socket&);
		Socket& operator=(Socket&);

	private:
		Spry::Internal::SocketHandle	_handle;
		int								_status;
		
		IPAddress						_boundAddress;
		int								_boundPort;

		int								_family;
		int								_type;
		int								_protocol;
	};

}
}
}


#endif