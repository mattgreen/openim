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

#ifndef CONTACTLIST_CONTACTLISTCTRL_HPP
#define CONTACTLIST_CONTACTLISTCTRL_HPP

#include <map>

#include "../../OpenIM/OpenIM.hpp"

#include "../../OpenIM/Interfaces/IResourceManager.hpp"

#include "../../OpenIM/Account.hpp"
#include "../../OpenIM/Contact.hpp"
#include "../../OpenIM/Group.hpp"

#include "../../OpenIM/WTL.hpp"

#include "ContactListCtrlActions.hpp"
#include "ContactListCtrlBehavior.hpp"
#include "Node.hpp"
#include "Resource.h"

class ContactListCtrl :
	public CWindowImpl<ContactListCtrl>
{
public:
	ContactListCtrl();
	~ContactListCtrl();

public:
	void				OnAccountConnect(Account* account, int progress);
	void				OnAccountCreate(Account* account, int position);
	void				OnAccountDestroy(Account* account);
	void				OnAccountDisconnect(Account* account);
	void				OnAccountUpdate(Account* account);

	void				OnContactCreate(Contact* contact, Group* group, int position);
	void				OnContactDestroy(Contact* contact);
	void				OnContactSignOff(Contact* contact);
	void				OnContactSignOn(Contact* contact);
	void				OnContactUpdate(Contact* contact);

	void				OnGroupCreate(Group* group, Group* parentGroup, int position, bool expanded = true);
	void				OnGroupDestroy(Group* group);
	void				OnGroupUpdate(Group* group);

public:
	bool				GetShowOfflineContacts();
	void				SetShowOfflineContacts(bool show);

private:
	void				CreateItem(Node* node, void* parent, bool expanded = true);
	void				FormatIdleTime(int idleTime, wchar_t* buffer, int len);
	Node*				GetSelectedNode();
	void				RemoveItems(Contact* contact);

public:
	DECLARE_WND_CLASS_EX(L"ContactListCtrl", 0, COLOR_3DFACE)

public:
	BEGIN_MSG_MAP(ContactListCtrl)
		COMMAND_ID_HANDLER_EX(ID_ACCOUNTCONTEXT_CONNECT, OnMenuAccountContextConnect)
		COMMAND_ID_HANDLER_EX(ID_ACCOUNTCONTEXT_ADDGROUP, OnMenuGroupContextAdd)
		COMMAND_ID_HANDLER_EX(ID_GROUPCONTEXT_ADDCONTACT, OnMenuGroupContextAddContact)
		COMMAND_ID_HANDLER_EX(ID_GROUPCONTEXT_EXPAND, OnMenuGroupContextToggle)
		COMMAND_ID_HANDLER_EX(ID_GROUPCONTEXT_RENAME, OnMenuGroupContextRename)
		COMMAND_ID_HANDLER_EX(ID_GROUPCONTEXT_DELETE, OnMenuGroupContextDelete)
		COMMAND_ID_HANDLER_EX(ID_CONTACTCONTEXT_RENAME, OnMenuContactContextRename)
		COMMAND_ID_HANDLER_EX(ID_CONTACTCONTEXT_DELETE, OnMenuContactContextDelete)
		MSG_WM_CONTEXTMENU(OnContextMenu)
		MSG_WM_CREATE(OnCreate)
		MSG_WM_SIZE(OnSize)
		NOTIFY_CODE_HANDLER_EX(NM_CUSTOMDRAW, OnCustomDraw)
		NOTIFY_CODE_HANDLER_EX(TVN_BEGINDRAG, OnBeginDrag)
		NOTIFY_CODE_HANDLER_EX(TVN_BEGINLABELEDIT, OnBeginLabelEdit)
		NOTIFY_CODE_HANDLER_EX(TVN_ENDLABELEDIT, OnEndLabelEdit)
		NOTIFY_CODE_HANDLER_EX(TVN_ITEMEXPANDING, OnItemExpanding)
		NOTIFY_CODE_HANDLER_EX(TVN_KEYDOWN, OnKeyDown)
END_MSG_MAP()

private:
	LRESULT				OnBeginDrag(NMHDR* nmhdr);
	LRESULT				OnBeginLabelEdit(NMHDR* nmhdr);
	void				OnContextMenu(HWND, CPoint);
	LRESULT				OnCreate(CREATESTRUCT*);
	LRESULT				OnCustomDraw(NMHDR* hdr);
	LRESULT				OnEndLabelEdit(NMHDR* nmhdr);
	LRESULT				OnItemExpanding(NMHDR* item);
	LRESULT				OnKeyDown(NMHDR* item);
	LRESULT				OnSize(UINT, CSize size);

private:
	void				OnMenuAccountContextConnect(UINT, int, HWND);
	void				OnMenuContactContextAdd(UINT, int, HWND);
	void				OnMenuContactContextDelete(UINT, int, HWND);
	void				OnMenuContactContextMessage(UINT, int, HWND);
	void				OnMenuContactContextRename(UINT, int, HWND);
	void				OnMenuGroupContextAdd(UINT, int, HWND);
	void				OnMenuGroupContextAddContact(UINT, int, HWND);
	void				OnMenuGroupContextDelete(UINT, int, HWND);
	void				OnMenuGroupContextRename(UINT, int, HWND);
	void				OnMenuGroupContextToggle(UINT, int, HWND);

private:
	Account*			GetAccountFor(const CTreeItem& item);
	CTreeItem			GetItemAt(const CPoint&);
	void				ShowAccountContextMenu(Node* node, const CPoint& pt);
	void				ShowContactContextMenu(Node* node, const CPoint& pt);
	void				ShowGroupContextMenu(Node* node, const CPoint& pt);

private:
	typedef std::map<Account*, Node*>		AccountTable;
	typedef std::map<Group*, Node*>			GroupTable;
	typedef std::multimap<Contact*, Node*>	ContactTable;

	CTreeViewCtrlEx			_tree;
	CImageList				_dragImageList;
	CFont					_treeFont;

	IResourceManager*		_resourceManager;
	Transaction				_transaction;

	bool					_dragging;

	AccountTable			_accounts;
	GroupTable				_groups;
	ContactTable			_contacts;

	CMenu					_accountContext;
	CMenu					_groupContext;
	CMenu					_contactContext;

	bool					_showOfflineContacts;
};


#endif