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

#ifndef OPENIM_COMPONENTS_ACCOUNTMANAGER_GENERALPAGE_HPP
#define OPENIM_COMPONENTS_ACCOUNTMANAGER_GENERALPAGE_HPP

#include "../../Account.hpp"
#include "../../ComponentManager.hpp"
#include "../../Interfaces/IAccountManager.hpp"
#include "../../Interfaces/IResourceManager.hpp"
#include "../../String.hpp"
#include "../../Spry.hpp"
#include "../../WTL.hpp"

#include "../../Resource.h"

class GeneralPage :
	public CPropertyPageImpl<GeneralPage>,
	public WinDataExchangeEx<GeneralPage>
{
public:
	enum { IDD = IDD_ACCOUNT_GENERAL_PAGE };

	GeneralPage(Account*& account, String& password) :
		ThisAccount(account),
		_password(password)
	{
		memset(Label, 0, sizeof(Label));
		memset(Protocol, 0, sizeof(Protocol));
		memset(UserID, 0, sizeof(UserID));
		memset(Password, 0, sizeof(Password));
		memset(DisplayName, 0, sizeof(DisplayName));

		if(account != 0)
		{
			wcsncpy(Protocol, account->GetProtocolName(), 63);
			wcsncpy(UserID, account->GetUserID(), 63);
			wcsncpy(Password, password.c_str(), 63);
			
			if(account->GetDisplayName() != 0)
				wcsncpy(DisplayName, account->GetDisplayName(), 63);
			
			if(account->GetLabel() != 0)
				wcsncpy(Label, account->GetLabel(), 63);
		}
	}

public:
	BEGIN_MSG_MAP(GeneralPage)
		COMMAND_CODE_HANDLER(CBN_SELCHANGE, OnChangeProtocol)
		COMMAND_ID_HANDLER(IDC_RETRIEVE, OnRetrieve)
		COMMAND_ID_HANDLER(IDC_SIGNUP, OnSignup)
		MSG_WM_INITDIALOG(OnInitDialog)
		CHAIN_MSG_MAP(CPropertyPageImpl<GeneralPage>)
	END_MSG_MAP()

	BEGIN_DDX_MAP(GeneralPage)
		DDX_COMBO_TEXT(IDC_PROTOCOL, Protocol)
		DDX_TEXT(IDC_LABEL, Label)
		DDX_TEXT(IDC_USERID, UserID)
		DDX_TEXT(IDC_PASSWORD, Password)
		DDX_TEXT(IDC_DISPLAYNAME, DisplayName)
	END_DDX_MAP()

public:
	LRESULT OnChangeProtocol(WORD, WORD, HWND, BOOL&)
	{
		UpdateButtons();

		return 0;
	}

	BOOL OnInitDialog (HWND, LPARAM)
	{
		_lostPasswordButton.Attach(GetDlgItem(IDC_RETRIEVE));
		_signupButton.Attach(GetDlgItem(IDC_SIGNUP));
		_userID.Attach(GetDlgItem(IDC_USERID));
		_protocols.Attach(GetDlgItem(IDC_PROTOCOL));

		// Attach presence image list to protocol combobox.
		IResourceManager* resMgr = ComponentManager::Get<IResourceManager>();
		Assert(resMgr != 0);
		if(resMgr != 0)
		{
			if(HIMAGELIST imageList = resMgr->GetImageList())
				_protocols.SetImageList(imageList);
		}

		// Can't change the protocol on an existing account.
		// It is just too messy to deal with at the moment.
		if(ThisAccount != 0)
			_protocols.EnableWindow(FALSE);
		
		// Populate account list
		const wchar_t** protocols = 0;
		if(IAccountManager* mgr = ComponentManager::Get<IAccountManager>())
			protocols = mgr->GetProtocols();

		for(int i = 0; protocols[i] != 0; i++)
		{
			int image = -1;
			if(resMgr != 0)
				image = resMgr->GetImageIndex(protocols[i]);

			COMBOBOXEXITEM item;
			item.mask = CBEIF_TEXT | CBEIF_IMAGE | CBEIF_SELECTEDIMAGE;
			item.iItem = -1;
			item.iImage = image;
			item.iSelectedImage = image;
			item.pszText = const_cast<wchar_t*>(protocols[i]);
			_protocols.InsertItem(&item);
		}

		// Select first protocol type if creating new protocol.
		if(ThisAccount == 0 && protocols[0] != 0)
			_protocols.SetCurSel(0);

		if(ThisAccount != 0)
			DoDataExchange(FALSE);

		UpdateButtons();

		return TRUE;
	}

	int OnApply()
	{
		const wchar_t* errorMsg = UpdateAccount();
		if(errorMsg != 0)
		{
			MessageBox(errorMsg, L"Unable to Update Account", MB_OK);

			return PSNRET_INVALID;
		}

		_password = Password;
		return PSNRET_NOERROR;
	}

	BOOL OnKillActive()
	{
		DoDataExchange(TRUE);

		if(_userID.GetWindowTextLength() == 0)
		{
			MessageBox(L"Invalid User ID.", L"Unable to Update Account", MB_ICONHAND);
			_userID.SetFocus();

			return TRUE;
		}

		return FALSE;
	}

	LRESULT OnRetrieve(WORD, WORD, HWND, BOOL&)
	{
		IAccountManager* mgr = ComponentManager::Get<IAccountManager>();
		if(mgr == 0)
			return 0;

		Spry::WCharBuffer<64> protocol;
		_protocols.GetWindowText(protocol, protocol.Capacity);
		
		const wchar_t* url = mgr->GetLostPasswordURL(protocol);
		if(url == 0)
			return 0;

		ShellExecute(0, L"open", url, 0, 0, SW_SHOW);

		return 0;
	}

	LRESULT	OnSignup(WORD, WORD, HWND, BOOL&)
	{
		IAccountManager* mgr = ComponentManager::Get<IAccountManager>();
		Assert(mgr != 0);

		Spry::WCharBuffer<64> protocol;
		protocol.Length = _protocols.GetWindowText(protocol, protocol.Capacity);
		const wchar_t* url = mgr->GetSignupURL(protocol);
		if(url == 0)
			return 0;

		ShellExecute(0, L"open", url, 0, 0, SW_SHOW);

		return 0;
	}

	const wchar_t* UpdateAccount()
	{
		Assert(ThisAccount);
		
		bool valid = ThisAccount->SetUserID(UserID);
		if(! valid)
			return L"Invalid user ID specified.";

		ThisAccount->SetPassword(Password);
		ThisAccount->SetLabel(Label);
		ThisAccount->SetDisplayName(DisplayName);

		return 0;
	}

	void UpdateButtons()
	{
		IAccountManager* mgr = ComponentManager::Get<IAccountManager>();
		Assert(mgr != 0);

		Spry::WCharBuffer<64> protocol;
		protocol.Length = _protocols.GetLBText(_protocols.GetCurSel(), protocol);

		_signupButton.EnableWindow(mgr->GetSignupURL(protocol) != 0);
		_lostPasswordButton.EnableWindow(mgr->GetLostPasswordURL(protocol) != 0);
	}

public:
	Account*&		ThisAccount;

	wchar_t			Protocol[64];
	wchar_t			Label[64];
	wchar_t			UserID[64];
	wchar_t			Password[64];
	wchar_t			DisplayName[64];

private:
	String&			_password;
	CComboBoxEx		_protocols;
	CEdit			_userID;
	CButton			_signupButton;
	CButton			_lostPasswordButton;
};

#endif