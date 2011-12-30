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

#ifndef OPENIM_ACCOUNTFACTORY_HPP
#define OPENIM_ACCOUNTFACTORY_HPP

#include "OpenIM.hpp"

class Account;

// Base class used to create protocol-specific Account instances.
//
class OPENIM_API AccountFactory
{
public:
	virtual ~AccountFactory() = 0;

	/// Creates a new instance of a specific type of Account.
	virtual Account*		Create() = 0;

	/// Destroys an instance of a specific type of Account.
	virtual void			Destroy(Account* account) = 0;
	
	/// Returns the URL used for lost password recovery. If this capability is
	/// not supported, then the null pointer is returned.
	virtual const wchar_t*	GetLostPasswordURL() = 0;

	/// Returns the human-readable name of this protocol (such as "AIM").
	virtual const wchar_t*	GetProtocolName() = 0;

	/// Returns the URL used for creating new accounts. If this capability is
	/// not supported, then the null pointer is returned.
	virtual const wchar_t*	GetSignupURL() = 0;
};


#endif