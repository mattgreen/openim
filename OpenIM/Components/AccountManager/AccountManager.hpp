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

#ifndef OPENIM_COMPONENTS_ACCOUNTMANAGER_HPP
#define OPENIM_COMPONENTS_ACCOUNTMANAGER_HPP

#include "../../OpenIM.hpp"

#include "../../Interfaces/IAccountManager.hpp"
#include "../../Events/SystemEvents.hpp"
#include "../../String.hpp"

#pragma warning(push)
#pragma warning(disable:4702)
#include <map>
#include <vector>
#pragma warning(pop)

class AccountManager :
	public IAccountManager
{
public:
	AccountManager();
	~AccountManager();

public:
	virtual Account*		Create(const wchar_t* protocol);
	virtual void			Destroy(Account*);
	
	virtual const wchar_t*	GetLostPasswordURL(const wchar_t* protocol);
	virtual const wchar_t**	GetProtocols();
	virtual const wchar_t*	GetSignupURL(const wchar_t* protocol);

	virtual void			Register(AccountFactory*);
	virtual void			Unregister(AccountFactory*);
	
	virtual void			Show();

private:
	void					OnLoadUserProfile(LoadUserProfileEvent&);
	void					OnUnloadUserProfile(UnloadUserProfileEvent&);

private:
	void					ReadAccounts(const wchar_t* dir);
	void					WriteAccounts(const wchar_t* dir);

private:
	typedef std::map<Account*, String>		Passwords;
	typedef std::vector<AccountFactory*>	Factories;
	typedef std::vector<Account*>			Accounts;
	typedef std::vector<const wchar_t*>		Protocols;

	enum Tags
	{
		AccountBegin = 0,
		AccountEnd = 1,
		UserID = 2,
		Password = 3,
		Autoconnect = 4,
		DisplayName = 5,
		Label = 6,
		Reconnect = 7,
		ReportIdleTime = 8,
		SendTypingNotifications = 9
	};

	EventBindings	_bindings;

	Factories		_factories;
	Accounts		_accounts;
	Passwords		_passwords;
	Protocols		_protocols;
	bool			_visible;
};

#endif