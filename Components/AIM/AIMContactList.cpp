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

#include "AIMContactList.hpp"

#include "AIMAccount.hpp"
#include "AIMContact.hpp"
#include "AIMGroup.hpp"
#include "Spry.hpp"

struct ItemSorter
{
	template<typename T>
	bool operator()(T* l, T* r) const
	{
		return l->Position < r->Position;
	}
};

inline bool ScreenNameEquals(const wchar_t* s1, const wchar_t* s2)
{
	Assert(s1 != 0);
	Assert(s2 != 0);

	while(*s1 != 0 && *s2 != 0)
	{
		// Consume whitespace.
		while(*s1 == L' ')
			s1++;
		while(*s2 == L' ')
			s2++;

		// Check lowercased char.
		if(tolower(*s1) != tolower(*s2))
			return false;

		// Advance if not at end of string.
		if(*s1 != 0)
			s1++;
		if(*s2 != 0)
			s2++;
	}
	
	return true;
}

AIMContactList::AIMContactList(AIMAccount* account) :
	_account(account)
{
}

AIMContactList::~AIMContactList()
{
	Clear(false);
}

void AIMContactList::AddContact(const wchar_t* id, Group* group)
{
	AIMGroup* aimGroup = dynamic_cast<AIMGroup*>(group);
	if(aimGroup == 0)
		return;

	_account->AddContact(id, aimGroup->GroupID);
}

void AIMContactList::AddGroup(const wchar_t* name, Group*)
{
	_account->AddGroup(name);
}

void AIMContactList::Clear(bool notify)
{
	foreach(AIMGroup* group, Groups)
	{
		foreach(AIMContact* contact, group->Contacts)
		{
			if(notify)
				contact->OnDestroy();

			delete contact;
		}

		group->Contacts.clear();

		if(notify)
			group->OnDestroy();

		delete group;
	}

	Groups.clear();
}

AIMContact* AIMContactList::GetContact(uint16 iid, uint16 gid)
{
	foreach(AIMGroup* group, Groups)
	{
		foreach(AIMContact* contact, group->Contacts)
		{
			if(contact->GroupID == gid && contact->ItemID == iid)
				return contact;

		}
	}

	return 0;
}

Contact* AIMContactList::GetContactByID(const wchar_t* id)
{
	Assert(id != 0);

	foreach(AIMGroup* group, Groups)
	{
		foreach(AIMContact* contact, group->Contacts)
		{
			if(ScreenNameEquals(contact->GetUserID(), id))
				return contact;

		}
	}

	return 0;
}

AIMGroup* AIMContactList::GetGroup(uint16 gid)
{
	foreach(AIMGroup* group, Groups)
	{
		if(group->GroupID == gid)
			return group;
	}

	return 0;
}

int AIMContactList::GetGroupCount() const
{
	return static_cast<int>(Groups.size());
}


Group* AIMContactList::GetGroupByID(const wchar_t* id)
{
	Assert(0);

	foreach(AIMGroup* group, Groups)
	{
		if(wcscmp(group->GetName(), id) == 0)
			return group;
	}

	return 0;
}

uint16 AIMContactList::GetUniqueBuddyID()
{
	uint16 bid = 0;
	bool duplicate = false;
	do
	{
		duplicate = false;
		bid = static_cast<uint16>(rand() % 0xffff);
		foreach(AIMGroup* group, Groups)
		{
			foreach(AIMContact* contact, group->Contacts)
			{
				if(contact->ItemID == bid)
				{
					duplicate = true;
					break;
				}
			}
		}
	}
	while(duplicate);

	return bid;
}

uint16 AIMContactList::GetUniqueGroupID()
{
	uint16 gid = 0;
	bool duplicate = false;
	do
	{
		duplicate = false;
		gid = static_cast<uint16>(rand() % 65535);
		foreach(AIMGroup* group, Groups)
		{
			if(group->GroupID == gid)
			{
				duplicate = true;
				break;
			}
		}
	}
	while(duplicate);

	return gid;
}

void AIMContactList::MoveContact(Contact* contact, Group* source, Group* destination, int position)
{
}

void AIMContactList::MoveGroup(Group* group, Group*, Group*, int position)
{
	AIMGroup* aimGroup = dynamic_cast<AIMGroup*>(group);
	if(aimGroup == 0)
		return;

}

void AIMContactList::OnContactAdd(const wchar_t* name, const wchar_t* alias, uint16 iid, uint16 gid, int position)
{
	AIMGroup* group = GetGroup(gid);
	Assert(group != 0);

	if(position == -1)
		position = static_cast<int>(group->Contacts.size());

	AIMContact* existingContact = static_cast<AIMContact*>(GetContactByID(name));

	AIMContact* contact = new AIMContact(_account, group, name, iid, gid, position);
	group->Contacts.push_back(contact);
	Sort<ItemSorter>(group->Contacts);

	if(alias != 0 && *alias != 0)
		contact->Alias = alias;

	if(existingContact != 0)
	{
		contact->IdleStart = existingContact->IdleStart;
		contact->Status = existingContact->Status;
		contact->ScreenName = existingContact->ScreenName;
	}

	contact->OnCreate();

	if(contact->IsConnected())
		contact->OnSignOn();
}

