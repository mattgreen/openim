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

#ifndef AIM_SERVERCONNECTION_HPP
#define AIM_SERVERCONNECTION_HPP

#include <set>

#include "Spry.hpp"

#include "../../OpenIM/String.hpp"

#include "../../Utilities/Traceable.hpp"

#include "Framer.hpp"
#include "Packetizer.hpp"

#include "Handlers/AuthorizationSNACHandler.hpp"
#include "Handlers/BuddyListSNACHandler.hpp"
#include "Handlers/GenericSNACHandler.hpp"
#include "Handlers/ICBMSNACHandler.hpp"
#include "Handlers/LocationSNACHandler.hpp"
#include "Handlers/PrivacySNACHandler.hpp"
#include "Handlers/SSISNACHandler.hpp"

typedef Spry::Delegate<void (int)>	ConnectHandler;
typedef Spry::Delegate<void (bool)>	DisconnectHandler;

class AIMAccount;

class ServerConnection :
	public Traceable
{
public:
	ServerConnection(AIMAccount* account);
	~ServerConnection();

public:
	void						AddContact(const wchar_t* user, uint16 gid);
	void						AddGroup(const wchar_t* group);
	void						AliasContact(const wchar_t* alias, uint16 gid, uint16 bid);

	void						Connect();
	void						Disconnect();

	bool						IsAway();

	AIMContactList*				GetContactList();
	State						GetConnectionStatus();
	const wchar_t*				GetNormalizedUserID();
	const wchar_t*				GetUserID();
	
	void						Message(const wchar_t* recipient, const wchar_t* msg, bool autoresponse);

	void						RenameGroup(const wchar_t* name, uint16 gid);

	void						RemoveContact(const wchar_t* contact, uint16 gid, uint16 bid);
	void						RemoveGroup(const wchar_t* group, uint16 gid);

	void						SetAccount(AIMAccount* account);
	void						SetIdleTime(uint32 seconds);
	void						SetPassword(const wchar_t* password);
	void						SetProfile(const wchar_t* profile);
	void						SetStatus(const wchar_t* status, const wchar_t* description);
	bool						SetUserID(const wchar_t* id);

public:
	void						EnableTracing(bool enabled);

private:
	bool						CanGoOnline();
	void						Reset();
	void						Setup();

private:
	void						OnSocketConnect(const AsyncResult&);
	void						OnSocketDisconnect(const AsyncResult&);
	void						OnSocketReceive(const AsyncResult&);
	void						OnSocketSend(const AsyncResult&);

private:
	void						FinishLogin();
	void						ProcessData(NetworkBinaryReader& reader);
	void						ProcessNegotiation(NetworkBinaryReader& reader);


private:
	typedef std::set<uint16>			Families;
	typedef std::vector<SNACHandler*>	SNACHandlers;
	
	AIMAccount*					_account;
	AIMContactList				_list;
	AsyncStreamSocket			_socket;
	Packetizer					_packetizer;
	Framer						_framer;
	State						_state;
	bool						_disconnecting;
	
	MemoryStream				_incoming;
	MemoryStream				_outgoing;

	SNACHandlers				_handlers;
	GenericSNACHandler			_genericHandler;
	LocationSNACHandler			_locationHandler;
	AuthorizationSNACHandler	_authHandler;
	BuddyListSNACHandler		_buddyListHandler;
	ICBMSNACHandler				_icbmHandler;
	PrivacySNACHandler			_privacyHandler;
	SSISNACHandler				_ssiHandler;

public:
	ConnectHandler				OnConnect;
	DisconnectHandler			OnDisconnect;
	ErrorHandler&				OnError;
	
	MessageHandler&				OnMessageReceive;
	MessageHandler&				OnMessageSend;
	MessageErrorHandler&		OnMessageSendError;
	TypingHandler&				OnTypingUpdate;
};


#endif