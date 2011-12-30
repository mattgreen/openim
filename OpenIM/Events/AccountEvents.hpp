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

#ifndef OPENIM_EVENTS_ACCOUNTEVENTS_HPP
#define OPENIM_EVENTS_ACCOUNTEVENTS_HPP

#include "../OpenIM.hpp"

#include "../Account.hpp"
#include "Event.hpp"


struct AccountConnectEvent :
	public Event<AccountConnectEvent>
{
	static const wchar_t* GetName() { return L"AccountConnectEvent"; }

	Account*	Account;
	int			Progress;
};


struct AccountCreateEvent :
	public Event<AccountCreateEvent>
{
	static const wchar_t* GetName() { return L"AccountCreateEvent"; }

	Account*	Account;
	int			Position;
};

struct AccountDestroyEvent :
	public Event<AccountDestroyEvent>
{
	static const wchar_t* GetName() { return L"AccountDestroyEvent"; }
	
	Account*	Account;
};

struct AccountDisconnectEvent :
	public Event<AccountDisconnectEvent>
{
	static const wchar_t* GetName() { return L"AccountDisconnectEvent"; }

	Account*	Account;
	bool		InitiatedByUser;
};

struct AccountErrorEvent :
	public Event<AccountErrorEvent>
{
	static const wchar_t* GetName() { return L"AccountErrorEvent"; }

	Account*		Account;
	const wchar_t*	Caption;
	const wchar_t*	Message;
};

struct AccountUpdateEvent :
	public Event<AccountUpdateEvent>
{
	static const wchar_t* GetName() { return L"AccountUpdateEvent"; }
		
	Account*	Account;
};

#endif