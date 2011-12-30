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

#ifndef OPENIM_COMPONENTS_ACCOUNTMANAGER_ACCOUNTEDITORPAGE_HPP
#define OPENIM_COMPONENTS_ACCOUNTMANAGER_ACCOUNTEDITORPAGE_HPP

#include "../../Account.hpp"
#include "../../String.hpp"
#include "../../WTL.hpp"

#include "../../Resource.h"

#include "GeneralPage.hpp"

class AccountEditorPage :
	public CPropertySheetImpl<AccountEditorPage>
{
public:
	AccountEditorPage(Account* account, String& password);

	BEGIN_MSG_MAP(AccountEditorPage)
		COMMAND_ID_HANDLER_EX(IDCANCEL, OnCancel)
		COMMAND_ID_HANDLER_EX(IDOK, OnOK)
		MSG_WM_INITDIALOG(OnInitDialog)
		CHAIN_MSG_MAP(CPropertySheetImpl<AccountEditorPage>)
	END_MSG_MAP()

	Account*			ThisAccount;
	GeneralPage			General;

private:
	void				OnCancel(UINT, UINT, HWND);
	BOOL				OnInitDialog(HWND, LPARAM);
	void				OnOK(UINT, UINT, HWND);
	
private:
	void				Destroy();

private:
	IAccountManager*	_accountManager;
	bool				_newAccount;
};

#endif