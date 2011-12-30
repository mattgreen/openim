//
// Copyright (C) OpenIM developers
//  
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software 
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

#include "ServerConnection.hpp"

#include "Spry.hpp"

#include "Channel.hpp"
#include "FLAP.hpp"
#include "State.hpp"
#include "TLV.hpp"
#include "TLVs.hpp"

ServerConnection::ServerConnection(AIMAccount* account) :
	_account(account),
	_list(account),
	_framer(_socket),
	_state(Offline),

	_genericHandler(&_list, _framer, _incoming, _outgoing),
	_locationHandler(&_list, _framer, _incoming, _outgoing),
	_authHandler(&_list, _framer, _incoming, _outgoing),
	_buddyListHandler(&_list, _framer, _incoming, _outgoing),
	_icbmHandler(&_list, _framer, _incoming, _outgoing),
	_privacyHandler(&_list, _framer, _incoming, _outgoing),
	_ssiHandler(&_list, _framer, _incoming, _outgoing),

	OnError(_ssiHandler.OnError),

	OnMessageReceive(_icbmHandler.OnMessageReceive),
	OnMessageSend(_icbmHandler.OnMessageSend),
	OnMessageSendError(_icbmHandler.OnMessageSendError),
	OnTypingUpdate(_icbmHandler.OnTypingUpdate)
{
	_handlers.push_back(&_genericHandler);
	_handlers.push_back(&_locationHandler);
	_handlers.push_back(&_authHandler);
	_handlers.push_back(&_buddyListHandler);
	_handlers.push_back(&_icbmHandler);
	_handlers.push_back(&_privacyHandler);
	_handlers.push_back(&_ssiHandler);

	_socket.OnConnect = AsyncSocketHandler(this, &ServerConnection::OnSocketConnect);
	_socket.OnDisconnect = AsyncSocketHandler(this, &ServerConnection::OnSocketDisconnect);
	_socket.OnReceive = AsyncSocketHandler(this, &ServerConnection::OnSocketReceive);
	_socket.OnSend = AsyncSocketHandler(this, &ServerConnection::OnSocketSend);		

#ifdef _DEBUG
	EnableTracing(true);
#endif

	Reset();
}

ServerConnection::~ServerConnection()
{
}

void ServerConnection::AddContact(const wchar_t* user, uint16 gid)
{
	if(GetConnectionStatus() != Online)
		return;

	_ssiHandler.AddContact(user, gid);
}

void ServerConnection::AddGroup(const wchar_t* group)
{
	_ssiHandler.AddGroup(group);
}

void ServerConnection::AliasContact(const wchar_t* alias, uint16 gid, uint16 iid)
{
	_ssiHandler.AliasContact(alias, gid, iid);
}

bool ServerConnection::CanGoOnline()
{
	foreach(SNACHandler* h, _handlers)
	{
		if(! h->IsSetup())
			return false;
	}

	return true;
}

void ServerConnection::Connect()
{
	if(_state == Offline)
	{
		_socket.Connect("login.oscar.aol.com", 5190);
		_state = AuthorizerConnect;
	}
}

void ServerConnection::Disconnect()
{
	_disconnecting = true;
	_socket.Disconnect();
}

void ServerConnection::EnableTracing(bool enabled)
{
	TracingEnabled = enabled;

	_framer.TracingEnabled = enabled;
	_genericHandler.TracingEnabled = enabled;
	_locationHandler.TracingEnabled = enabled;
	_authHandler.TracingEnabled = enabled;
	
}

void ServerConnection::FinishLogin()
{
	NetworkBinaryWriter writer(_outgoing);

	foreach(Service service, _genericHandler.KnownServices)
	{
		writer << service.ID;
		writer << service.Version;
		switch(service.ID)
		{
		case Generic::ServiceID:
			writer << static_cast<uint16>(0x0010);
			break;

		case Location::ServiceID:
		case BuddyList::ServiceID:
		case ICBM::ServiceID:
		case Invites::ServiceID:
		case Privacy::ServiceID:
		case SSI::ServiceID:
			writer << static_cast<uint16>(0x110);
			break;

		default:
			writer << static_cast<uint16>(0x0104);
			break; 
		}

		writer << service.ToolVersion;
	}

	_framer.SendSNAC(SNAC(Generic::ServiceID, Generic::ClientReady), _outgoing);
	_state = Online;
	
	if(OnConnect != 0)
		OnConnect(100);
}

AIMContactList* ServerConnection::GetContactList()
{
	return &_list;
}

const wchar_t* ServerConnection::GetNormalizedUserID()
{
	return _authHandler.NormalizedScreenName;
}

const wchar_t* ServerConnection::GetUserID()
{
	return _authHandler.ScreenName.c_str();
}

bool ServerConnection::IsAway()
{
	return _locationHandler.IsAway();
}

State ServerConnection::GetConnectionStatus()
{
	return _state;
}

void ServerConnection::Message(const wchar_t* recipient, const wchar_t* msg, bool autoresponse)
{
	_icbmHandler.Send(recipient, msg, autoresponse);
}

void ServerConnection::OnSocketConnect(const AsyncResult& result)
{
	if(! result.Success)
		return;
	
	switch(_state)
	{
	case AuthorizerConnect:
		_state = AuthorizerGreeting;
		break;

	case BOSConnect:
		_state = SendAuthorizationCookie;
		break;
	}
}

void ServerConnection::OnSocketDisconnect(const AsyncResult&)
{
	Trace("Disconnected\n");

	if(_state == AuthorizationSuccess || _state == Migrating)
	{
		_socket.Connect(_authHandler.BOSAddress, _authHandler.BOSPort);
		_state = BOSConnect;
	}
	else
	{
		_state = Offline;
		
		Reset();

		if(OnDisconnect != 0)
			OnDisconnect(! _disconnecting);
	}
}

