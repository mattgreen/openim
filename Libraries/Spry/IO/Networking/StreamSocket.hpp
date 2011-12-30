#ifndef SPRY_NETWORKING_STREAMSOCKET_HPP
#define SPRY_NETWORKING_STREAMSOCKET_HPP

#include "../../Assert.hpp"

#include "DNS.hpp"
#include "Socket.hpp"

namespace Spry
{
namespace IO
{
namespace Networking
{
	class StreamSocket :
		public Socket
	{
	public:
		StreamSocket() :
			Socket(AF_INET, SOCK_STREAM, 0)
		{
		}

		void Accept()
		{
			
		}

		void Connect(const char* hostname, unsigned short port)
		{
			Assert(hostname != 0);
			
			unsigned long address = inet_addr(hostname);
			if(address == INADDR_NONE)
				address = DNS::Resolve(hostname);

			Connect(address, port);
		}

		void Connect(IPAddress address, unsigned short port)
		{
			Assert(GetStatus() == SocketStatus::Disconnected);

			if(! IsAllocated())
				Allocate();

			sockaddr_in sa;
			sa.sin_family = address.AddressFamily;
			sa.sin_addr.S_un.S_addr = address;
			sa.sin_port = htons(port);
			
			int result = connect(GetHandle(), reinterpret_cast<sockaddr*>(&sa), sizeof sa);
			if(result != SOCKET_ERROR)
			{
				SetStatus(SocketStatus::Connected);
			}
			else
			{
				int error = WSAGetLastError();
				if(error == WSAEWOULDBLOCK)
					SetStatus(SocketStatus::Connecting);
				else
					throw SocketException();
			}
		}

		// Begin listening for incoming connections.
		// Throws: SocketException
		void Listen(int backlog = 5)
		{
			Assert(backlog > 0);
			Assert(GetStatus() == SocketStatus::Bound);

			if(listen(GetHandle(), backlog) == SOCKET_ERROR)
				throw SocketException();
		}

		// Receive data from a connected socket.
		// Returns: number of bytes received
		// Throws: SocketException
		int Receive(char* buffer, int length)
		{
			Assert(buffer != NULL);
			Assert(length > 0);
			Assert(GetStatus() == SocketStatus::Connected);

			int result = recv(GetHandle(), buffer, length, 0);
			switch(result)
			{
			case 0:
				throw SocketException();
				break;

			case SOCKET_ERROR:
				if(GetLastError() == WSAEWOULDBLOCK)
					result = 0;
				else
					HandleError();
				break;
			}

			return result;
		}

		// Send data on a connected socket.
		// Returns: number of bytes sent.
		// Throws: SocketException
		int Send(const char* buffer, int length)
		{
			Assert(buffer != NULL);
			Assert(length >= 0);
			Assert(GetStatus() == SocketStatus::Connected);
			
			if(length == 0)
				return 0;

			int result = send(GetHandle(), buffer, length, 0);
			if(result == SOCKET_ERROR)
				HandleError();

			return result;
		}

	protected:
		StreamSocket(SOCKET s) :
			Socket(s, AF_INET, SOCK_STREAM, 0)
		{
		}

	private:
		virtual void HandleError()
		{
			throw SocketException();
		}
	};
}
}
}

#endif