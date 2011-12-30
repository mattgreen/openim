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

#include "SSISNACHandler.hpp"

#include "../AIMAccount.hpp"
#include "../AIMContact.hpp"
#include "../AIMGroup.hpp"

namespace
{
	const wchar_t* SSIErrorMessages[] =
	{
		0,
		L"No such item",
		L"Item already exists",
		L"Item already exists",
		0,
		0,
		0,
		0,
		0,
		0,
		L"Item already exists",
		0,
		L"Cannot add any more items of type",
		L"ICQ user cannot be added to AIM contact list",
		L"Authorization required"
	};
}

SSISNACHandler::SSISNACHandler(AIMContactList* list, Framer& framer, MemoryStream& input, MemoryStream& output) :
	SNACHandler(list, SSI::ServiceID, framer, input, output),
	_list(list)
{
}

void SSISNACHandler::AddContact(const wchar_t* user, uint16 gid)
{
	uint16 bid = _list->GetUniqueBuddyID();

	Transaction transaction;
	transaction.Type = AddContactTransactionStart;
	transaction.Item.Name.Assign(user);
	transaction.Item.GroupID = gid;
	transaction.Item.ItemID = bid;
	transaction.Item.Type = 0;
	_output << transaction.Item;
	_output << (uint16)0;
	SendSNAC(SSI::ServiceID, SSI::AddItem, 0x0080);

	_transactions.push(transaction);
}

void SSISNACHandler::AddGroup(const wchar_t* group)
{
	uint16 gid = _list->GetUniqueGroupID();

	BeginTransaction();

	// Add the group.
	Transaction transaction;
	transaction.Type = AddGroupTransaction;
	transaction.Item.Name.Assign(group);
	transaction.Item.GroupID = gid;
	transaction.Item.ItemID = 0;
	transaction.Item.Type = 1;
	_output << transaction.Item;
	_output << (uint16)0;
	SendSNAC(SSI::ServiceID, SSI::AddItem, 0x0080);

	// Modify the parent group's order.
	SSIItem masterGroup;
	masterGroup.GroupID = 0;
	masterGroup.ItemID = 0;
	masterGroup.Name.Length = 0;
	masterGroup.Type = 1;

	uint16 len = 2 * (static_cast<uint16>(_list->GetGroupCount()) + 1);

	_output << masterGroup;
	_output << static_cast<uint16>(2 + 2 + len);
	_output << static_cast<uint16>(0xC8);
	_output << len;

	for(AIMGroups::iterator i = _list->Groups.begin(); i != _list->Groups.end(); ++i)
		_output << (*i)->GroupID;
	_output << gid;
	SendSNAC(SSI::ServiceID, SSI::ModifyItem, 0x0080);

	EndTransaction();

	_transactions.push(transaction);
}

void SSISNACHandler::AliasContact(const wchar_t* alias, uint16 gid, uint16 bid)
{
	AIMContact* contact = _list->GetContact(bid, gid);
	Assert(contact != 0);

	Transaction transaction;
	transaction.Type = ModifyContactTransaction;
	transaction.Item.Name.Assign(contact->GetUserID());
	transaction.Item.GroupID = gid;
	transaction.Item.ItemID = bid;
	transaction.Item.Type = 0;
	_output << transaction.Item;

	transaction.Item.Name = alias;
	_transactions.push(transaction);

	uint16 len = 0;
	if(alias != 0)
		len = static_cast<uint16>(wcslen(alias));

	if(alias != 0)
	{
		_output << static_cast<uint16>(2 + 2 + len);
		_output << static_cast<uint16>(0x131);
		_output << len;
		_output.WriteChars<Encoding::ASCII>(alias, len);
	}
	else
	{
		_output << static_cast<uint16>(0);
	}

	SendSNAC(SSI::ServiceID, SSI::ModifyItem);

}

void SSISNACHandler::BeginTransaction()
{
	SendSNAC(SSI::ServiceID, SSI::BeginTransaction);
}

void SSISNACHandler::DoReset()
{
	_list->Clear();

	_groups.clear();
	_currentGroup.clear();

	while(! _transactions.empty())
		_transactions.pop();
}

void SSISNACHandler::DoSetup()
{
	SendSNAC(SSI::ServiceID, SSI::Setup);
}

