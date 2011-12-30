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

#ifndef OPENIM_COMPONENTS_ACCOUNTMANAGER_ACCOUNTEDITORDIALOG_HPP
#define OPENIM_COMPONENTS_ACCOUNTMANAGER_ACCOUNTEDITORDIALOG_HPP

#pragma warning(push)
#pragma warning(disable:4702)
#include <map>
#include <vector>
#pragma warning(pop)

#include "../../WTL.hpp"

#include "../../Account.hpp"
#include "../../Interfaces/IAccountManager.hpp"
#include "../../Interfaces/IResourceManager.hpp"
#include "../../Resource.h"
#include "../../String.hpp"

#include "AccountEditorPage.hpp"

typedef std::vector<Account*>			Accounts;
typedef std::map<Account*, String>		Passwords;

class AccountEditorDialog :
	public CDialogImpl<AccountEditorDialog>
{
public:
	AccountEditorDialog(Accounts& accounts, Passwords& passwords);

	enum
	{
		IDD = IDD_ACCOUNTEDITOR
	};

	BEGIN_MSG_MAP(AccountEditorDialog)
		COMMAND_ID_HANDLER(IDCANCEL, OnClose)
		COMMAND_ID_HANDLER(IDOK, OnClose)
		COMMAND_ID_HANDLER(IDC_ADD, OnAdd)
		COMMAND_ID_HANDLER(IDC_EDIT, OnEdit)
		COMMAND_ID_HANDLER(IDC_REMOVE, OnRemove)
		MSG_WM_INITDIALOG(OnInitDialog)
		NOTIFY_CODE_HANDLER(LVN_ITEMCHANGED, OnItemChanged)
		NOTIFY_CODE_HANDLER(NM_DBLCLK, OnDoubleClick)
		//CHAIN_MSG_MAP(CDialogImpl<AccountEditorDialog>)
	END_MSG_MAP()

private:
	void			DestroyAccount(Account* account);

private:
	LRESULT			OnAdd(WORD, WORD, HWND, BOOL&);
	LRESULT			OnClose(WORD, WORD, HWND, BOOL&);
	LRESULT 		OnDoubleClick(int, NMHDR*, BOOL&);
	LRESULT			OnEdit(WORD, WORD, HWND, BOOL&);
	BOOL			OnInitDialog(HWND, LPARAM);
	LRESULT 		OnItemChanged(int, NMHDR*, BOOL&);
	LRESULT			OnRemove(WORD, WORD, HWND, BOOL&);

private:
	void			Refresh();
	const wchar_t*	UpdateAccount(Account* account, AccountEditorPage& page);
	void			UpdateButtons();

private:
	IAccountManager*	_accountManager;
	IResourceManager*	_resourceManager;

	Accounts&			_accounts;
	Passwords&			_passwords;

	CListViewCtrl		_userAccounts;
	CButton				_editButton;
	CButton				_removeButton;
};

#endif