void AIMContactList::OnContactAlias(const wchar_t* alias, uint16 iid, uint16 gid)
{
	AIMContact* contact = GetContact(iid, gid);
	Assert(contact != 0);

	if(alias != 0)
		contact->Alias = alias;
	else
		contact->Alias.clear();

	contact->OnUpdate();
}

void AIMContactList::OnContactRemove(const wchar_t*, uint16 iid, uint16 gid)
{
	AIMGroup* group = GetGroup(gid);
	if(group == 0)
		return;

	for(AIMGroup::AIMContacts::iterator i = group->Contacts.begin(); i != group->Contacts.end(); ++i)
	{
		if((*i)->ItemID == iid && (*i)->GroupID == gid)
		{
			(*i)->OnDestroy();

			delete *i;
			group->Contacts.erase(i);
			break;
		}
	}
}

void AIMContactList::OnContactSignOff(const UserInfo& info)
{
	foreach(AIMGroup* group, Groups)
	{
		foreach(AIMContact* contact, group->Contacts)
		{
			if(ScreenNameEquals(contact->GetUserID(), info.ScreenName))
			{
				contact->OnSignOff();
			}
		}
	}
}

void AIMContactList::OnContactSignOn(const UserInfo& info)
{
	foreach(AIMGroup* group, Groups)
	{
		foreach(AIMContact* contact, group->Contacts)
		{
			if(ScreenNameEquals(contact->GetUserID(), info.ScreenName))
			{
				bool justConnected = ! contact->IsConnected();

				// Not possible to tell if they are invisible yet from presence notifications.
				if(! info.IsAway())
					contact->Status = StatusAvailable;
				else
					contact->Status = StatusAway;

				contact->Update(info);

				// If they were offline before, then we need to generate a signon event for them.
				if(justConnected)
					contact->OnSignOn();
				else
					contact->OnUpdate();
			}
		}
	}
}

void AIMContactList::OnGroupAdd(const wchar_t* name, uint16 gid, int position)
{
	if(position == -1)
		position = GetGroupCount();

	AIMGroup* newGroup = new AIMGroup(_account, name, gid, position);
	
	Groups.push_back(newGroup);
	Sort<ItemSorter>(Groups);
	newGroup->OnCreate();
}

void AIMContactList::OnGroupRemove(const wchar_t*, uint16 gid)
{
	// Find the group the user wants removed.
	AIMGroup* group = GetGroup(gid);
	Assert(group != 0);

	// First, remove all contacts contained within the group being removed. 
	// We need to copy the list of contacts out to avoid invalid iterators.
	AIMGroup::AIMContacts contacts;
	foreach(AIMContact* contact, group->Contacts)
		contacts.push_back(contact);

	foreach(AIMContact* contact, contacts)
		OnContactRemove(contact->GetUserID(), contact->ItemID, contact->GroupID);

	// Destroy the group now.
	group->OnDestroy();
	for(AIMGroups::iterator i = Groups.begin(); i != Groups.end(); ++i)
	{
		if((*i)->GroupID == gid)
		{
			Groups.erase(i);
			break;
		}
	}
	delete group;
}

void AIMContactList::OnGroupRename(const wchar_t* newName, uint16 gid)
{
	AIMGroup* group = GetGroup(gid);
	Assert(group != 0);

	group->Name = newName;
	group->OnUpdate();
}

void AIMContactList::OnGroupUpdate(const wchar_t*, uint16 gid, int position)
{
	AIMGroup* group = GetGroup(gid);
	if(group == 0)
		return;

	group->Position = position;
	Sort<ItemSorter>(Groups);
		
	group->OnUpdate();
}

void AIMContactList::RemoveContact(Contact* contact, Group* group)
{
	AIMGroup* aimGroup = dynamic_cast<AIMGroup*>(group);
	if(aimGroup == 0)
		return;

	AIMContact* aimContact = dynamic_cast<AIMContact*>(contact);
	if(aimContact == 0)
		return;

	_account->RemoveContact(aimContact->GetUserID(), aimGroup->GroupID, aimContact->ItemID);
}

void AIMContactList::RemoveGroup(Group* group, Group*)
{
	AIMGroup* aimGroup = dynamic_cast<AIMGroup*>(group);
	if(group == 0)
		return;

	_account->RemoveGroup(aimGroup->GetName(), aimGroup->GroupID);
}
