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

#ifndef OPENIM_ACCOUNT_HPP
#define OPENIM_ACCOUNT_HPP

#include "OpenIM.hpp"

#include "AccountInfo.hpp"
#include "AccountStatus.hpp"
#include "ContactList.hpp"

class OPENIM_API Account
{
public:
	virtual	~Account() = 0;
	
public:
	/// Begin connecting to the account using the previously supplied credentials.
	virtual void				Connect() = 0;

	/// Begin disconnecting from a connected account.
	virtual void				Disconnect() = 0;

	virtual const AccountInfo*	GetAccountInfo() = 0;
	virtual ContactList*		GetContactList() = 0;
	virtual const wchar_t*		GetDisplayName() = 0;
	virtual const wchar_t*		GetLabel() = 0;
	virtual const wchar_t*		GetProtocolName() = 0;
	virtual const wchar_t*		GetUserID() = 0;

	virtual AccountStatus		GetConnectionStatus() = 0;

	virtual void				SetDisplayName(const wchar_t* displayName) = 0;
	virtual void				SetLabel(const wchar_t* label) = 0;
	virtual void				SetPassword(const wchar_t* password) = 0;
	virtual bool				SetUserID(const wchar_t* userID) = 0;
	
};

#endif