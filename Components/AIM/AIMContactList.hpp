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

#ifndef AIM_AIMCONTACTLIST_HPP
#define AIM_AIMCONTACTLIST_HPP

#pragma warning(push)
#pragma warning(disable:4702)
#include <vector>
#pragma warning(pop)

#include "../../OpenIM/ContactList.hpp"
#include "../../Utilities/Types.hpp"

#include "UserInfo.hpp"

class AIMAccount;
class AIMContact;
class AIMGroup;
typedef std::vector<AIMGroup*>	AIMGroups;

class AIMContactList :
	public ContactList
{
public:
	AIMContactList(AIMAccount* account);
	~AIMContactList();

public:
	virtual void		AddContact(const wchar_t* id, Group* group);
	virtual void		AddGroup(const wchar_t* name, Group* group);

	virtual Contact*	GetContactByID(const wchar_t* id);
	virtual Group*		GetGroupByID(const wchar_t* id);
	
	virtual void		MoveContact(Contact* contact, Group* source, Group* destination, int position);
	virtual void		MoveGroup(Group* group, Group* source, Group* destination, int position);

	virtual	void		RemoveContact(Contact* contact, Group* group);
	virtual void		RemoveGroup(Group* group, Group* parentGroup);

public:
	void				Clear(bool notify = true);
	AIMContact*			GetContact(uint16 iid, uint16 gid);
	int					GetGroupCount() const;
	AIMGroup*			GetGroup(uint16 gid);
	AIMGroups&			GetList();
	uint16				GetUniqueBuddyID();
	uint16				GetUniqueGroupID();
	
public:
	void				OnAccountDisconnect();

	void				OnContactAdd(const wchar_t* name, const wchar_t* alias, uint16 iid, uint16 gid, int position);
	void				OnContactAlias(const wchar_t* alias, uint16 iid, uint16 gid);
	void				OnContactMove(const wchar_t* name, uint16 iid, uint16 gid, int position);
	void				OnContactRemove(const wchar_t* name, uint16 iid, uint16 gid);
	void				OnContactSignOn(const UserInfo& info);
	void				OnContactSignOff(const UserInfo& info);

	void				OnGroupAdd(const wchar_t* name, uint16 gid, int position);
	void				OnGroupRemove(const wchar_t* name, uint16 gid);
	void				OnGroupRename(const wchar_t* newName, uint16 gid);
	void				OnGroupUpdate(const wchar_t* name, uint16 gid, int position);

public:
	AIMGroups			Groups;

private:
	AIMAccount*			_account;

};


#endif