void SSISNACHandler::EndTransaction()
{
	SendSNAC(SSI::ServiceID, SSI::EndTransaction);
}

State SSISNACHandler::Process(const SNAC& snac)
{
	switch(snac.Subtype)
	{
	case SSI::Limitations:
		ProcessLimitations(snac);
		break;

	case SSI::ContactList:
		ProcessContactList(snac);
		break;

	case SSI::Acknowledge:
		ProcessAcknowledge(snac);
		break;
	}

	return NoChange;
}

void SSISNACHandler::ProcessAcknowledge(const SNAC&)
{
	while(_inputStream.Position < _inputStream.Length)
	{
		if(_transactions.empty())
			return;

		Transaction t = _transactions.front();
		_transactions.pop();

		uint16 result = 0;
		_input >> result;

		const wchar_t* caption = 0;

		switch(t.Type)
		{
		case AddContactTransactionStart:
			if(result == 0)
			{
				AIMGroup* group = _list->GetGroup(t.Item.GroupID);
				Assert(group != 0);

				SSIItem parentGroup;
				parentGroup.GroupID = t.Item.GroupID;
				parentGroup.ItemID = 0;
				parentGroup.Name.Assign(group->GetName());
				parentGroup.Type = 1;

				uint16 len = 2 * (static_cast<uint16>(group->Contacts.size()) + 1);

				_output << parentGroup;
				_output << static_cast<uint16>(2 + 2 + len);
				_output << static_cast<uint16>(0xC8);
				_output << len;

				for(AIMGroup::AIMContacts::iterator i = group->Contacts.begin(); i != group->Contacts.end(); ++i)
					_output << (*i)->ItemID;
				_output << t.Item.ItemID;
				SendSNAC(SSI::ServiceID, SSI::ModifyItem);

				t.Type = AddContactTransactionModifyGroup;
				_transactions.push(t);

				_list->OnContactAdd(t.Item.Name, 0, t.Item.ItemID, t.Item.GroupID, -1);
			}
			else
				caption = L"Unable To Add Contact";
			break;

		case AddContactTransactionModifyGroup:
			if(result == 0)
				EndTransaction();

			break;

		case AddGroupTransaction:
			if(result == 0)
				_list->OnGroupAdd(t.Item.Name, t.Item.GroupID, -1);
			else
				caption = L"Unable To Add Group";
			break;
		
		case ModifyContactTransaction:
			if(result == 0)
			{
				const wchar_t* alias = 0;
				if(t.Item.Name.Length > 0)
					alias = t.Item.Name;

				_list->OnContactAlias(alias, t.Item.ItemID, t.Item.GroupID);
			}

			break;

		case RemoveContactTransaction:
			if(result == 0)
				_list->OnContactRemove(t.Item.Name, t.Item.ItemID, t.Item.GroupID);
			else
				caption = L"Unable To Remove Contact";

			break;

		case RemoveGroupTransaction:
			if(result == 0)
				_list->OnGroupRemove(t.Item.Name, t.Item.GroupID);
			else
				caption = L"Unable To Remove Group";

			break;

		case RenameGroupTransaction:
			if(result == 0)
				_list->OnGroupRename(t.Item.Name, t.Item.GroupID);
			else
				caption = L"Unable To Rename Group";

			break;
		}
		
		if(result != 0)
		{
			Assert(OnError != 0);
			
			const wchar_t* errorMessage = 0;
			if(result > 0 && result < DimensionOf(SSIErrorMessages))
				errorMessage = SSIErrorMessages[result];

			OnError(caption, errorMessage);
		}
	}
}

void SSISNACHandler::ProcessContactList(const SNAC& snac)
{
	unsigned char version = 0;
	_input >> version;

	uint16 items = 0;
	_input >> items;

	for(uint16 i = 0; i < items; i++)
	{
		SSIItem item;
		_input >> item;

		uint16 len = 0;
		_input >> len;

		StreamOffset nextItem = _inputStream.Position + len;
		switch(item.Type)
		{
		case 0x00:	// contact
			ProcessContactListContact(item, nextItem);
			break;

		case 0x01:	// group
			_currentGroup.clear();
			ProcessContactListGroup(item, nextItem);
			break;
		}

		if(_inputStream.Position != nextItem)
			_inputStream.Seek(nextItem);
	}

	// Bit one of the SNAC flags is zero if this is the last chunk of the list.
	if((snac.Flags & 0x01) == 0)
	{
		SendSNAC(SSI::ServiceID, SSI::Activate);
	}
}

