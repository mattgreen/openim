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

#ifndef AIM_SSISNACHANDLER_HPP
#define AIM_SSISNACHANDLER_HPP

#pragma warning(push)
#pragma warning(disable:4702)
#include <map>
#include <queue>
#pragma warning(pop)

#include "../AIMContactList.hpp"
#include "../SNACHandler.hpp"
#include "../SSIItem.hpp"

typedef Spry::Delegate<void (const wchar_t*, const wchar_t*)>	ErrorHandler;

class AIMAccount;

class SSISNACHandler :
	public SNACHandler
{
public:
	SSISNACHandler(AIMContactList* list, Framer& framer, Spry::IO::MemoryStream& input, Spry::IO::MemoryStream& output);

	void			AddContact(const wchar_t* user, uint16 gid);
	void			AddGroup(const wchar_t* group);
	void			AliasContact(const wchar_t* alias, uint16 gid, uint16 bid);
	void			RemoveContact(const wchar_t* user, uint16 gid, uint16 bid);
	void			RemoveGroup(const wchar_t* group, uint16 gid);
	void			RenameGroup(const wchar_t* group, uint16 gid);
	State			Process(const SNAC& snac);
	void			RebuildList(AIMGroup** groups, int count);

public:
	AIMAccount*		Account;
	ErrorHandler	OnError;
	
private:
	void			BeginTransaction();
	void			DoReset();
	void			DoSetup();
	void			EndTransaction();
	void			ProcessAcknowledge(const SNAC&);
	void			ProcessContactList(const SNAC&);
	void			ProcessContactListContact(const SSIItem& item, const Spry::IO::StreamOffset& nextItem);
	void			ProcessContactListGroup(const SSIItem& item, const Spry::IO::StreamOffset& nextItem);
	void			ProcessLimitations(const SNAC&);

private:
	enum TransactionType
	{
		AddContactTransactionStart,
		AddContactTransactionModifyGroup,
		AddGroupTransaction,
		ModifyContactTransaction,
		ModifyGroupTransaction,
		RemoveContactTransaction,
		RemoveGroupTransaction,
		RenameGroupTransaction
	};

	struct Transaction
	{
		TransactionType	Type;
		SSIItem			Item;
	};

	typedef std::map<uint16, int>	Positions;
	typedef std::queue<Transaction>	Transactions;

	AIMContactList*	_list;
	Transactions	_transactions;
	Positions		_groups;
	Positions		_currentGroup;
};

#endif