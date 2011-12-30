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

#ifndef OPENIM_INTERFACES_IACCOUNTMANAGER_HPP
#define OPENIM_INTERFACES_IACCOUNTMANAGER_HPP

#include "../OpenIM.hpp"

#include "../Account.hpp"
#include "../AccountFactory.hpp"
#include "../ComponentImpl.hpp"

class IAccountManager :
	public ComponentImpl<0x3>
{
public:
	virtual Account*		Create(const wchar_t* protocol) = 0;
	virtual void			Destroy(Account*) = 0;

	virtual const wchar_t*	GetLostPasswordURL(const wchar_t* protocol) = 0;
	virtual const wchar_t**	GetProtocols() = 0;
	virtual const wchar_t*	GetSignupURL(const wchar_t* protocol) = 0;

	virtual void			Register(AccountFactory*) = 0;
	virtual void			Unregister(AccountFactory*) = 0;
	
	virtual void			Show() = 0;
};

#endif