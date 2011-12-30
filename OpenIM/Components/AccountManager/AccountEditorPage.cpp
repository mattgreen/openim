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

#include "AccountEditorPage.hpp"

#include "../../ComponentManager.hpp"
#include "../../Spry.hpp"
#include "../../Interfaces/IAccountManager.hpp"

AccountEditorPage::AccountEditorPage(Account* account, String& password) :
	CPropertySheetImpl<AccountEditorPage>(account == 0 ? L"Add Account" : L"Edit Account"),
		
	ThisAccount(account),
	General(ThisAccount, password),
	_accountManager(ComponentManager::Get<IAccountManager>()),
	_newAccount(ThisAccount == 0)
{
	Assert(_accountManager != 0);

	m_psh.dwFlags |= PSH_NOAPPLYNOW;
	AddPage(General);
}

void AccountEditorPage::Destroy()
{
	if(ThisAccount != 0)
	{
		_accountManager->Destroy(ThisAccount);
		ThisAccount = 0;
	}
}

void AccountEditorPage::OnCancel(UINT, UINT, HWND)
{
	// If we were creating a new account, then dispose of it now - the user doesn't want it.
	if(_newAccount)
		Destroy();

	SetMsgHandled(FALSE);
}

BOOL AccountEditorPage::OnInitDialog(HWND, LPARAM)
{
	// Remove the context help button on the title bar.
	ModifyStyle(WS_EX_CONTEXTHELP, 0);
	UpdateWindow();

	return TRUE;
}

void AccountEditorPage::OnOK(UINT, UINT, HWND)
{
	// Create the new account. Ideally, we'd be doing this inside one of the property sheets, however
	// I'm not 100% certain if they are guaranteed to be validated in any particular order. As such,
	// the responsibility is delegated upward.
	if(_newAccount)
	{
		// DDX the general page so we can figure out what protocol was selected.
		General.DoDataExchange(TRUE);

		// Destroy any existing account - there is no danger of this account actually being used yet.
		// It may also be the wrong protocol.
		Destroy();

		// Create a new account now.
		ThisAccount = _accountManager->Create(General.Protocol);			
		if(ThisAccount == 0)
		{
			MessageBox(L"Unable to create a new account with the requested protocol.", 
				L"Unable to Create Account", MB_ICONERROR);
			
			// Early return inhibits property sheet notification.
			return;
		}
	}

	SetMsgHandled(FALSE);
}