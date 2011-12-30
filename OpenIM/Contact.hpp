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

#ifndef OPENIM_CONTACT_HPP
#define OPENIM_CONTACT_HPP

#include "OpenIM.hpp"

#include "Account.hpp"

class OPENIM_API Contact
{
public:
	virtual ~Contact() = 0;

	virtual Account*		GetAccount() = 0;
	virtual const wchar_t*	GetAlias() = 0;
	virtual const wchar_t*	GetDisplayName() = 0;
	virtual int				GetIdleTime() = 0;
	virtual const wchar_t*	GetStatus() = 0;
	virtual const wchar_t*	GetUserID() = 0;
	virtual bool			IsConnected() = 0;
	virtual bool			SetAlias(const wchar_t* alias) = 0;
};


#endif