#ifndef SPRY_IO_NETWORKING_INTERNAL_ASYNCDNSRESOLVERIMPL_HPP
#define SPRY_IO_NETWORKING_INTERNAL_ASYNCDNSRESOLVERIMPL_HPP

#include "../../../OSHeaders.hpp"
#include "../../../Collections/ArrayList.hpp"
#include "../DNS.hpp"

#include "AsyncDNSResolver.hpp"
#include "WinsockInitializer.hpp"
#include "../../../Internal/MessageReactor.hpp"

#define WM_DNS (WM_USER + 33)

#pragma comment(lib, "wsock32.lib")

namespace Spry
{
namespace IO
{
namespace Networking
{
namespace Internal
{
	using Spry::Collections::ArrayList;
	using Spry::Internal::MessageHandler;
	using Spry::Internal::MessageReactor;
	using Spry::IO::Networking::AsyncDNSResponse;
	using Spry::IO::Networking::AsyncDNSResponseHandler;

	class AsyncDNSResolverImpl :
		public AsyncDNSResolver
	{
	public:
		AsyncDNSResolverImpl() :
			_nextID(),
			_active(false)
		{
			if(MessageReactor* instance = MessageReactor::GetInstance())
				instance->AddMessageHandler(WM_DNS, MessageHandler(this, &AsyncDNSResolverImpl::OnMessage));
		}

		~AsyncDNSResolverImpl()
		{
			if(! _lookups.IsEmpty())
			{
				Lookup lookup = _lookups[0];
				Cancel(lookup.ID);

				_lookups.Clear();
			}

			if(MessageReactor* instance = MessageReactor::GetInstance())
				instance->RemoveMessageHandler(WM_DNS, MessageHandler(this, &AsyncDNSResolverImpl::OnMessage));
		}

		// Cancels an asynchronous DNS resolution request. 
		// If it has already completed, the delegate will not be called.
		void Cancel(int token)
		{
			for(ArrayList<Lookup>::Iterator i = _lookups.Begin; i != _lookups.End; ++i)
			{
				if(i->ID == token)
				{
					if(i->Task != 0)
						WSACancelAsyncRequest(i->Task);

					_lookups.RemoveAt(i);
					
					break;
				}
			}
		}

		void OnMessage(WPARAM task, LPARAM lParam)
		{
			// If no lookups are pending, then don't do anything.
			if(_lookups.IsEmpty())
				return;
			
			// If the task handle doesn't match up, then we don't know what we're looking at.
			Lookup activeLookup = _lookups[0];
			if(activeLookup.Task != reinterpret_cast<HANDLE>(task))
				return;

			// Remove the task handle, and change status to inactive until we start another request.
			_active = false;
			_lookups.RemoveAt(0);

			int errorCode = WSAGETASYNCERROR(lParam);

			AsyncDNSResponse response;
			response.ID = activeLookup.ID;
			response.Success = (errorCode == 0);
		
			if(response.Success)
			{
				hostent* addresses = reinterpret_cast<hostent*>(_buffer);
				unsigned int address = 0;
				memcpy(&address, addresses->h_addr_list[0], addresses->h_length);

				response.Address = address;
			}

			// Call delegate.
			if(activeLookup.Handler != 0)
				activeLookup.Handler(response);

			ResolveNext();
		}

		// Begins name resolution of the specified host. When resolution has completed, the delegate is invoked.
		// Returns a token that can be used to later cancel the operation.
		int Resolve(const char* host, AsyncDNSResponseHandler handler)
		{
			Lookup lookup;
			lookup.Handler = handler;

			if(host == 0)
				throw DNSException();

			int len = lstrlenA(host) + 1;
			if(len > 255)
				throw DNSException();

			lstrcpynA(lookup.Hostname, host, len);
			lookup.Handler = handler;
			lookup.ID = _nextID++;
			lookup.Task = 0;

			_lookups.Add(lookup);

			if(! _active)
				ResolveNext();

			return lookup.ID;
		}

	private:
		// Dispatches the next item in the queue to be resolved.
		void ResolveNext()
		{
			if(_lookups.IsEmpty())
				return;
			
			if(MessageReactor* instance = MessageReactor::GetInstance())
			{
				Lookup& nextLookup = _lookups[0];
				nextLookup.Task = WSAAsyncGetHostByName(instance->GetHWND(),
					WM_DNS, 
					nextLookup.Hostname,
					_buffer,
					MAXGETHOSTSTRUCT);
				
				if(nextLookup.Task == 0)
					_lookups.RemoveAt(0);
				else
					_active = true;
			}
		}

	private:
		AsyncDNSResolverImpl& operator=(const AsyncDNSResolverImpl&);

	private:
		struct Lookup
		{
			Lookup()
			{
			}

			AsyncDNSResponseHandler	Handler;
			HANDLE					Task;
			int						ID;
			char					Hostname[256];
		};

		ArrayList<Lookup>	_lookups;
		char				_buffer[MAXGETHOSTSTRUCT];
		int					_nextID;
		bool				_active;
	};
}
}
}
}

#undef WM_DNS

#endif