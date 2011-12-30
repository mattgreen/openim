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

#include "AIMAccount.hpp"

#include "../../OpenIM/Events/AccountEvents.hpp"
#include "../../OpenIM/Events/ContactEvents.hpp"

#include "AIMContact.hpp"
#include "AIMGroup.hpp"

AIMAccount::AIMAccount() :
	AccountImpl(0, 0),
	_connection(this)
{
	_connection.OnConnect = ConnectHandler(this, &AIMAccount::OnConnect);
	_connection.OnDisconnect = DisconnectHandler(this, &AIMAccount::OnDisconnect);
	_connection.OnError = ErrorHandler(this, &AIMAccount::OnError);

#ifdef _DEBUG
	_connection.EnableTracing(true);
#endif
}

AIMAccount::~AIMAccount()
{
}

void AIMAccount::AddContact(const wchar_t* name, uint16 gid)
{
	_connection.AddContact(name, gid);
}

void AIMAccount::AddGroup(const wchar_t* name)
{
	_connection.AddGroup(name);
}

void AIMAccount::AliasContact(const wchar_t* alias, uint16 gid, uint16 iid)
{
	_connection.AliasContact(alias, gid, iid);
}

void AIMAccount::Connect()
{
	_connection.Connect();
}

void AIMAccount::Disconnect()
{
	_connection.Disconnect();
}

AccountStatus AIMAccount::GetConnectionStatus()
{
	switch(_connection.GetConnectionStatus())
	{
	case AuthorizerConnect:
	case AuthorizerGreeting:
	case MD5Response:
	case AuthorizerResponse:
	case AuthorizationSuccess:
	case AuthorizationFailure:
	case BOSConnect:
	case SendAuthorizationCookie:
	case SupportedServices:
	case ServiceVersionNumbers:
	case RateLimit:
	case ServiceSetup:
	case ServiceSetupResponses:
		return Connecting;
		break;
		
	case Online:
	case Migrating:
		return Connected;
		break;

	case Offline:
		return Disconnected;
		break;

	default:
		Fail("Unknown connection state.");
		return Disconnected;
		break;
	}
}

ContactList* AIMAccount::GetContactList()
{
	return _connection.GetContactList();
}

const wchar_t* AIMAccount::GetDisplayName()
{
	if(_displayName.empty())
		return 0;
	else
        return _displayName.c_str();
}


const wchar_t* AIMAccount::GetLabel()
{
	if(_label.empty())
		return 0;
	else
        return _label.c_str();
}

const wchar_t* AIMAccount::GetProtocolName()
{
	return L"AIM";
}

const wchar_t* AIMAccount::GetUserID()
{
	return _connection.GetUserID();
}

void AIMAccount::OnConnect(int progress)
{
	AccountConnectEvent e;
	e.Account = this;
	e.Progress = progress;
	e.Invoke();
}

void AIMAccount::OnDisconnect(bool initiated)
{
	AccountDisconnectEvent e;
	e.Account = this;
	e.InitiatedByUser = initiated;
	e.Invoke();
}


void AIMAccount::OnError(const wchar_t* caption, const wchar_t* message)
{
	AccountErrorEvent e;
	e.Account = this;
	e.Caption = caption;
	e.Message = message;
	e.Invoke();
}

void AIMAccount::RenameGroup(const wchar_t* name, uint16 gid)
{
	_connection.RenameGroup(name, gid);
}

void AIMAccount::RemoveContact(const wchar_t* name, uint16 gid, uint16 iid)
{
	_connection.RemoveContact(name, gid, iid);
}

void AIMAccount::RemoveGroup(const wchar_t* name, uint16 gid)
{
	_connection.RemoveGroup(name, gid);
}

void AIMAccount::SetDisplayName(const wchar_t* displayName)
{
	if(displayName != 0)
		_displayName = displayName;
	else
		_displayName.clear();
}

void AIMAccount::SetLabel(const wchar_t* label)
{
	if(label != 0)
		_label = label;
	else
		_label.clear();
}

void AIMAccount::SetPassword(const wchar_t* password)
{
	_connection.SetPassword(password);
}

bool AIMAccount::SetUserID(const wchar_t* userid)
{
	return _connection.SetUserID(userid);
}