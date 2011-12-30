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

#ifndef OPENIM_CONTACTLIST_HPP
#define OPENIM_CONTACTLIST_HPP

#include "OpenIM.hpp"

class Contact;
class Group;

class OPENIM_API ContactList
{
public:
	virtual ~ContactList() = 0;

	virtual void		AddContact(const wchar_t* id, Group* group) = 0;

	virtual void		AddGroup(const wchar_t* name, Group* group) = 0;

	// Returns a Contact with the given user ID, or null if none is found.
	virtual Contact*	GetContactByID(const wchar_t* id) = 0;

	// Returns a Group with the given ID, or null if none is found.
	virtual Group*		GetGroupByID(const wchar_t* id) = 0;

	virtual	void		RemoveContact(Contact* contact, Group* group) = 0;

	virtual void		RemoveGroup(Group* group, Group* parentGroup) = 0;
};


#endif