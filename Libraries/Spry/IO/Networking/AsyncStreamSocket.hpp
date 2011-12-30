#ifndef SPRY_IO_NETWORKING_ASYNCSTREAMSOCKET_HPP
#define SPRY_IO_NETWORKING_ASYNCSTREAMSOCKET_HPP

#include "../../Delegate.hpp"

#include "../../Internal/MessageReactor.hpp"
#include "../../Internal/Shims.hpp"

#include "AsyncResult.hpp"
#include "DNS.hpp"
#include "StreamSocket.hpp"

#define WM_SOCKET (WM_USER + 32)

namespace Spry
{
namespace IO
{
namespace Networking
{
	typedef Spry::Delegate<void (const AsyncResult&)> AsyncSocketHandler;

	class AsyncStreamSocket :
		private StreamSocket
	{
	public:
		AsyncStreamSocket() :
			_canAccept(false),
			_canReceive(false),
			_canSend(false),
			_shutdown(false),
			_dnsId(0),
			_port(0)
		{
		}
		
		~AsyncStreamSocket()
		{
			Release();
		}

		void Connect(IPAddress address, unsigned short port)
		{
			StreamSocket::Connect(address, port);
		}

		// Begin establishing a connection to a remote host.
		// Throws: DNSException, SocketException
		template<typename String>
		void Connect(const String& address, unsigned short port)
		{
			const char* str = Spry::Internal::GetStringBuffer(address);
			
			unsigned long addr = inet_addr(str);
			if(addr != INADDR_NONE)
			{
				StreamSocket::Connect(addr, port);
			}
			else
			{
				SetStatus(SocketStatus::Resolving);
				_port = port;

				_dnsId = DNS::ResolveAsync(str,	AsyncDNSResponseHandler(this, &AsyncStreamSocket::OnDNSResolutionNotification));
			}
		}

		void Disconnect()
		{
			switch(GetStatus())
			{
			case SocketStatus::Connected:
				shutdown(GetHandle(), SD_BOTH);
				_shutdown = true;

				break;
			
			case SocketStatus::Connecting:
			case SocketStatus::Bound:
				Release();

				break;

			case SocketStatus::Resolving:
				DNS::Cancel(_dnsId);
				Initialize();

				break;
			}
		}

		int Receive(char* buffer, int length)
		{
			return StreamSocket::Receive(buffer, length);
		}

		int Send(const char* buffer, int length)
		{
			return StreamSocket::Send(buffer, length);
		}

		AsyncSocketHandler OnAccept;
		AsyncSocketHandler OnConnect;
		AsyncSocketHandler OnDisconnect;
		AsyncSocketHandler OnOutOfBandData;
		AsyncSocketHandler OnReceive;
		AsyncSocketHandler OnSend;

	protected:
		explicit AsyncStreamSocket(SOCKET s) :
			StreamSocket(s),
			_canAccept(false),
			_canReceive(false),
			_canSend(false),
			_dnsId(0),
			_port(0)
		{
			Allocate();
		}

		void Allocate()
		{
			StreamSocket::Allocate();

			Spry::Internal::MessageReactor::GetInstance()->AddMessageHandler(WM_SOCKET,
					static_cast<WPARAM>(GetHandle()),
					Spry::Internal::MessageHandler(this, &AsyncStreamSocket::OnMessage));

			int result = WSAAsyncSelect(GetHandle(),
				Spry::Internal::MessageReactor::GetInstance()->GetHWND(),
				WM_SOCKET,
				FD_ACCEPT | FD_CONNECT | FD_CLOSE | FD_OOB | FD_READ | FD_WRITE);

			if(result == SOCKET_ERROR)
			{
				Release();

				throw SocketException();
			}
		}

		void Initialize()
		{
			SetStatus(SocketStatus::Disconnected);
			_dnsId = 0;
			_port = 0;

			_canAccept = false;
			_canReceive = false;
			_canSend = false;
			_shutdown = false;
		}

		void OnAcceptNotification(const AsyncResult& result)
		{
			if(_shutdown)
				return;

			_canAccept = true;

			if(OnAccept != 0)
				OnAccept(result);
		}

		void OnConnectNotification(const AsyncResult& result)
		{
			if(result.Success)
				SetStatus(SocketStatus::Connected);
			else
				SetStatus(SocketStatus::Disconnected);

			if(OnConnect != 0)
				OnConnect(result);
		}

		void OnDisconnectNotification(const AsyncResult& result)
		{
			Release();
			Initialize();

			if(OnDisconnect != 0)
				OnDisconnect(result);
		}

		void OnDNSResolutionNotification(const AsyncDNSResponse& result)
		{
			SetStatus(SocketStatus::Disconnected);
			if(result.Success)
			{
				StreamSocket::Connect(static_cast<unsigned long>(result.Address), static_cast<unsigned short>(_port));
			}
			else
			{
				_dnsId = 0;
				_port = 0;
			}
		}

		// Called when a message is received for this asynchronous socket instance.
		virtual void OnMessage(WPARAM sock, LPARAM lParam)
		{
			if(GetHandle() != static_cast<SOCKET>(sock))
				return;

			AsyncResult result;
			result.Error = WSAGETSELECTERROR(lParam);
			result.Success = result.Error == 0;

			switch(WSAGETSELECTEVENT(lParam))
			{
			case FD_ACCEPT:
				OnAcceptNotification(result);
				break;

			case FD_CONNECT:
				OnConnectNotification(result);
				break;

			case FD_CLOSE:
				OnDisconnectNotification(result);
				break;

			case FD_OOB:
				OnOutOfBandDataNotification(result);
				break;

			case FD_READ:
				OnReceiveNotification(result);
				break;

			case FD_WRITE:
				OnSendNotification(result);
				break;
			}
		}

		void OnOutOfBandDataNotification(const AsyncResult& result)
		{
			if(_shutdown)
				return;

			if(OnOutOfBandData != 0)
				OnOutOfBandData(result);
		}

		void OnReceiveNotification(const AsyncResult& result)
		{
			if(_shutdown)
				return;

			_canReceive = true;

			if(OnReceive != 0)
				OnReceive(result);
		}

		void OnSendNotification(const AsyncResult& result)
		{
			if(_shutdown)
				return;

			_canSend = true;

			if(OnSend != 0)
				OnSend(result);
		}


		void Release()
		{
			::WSAAsyncSelect(GetHandle(),
				Spry::Internal::MessageReactor::GetInstance()->GetHWND(),
				0,
				0);

			Spry::Internal::MessageReactor::GetInstance()->RemoveMessageHandler(WM_SOCKET,
				static_cast<WPARAM>(GetHandle()),
				Spry::Internal::MessageHandler(this, &AsyncStreamSocket::OnMessage));

			StreamSocket::Release();
		}

	private:
		AsyncStreamSocket(const AsyncStreamSocket&);
		AsyncStreamSocket& operator=(AsyncStreamSocket&);

	private:
		int		_dnsId;
		int		_port;
		
		bool	_canAccept;
		bool	_canReceive;
		bool	_canSend;
		bool	_shutdown;
	};
}
}
}

#endif