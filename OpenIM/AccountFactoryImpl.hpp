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

#ifndef OPENIM_ACCOUNTFACTORYIMPL_HPP
#define OPENIM_ACCOUNTFACTORYIMPL_HPP

#include "OpenIM.hpp"

#include "Account.hpp"
#include "AccountFactory.hpp"

template<typename AccountType>
class AccountFactoryImpl :
	public AccountFactory
{
public:
	AccountFactoryImpl(const wchar_t* name, const wchar_t* lostPassURL = 0, const wchar_t* signupURL = 0) :
		_name(name),
		_lostPasswordURL(lostPassURL),
		_signupURL(signupURL)
	{
		Assert(name != 0);
	}

	virtual ~AccountFactoryImpl()
	{
	}

	virtual AccountType* Create()
	{
		return new AccountType();
	}

	virtual void Destroy(Account* account)
	{
		delete account;
	}

	virtual const wchar_t* GetLostPasswordURL()
	{
		return _lostPasswordURL;
	}

	virtual const wchar_t* GetProtocolName()
	{
		return _name;
	}

	virtual const wchar_t* GetSignupURL()
	{
		return _signupURL;
	}

private:
	const wchar_t*	_name;
	const wchar_t*	_lostPasswordURL;
	const wchar_t*	_signupURL;
};


#endif