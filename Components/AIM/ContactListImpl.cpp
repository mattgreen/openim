#include "ContactListImpl.hpp"

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

ContactListImpl::~ContactListImpl()
{
	Clear(false);
}

void ContactListImpl::Clear(bool notify)
{
	foreach(AIMGroup* group, _groups)
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

	_groups.clear();
}

AIMContact* ContactListImpl::GetContact(uint16 iid, uint16 gid)
{
	foreach(AIMGroup* group, _groups)
	{
		foreach(AIMContact* contact, group->Contacts)
		{
			if(contact->ItemID == iid && contact->GroupID == gid)
				return contact;
		}
	}

	return 0;
}

AIMContact* ContactListImpl::GetContactByID(const wchar_t* id)
{
	Assert(id != 0);

	foreach(AIMGroup* group, _groups)
	{
		foreach(AIMContact* contact, group->Contacts)
		{
			if(ScreenNameEquals(contact->GetUserID(), id))
				return contact;

		}
	}

	return 0;
}

AIMGroup* ContactListImpl::GetGroup(uint16 gid)
{
	foreach(AIMGroup* group, _groups)
	{
		if(group->GroupID == gid)
			return group;
	}

	return 0;
}

int ContactListImpl::GetGroupCount() const
{
	return static_cast<int>(_groups.size());
}

AIMGroups& ContactListImpl::GetList()
{
	return _groups;
}

uint16 ContactListImpl::GetUniqueBuddyID()
{
	uint16 bid = 0;
	bool duplicate = false;
	do
	{
		duplicate = false;
		bid = static_cast<uint16>(rand() % 0xffff);
		foreach(AIMGroup* group, _groups)
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

uint16 ContactListImpl::GetUniqueGroupID()
{
	uint16 gid = 0;
	bool duplicate = false;
	do
	{
		duplicate = false;
		gid = static_cast<uint16>(rand() % 65535);
		foreach(AIMGroup* group, _groups)
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

void ContactListImpl::OnAccountDisconnect(AIMAccount*)
{
	Clear();
}

void ContactListImpl::OnContactAdd(AIMAccount* acct, const wchar_t* name, const wchar_t* alias, uint16 iid, uint16 gid, int position)
{
	AIMGroup* group = GetGroup(gid);
	Assert(group != 0);

	if(position == -1)
		position = static_cast<int>(group->Contacts.size());

	AIMContact* contact = new AIMContact(acct, group, name, iid, gid, position);
	group->Contacts.push_back(contact);
	Sort<ItemSorter>(group->Contacts);

	if(alias != 0 && *alias != 0)
		contact->Alias = alias;

	if(AIMContact* existingContact = GetContactByID(name))
	{
		contact->IdleStart = existingContact->IdleStart;
		contact->Status = existingContact->Status;
		contact->ScreenName = existingContact->ScreenName;
	}

	contact->OnCreate();

	if(contact->IsConnected())
		contact->OnSignOn();
}

void ContactListImpl::OnContactRemove(AIMAccount*, const wchar_t*, uint16 iid, uint16 gid, int)
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

void ContactListImpl::OnGroupAdd(AIMAccount* acct, const wchar_t* name, uint16, uint16 gid, int position)
{
	if(position == -1)
		position = GetGroupCount();

	AutoPtr<AIMGroup> newGroup(new AIMGroup(acct, name, gid, position));
	
	_groups.push_back(newGroup.Get());
	Sort<ItemSorter>(_groups);
	newGroup->OnCreate();

	newGroup.Release();
}

void ContactListImpl::OnGroupRemove(AIMAccount* acct, const wchar_t*, uint16, uint16 gid, int)
{
	// Find the group the user wants removed.
	AIMGroup* group = 0;
	for(AIMGroups::iterator i = _groups.begin(); i != _groups.end(); ++i)
	{
		if((*i)->GroupID == gid)
		{
			group = *i;
			_groups.erase(i);
			break;
		}
	}

	if(group == 0)
		return;

	// First, remove all contacts contained within the group being removed. 
	// We need to copy the list of contacts out to avoid invalid iterators.
	AIMGroup::AIMContacts contacts;
	foreach(AIMContact* contact, group->Contacts)
		contacts.push_back(contact);

	foreach(AIMContact* contact, contacts)
		OnContactRemove(acct, contact->GetUserID(), contact->ItemID, contact->GroupID, 0);

	// Destroy the group now.
	group->OnDestroy();
	delete group;
}

void ContactListImpl::OnGroupUpdate(AIMAccount*, const wchar_t*, uint16 iid, uint16, int position)
{
	AIMGroup* group = GetGroup(iid);
	if(group == 0)
		return;

	group->Position = position;
	Sort<ItemSorter>(_groups);
		
	group->OnUpdate();
}