void SSISNACHandler::ProcessContactListContact(const SSIItem& item, const StreamOffset& nextItem)
{
	int position = 0;
	if(_currentGroup.find(item.ItemID) != _currentGroup.end())
		position = _currentGroup[item.ItemID];

	wchar_t alias[128] = { 0, };
	TLVReader reader(_input, _inputStream);
	while(_inputStream.Position < nextItem && reader.Read())
	{
		if(reader.Type == 0x131) // Alias
			_input.ReadChars<Encoding::ASCII>(alias, 128, reader.Length);
	}

	_list->OnContactAdd(item.Name, alias, item.ItemID, item.GroupID, position);
}

void SSISNACHandler::ProcessContactListGroup(const SSIItem& item, const StreamOffset& nextItem)
{
	bool master = item.GroupID == 0;

	TLVReader reader(_input, _inputStream);
	while(_inputStream.Position < nextItem && reader.Read())
	{
		if(reader.Type == 0xC8)	// Group ordering
		{
			uint16 idCount = reader.Length / sizeof(uint16);
			for(uint16 j = 0; j < idCount; j++)
			{
				uint16 id = 0;
				_input >> id;

				if(master)
					_groups[id] = static_cast<int>(_groups.size());
				else
					_currentGroup[id] = static_cast<int>(_currentGroup.size());
			}
		}
	}

	if(! master)
	{
		int position = 0;
		if(_groups.find(item.GroupID) != _groups.end())
			position = _groups[item.GroupID];
		
		_list->OnGroupAdd(item.Name, item.GroupID, position);
	}
}

void SSISNACHandler::ProcessLimitations(const SNAC&)
{
	_setup = true;

	uint32 modTime = 0;
	_output << modTime;

	uint16 items = 0;
	_output << items;

	SendSNAC(SSI::ServiceID, SSI::CheckListModificationDate);
}

void SSISNACHandler::RemoveContact(const wchar_t* user, uint16 gid, uint16 bid)
{
	Transaction transaction;
	transaction.Type = RemoveContactTransaction;
	transaction.Item.Name.Assign(user);
	transaction.Item.GroupID = gid;
	transaction.Item.ItemID = bid;
	transaction.Item.Type = 0;
	_output << transaction.Item;
	_output << (uint16)0;

	SendSNAC(SSI::ServiceID, SSI::RemoveItem, 0x0080);
	
	_transactions.push(transaction);
}

void SSISNACHandler::RemoveGroup(const wchar_t* group, uint16 gid)
{
	Transaction transaction;
	transaction.Type = RemoveGroupTransaction;
	transaction.Item.Name.Assign(group);
	transaction.Item.GroupID = gid;
	transaction.Item.ItemID = 0;
	transaction.Item.Type = 1;
	_output << transaction.Item;
	_output << (uint16)0;

	SendSNAC(SSI::ServiceID, SSI::RemoveItem, 0x0080);		
	_transactions.push(transaction);
}

void SSISNACHandler::RenameGroup(const wchar_t* newName, uint16 gid)
{
	AIMGroup* group = _list->GetGroup(gid);
	Assert(group != 0);

	Transaction transaction;
	transaction.Type = RenameGroupTransaction;
	transaction.Item.Name.Assign(newName);
	transaction.Item.GroupID = gid;
	transaction.Item.ItemID = 0;
	transaction.Item.Type = 1;
	_output << transaction.Item;

	uint16 len = 2 * static_cast<uint16>(group->Contacts.size());

	_output << static_cast<uint16>(2 + 2 + len);
	_output << static_cast<uint16>(0xC8);
	_output << len;

	for(AIMGroup::AIMContacts::iterator i = group->Contacts.begin(); i != group->Contacts.end(); ++i)
		_output << (*i)->ItemID;

	_transactions.push(transaction);

	SendSNAC(SSI::ServiceID, SSI::ModifyItem);
}