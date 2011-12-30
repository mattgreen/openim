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

#ifndef AIM_AIMACCOUNT_HPP
#define AIM_AIMACCOUNT_HPP

#include "../../OpenIM/AccountImpl.hpp"

#include "AIMContactList.hpp"
#include "ServerConnection.hpp"
#include "Spry.hpp"

class AIMAccount :
	public AccountImpl
{
public:
	AIMAccount();
	~AIMAccount();

// Account interface
public:
	virtual void			Connect();
	virtual void			Disconnect();
	virtual AccountStatus	GetConnectionStatus();
	virtual ContactList*	GetContactList();
	virtual const wchar_t*	GetDisplayName();
	virtual const wchar_t*	GetLabel();
	virtual const wchar_t*	GetProtocolName();
	virtual const wchar_t*	GetUserID();
	virtual void			SetDisplayName(const wchar_t* displayName);
	virtual void			SetLabel(const wchar_t* label);
	virtual void			SetPassword(const wchar_t*);
	virtual bool			SetUserID(const wchar_t* userid);

public:
	void					AddContact(const wchar_t* name, uint16 gid);
	void					AddGroup(const wchar_t* name);
	void					AliasContact(const wchar_t* alias, uint16 gid, uint16 iid);
	void					RenameGroup(const wchar_t* name, uint16 gid);
	void					RemoveContact(const wchar_t* name, uint16 gid, uint16 iid);
	void					RemoveGroup(const wchar_t* name, uint16 gid);

// Event handlers
public:
	void					OnConnect(int progress);
	void					OnDisconnect(bool initiated);
	void					OnError(const wchar_t* caption, const wchar_t* message);

private:
	AIMAccount(const AIMAccount&);
	AIMAccount& operator=(const AIMAccount&);

private:
	ServerConnection		_connection;
	String					_label;
	String					_displayName;
};


#endif