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

#ifndef OPENIM_INTERFACES_IRESOURCEMANAGER_HPP
#define OPENIM_INTERFACES_IRESOURCEMANAGER_HPP

#include "../OpenIM.hpp"

#include "../Account.hpp"
#include "../Contact.hpp"
#include "../Group.hpp"

#include "../ComponentImpl.hpp"

#include "../Win32Headers.hpp"

class IResourceManager :
	public ComponentImpl<0x4>
{
public:
	virtual int			GetGroupImageIndex() = 0;
	virtual int			GetImageIndex(const wchar_t* protocol) = 0;
	virtual int			GetImageIndex(Account* account) = 0;
	virtual int			GetImageIndex(Contact* contact) = 0;
	virtual int			GetImageIndex(Group* group) = 0;
	virtual HIMAGELIST	GetImageList() = 0;
};

#endif