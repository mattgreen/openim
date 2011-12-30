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

#include "AccountEditorDialog.hpp"

#include "../../ComponentManager.hpp"
#include "../../Events/AccountEvents.hpp"
#include "../../Spry.hpp"


AccountEditorDialog::AccountEditorDialog(Accounts& accounts, Passwords& passwords) :
	_accountManager(ComponentManager::Get<IAccountManager>()),
	_resourceManager(ComponentManager::Get<IResourceManager>()),
	_accounts(accounts),
	_passwords(passwords)
{
}

void AccountEditorDialog::DestroyAccount(Account* account)
{
	Assert(account != 0);

	AccountDestroyEvent e;
	e.Account = account;
	e.Invoke();

	_accountManager->Destroy(account);
	
	_passwords.erase(account);
	_accounts.erase(std::find(_accounts.begin(), _accounts.end(), account));
}

LRESULT	AccountEditorDialog::OnAdd(WORD, WORD, HWND, BOOL&)
{
	String password;
	AccountEditorPage page(0, password);
	
	if(page.DoModal() == IDOK)
	{
		if(page.ThisAccount == 0)
		{
			MessageBox(L"Unable to create account!");

			return 0;
		}

		_accounts.push_back(page.ThisAccount);
		_passwords[page.ThisAccount] = password;

		AccountCreateEvent e;
		e.Account = page.ThisAccount;
		e.Position = (int)(_accounts.size() - 1);
		e.Invoke();

		Refresh();
	}
	
	return 0;
}

LRESULT	AccountEditorDialog::OnClose(WORD, WORD, HWND, BOOL&)
{
	EndDialog(IDOK);

	return 0;
}

LRESULT AccountEditorDialog::OnDoubleClick(int, NMHDR*, BOOL&)
{
	BOOL handled = FALSE;
	OnEdit(0, 0, 0, handled);

	return 0;
}

LRESULT	AccountEditorDialog::OnEdit(WORD, WORD, HWND, BOOL&)
{
	// Get the selected Account.
	int index = _userAccounts.GetSelectedIndex();
	if(index == -1)
		return 0;
	
	Account* account = reinterpret_cast<Account*>(_userAccounts.GetItemData(index));
	Assert(account != 0);
	if(account == 0)
		return 0;

	// Display the account properties.
	AccountEditorPage page(account, _passwords[account]);
	if(page.DoModal() == IDOK)
	{
		// Try to update. The property sheet is responsible for data validation.
		const wchar_t* errorMessage = UpdateAccount(account, page);
		if(errorMessage != 0)
		{
			MessageBox(errorMessage, L"Unable to Edit Account", MB_OK);

			return 0;
		}

		_passwords[page.ThisAccount] = String(page.General.Password);

		AccountUpdateEvent e;
		e.Account = account;
		e.Invoke();

		Refresh();
	}

	return 0;
}

BOOL AccountEditorDialog::OnInitDialog(HWND, LPARAM)
{
	// Bind to UI controls.
	_userAccounts.Attach(GetDlgItem(IDC_ACCOUNTS));
	_editButton.Attach(GetDlgItem(IDC_EDIT));
	_removeButton.Attach(GetDlgItem(IDC_REMOVE));

	// Set up list view.
	HIMAGELIST imageList = _resourceManager->GetImageList();
	if(imageList != 0)
		_userAccounts.SetImageList(imageList, LVSIL_SMALL);

	_userAccounts.AddColumn(L"User ID", 0);
	_userAccounts.AddColumn(L"Service", 1);
	_userAccounts.AddColumn(L"Status", 2);
	_userAccounts.SetExtendedListViewStyle(LVS_EX_FULLROWSELECT);
	Refresh();
	
	return TRUE;
}

LRESULT AccountEditorDialog::OnItemChanged(int, NMHDR*, BOOL&)
{
	UpdateButtons();

	return 0;
}

LRESULT	AccountEditorDialog::OnRemove(WORD, WORD, HWND, BOOL&)
{
	int result = MessageBox(L"Are you sure you want to delete the selected account?",
							L"Confirm Account Deletion", MB_YESNO | MB_ICONWARNING);
	if(result == IDYES)
	{
		int index = _userAccounts.GetSelectedIndex();
		if(index == -1)
			return 0;

		Account* account = reinterpret_cast<Account*>(_userAccounts.GetItemData(index));
		Assert(account != 0);

		if(account->GetConnectionStatus() == Connecting || account->GetConnectionStatus() == Connected)
			account->Disconnect();

		DestroyAccount(account);
		Refresh();
	}

	return 0;
}

void AccountEditorDialog::Refresh()
{
	// Add all accounts
	_userAccounts.DeleteAllItems();

	int index = 0;
	foreach(Account* account, _accounts)
	{
		int imageIndex = _resourceManager->GetImageIndex(account);

		_userAccounts.InsertItem(index, account->GetUserID(), imageIndex);
		_userAccounts.SetItemText(index, 1, account->GetProtocolName());
		
		if(account->GetConnectionStatus() == Connected)
			_userAccounts.SetItemText(index, 2, L"Connected");
		else
			_userAccounts.SetItemText(index, 2, L"Disconnected");
		_userAccounts.SetItemData(index, reinterpret_cast<LONG_PTR>(account));
		index++;
	}
	
	// Size column headers appropriately.
	_userAccounts.SetColumnWidth(0, LVSCW_AUTOSIZE);
	if(_userAccounts.GetColumnWidth(0) < 95)
		_userAccounts.SetColumnWidth(0, 95);

	_userAccounts.SetColumnWidth(1, LVSCW_AUTOSIZE_USEHEADER);
	_userAccounts.SetColumnWidth(2, LVSCW_AUTOSIZE_USEHEADER);
	
	UpdateButtons();
}

const wchar_t* AccountEditorDialog::UpdateAccount(Account* account, AccountEditorPage& page)
{
	Assert(account != 0);

	bool valid = account->SetUserID(page.General.UserID);
	if(! valid)
		return L"Invalid user ID specified.";
	
	account->SetPassword(page.General.Password);

	return 0;
}

void AccountEditorDialog::UpdateButtons()
{
	BOOL canEditOrDelete = _userAccounts.GetSelectedCount() > 0 ? TRUE : FALSE;
	_editButton.EnableWindow(canEditOrDelete);
	_removeButton.EnableWindow(canEditOrDelete);
}