void ServerConnection::OnSocketReceive(const AsyncResult&)
{
	try
	{
		Buffer<8192> buffer;
		int length = _socket.Receive(buffer, buffer.Capacity);
	
		_packetizer.Append(buffer, length);
		while(_packetizer.HasMorePackets())
		{
			length = _packetizer.GetNextPacket(buffer, buffer.Capacity);	

			if(TracingEnabled)
			{
				Trace("RECV: %i bytes\n", length);
				TraceHexDump(buffer, length);
			}

			_incoming.Clear();
			_incoming.Write(buffer, length);
			_incoming.Position = 0;

			_outgoing.Clear();

			NetworkBinaryReader reader(_incoming);

			try
			{
				FLAP header;
				reader >> header;

				switch(header.Channel)
				{
				case ConnectionNegotiationChannel:
					ProcessNegotiation(reader);
					break;

				case DataChannel:
					ProcessData(reader);
					break;

				case ErrorChannel:
					break;

				case ConnectionCloseNegotiationChannel:
					Disconnect();
					break;

				case KeepAliveChannel:
					break;
				}

			}
			catch(EndOfStreamException&)
			{
				Trace("\nERROR: parsing underflow when trying to parse the previous packet.\n");

				Disconnect();
			}
		}
	}
	catch(SocketException& e)
	{
		Trace(e.Description);
	}
}

void ServerConnection::OnSocketSend(const AsyncResult&)
{
	
}

void ServerConnection::ProcessData(NetworkBinaryReader& reader)
{
	SNAC snac;
	reader >> snac;

	Trace("SNAC: %0x:%0x\n", snac.Service, snac.Subtype);

	// Dispatch SNAC to appropriate handler.
	State newState = NoChange;
	switch(snac.Service)
	{
	case Generic::ServiceID:
		newState = _genericHandler.Process(snac);
		break;

	case Location::ServiceID:
		newState = _locationHandler.Process(snac);
		break;
	
	case BuddyList::ServiceID:
		newState = _buddyListHandler.Process(snac);
		break;

	case ICBM::ServiceID:
		newState = _icbmHandler.Process(snac);
		break;

	case Privacy::ServiceID:
		newState = _privacyHandler.Process(snac);
		break;

	case SSI::ServiceID:
		newState = _ssiHandler.Process(snac);
		break;

	case Authorization::ServiceID:
		newState = _authHandler.Process(snac);
		break;
	}
	
	// Record state change if present.
	if(newState != NoChange)
		_state = newState;

	switch(_state)
	{
	case AuthorizationFailure:
		OnError(L"Unable To Connect to AIM", _authHandler.ErrorMessage);
		break;

	case ServiceSetup:
		Setup();
		break;

	case ServiceSetupResponses:
		if(CanGoOnline())
			FinishLogin();

		break;
	}

}

void ServerConnection::ProcessNegotiation(NetworkBinaryReader&)
{
	const int FLAPVersion = 1;

	if(_state == AuthorizerGreeting)
	{
		if(OnConnect != 0)
			OnConnect(5);

		NetworkBinaryWriter writer(_outgoing);
		
		writer << FLAPVersion;
		_framer.Send(ConnectionNegotiationChannel, _outgoing);

		// Request an MD5 key for this screenname.
		TLV<char[16]> screenname(TLVs::ScreenName);
		screenname.Length = static_cast<unsigned short>(
			Encoding::ASCII::GetBytes(_authHandler.ScreenName, screenname.Value, DimensionOf(screenname.Value)));
		writer << screenname;

		_framer.SendSNAC(SNAC(Authorization::ServiceID, Authorization::MD5Key), _outgoing);

		_state = MD5Response;
	}
	else if(_state == SendAuthorizationCookie)
	{
		Trace("Sending authorization cookie.\n");

		NetworkBinaryWriter writer(_outgoing);

		writer << FLAPVersion;
		writer << _authHandler.Cookie;
		_framer.Send(ConnectionNegotiationChannel, _outgoing);

		_state = SupportedServices;
	}
}

void ServerConnection::RemoveContact(const wchar_t* contact, uint16 gid, uint16 bid)
{
	_ssiHandler.RemoveContact(contact, gid, bid);
}

void ServerConnection::RemoveGroup(const wchar_t* group, uint16 gid)
{
	_ssiHandler.RemoveGroup(group, gid);
}

void ServerConnection::RenameGroup(const wchar_t* name, uint16 gid)
{
	_ssiHandler.RenameGroup(name, gid);
}

void ServerConnection::Reset()
{
	_disconnecting = false;
	_packetizer.Clear();

	foreach(SNACHandler* handler, _handlers)
	{
		handler->Reset();
	}
}

void ServerConnection::SetAccount(AIMAccount* account)
{
	_ssiHandler.Account = account;
	_account = account;
}

void ServerConnection::SetIdleTime(uint32 seconds)
{
	_genericHandler.SetIdleTime(seconds);
}

void ServerConnection::SetProfile(const wchar_t* profile)
{
	_locationHandler.SetProfile(profile);
}

void ServerConnection::SetStatus(const wchar_t* status, const wchar_t* description)
{
	if(status == String(L"Away"))
		_locationHandler.SetAwayMessage(description);

}

void ServerConnection::Setup()
{
	foreach(SNACHandler* handler, _handlers)
	{
		handler->Setup();
	}

	_state = ServiceSetupResponses;
}

bool ServerConnection::SetUserID(const wchar_t* id)
{
	_authHandler.ScreenName.assign(id);

	return true;
}

void ServerConnection::SetPassword(const wchar_t* password)
{
	_authHandler.Password.assign(password);
}
