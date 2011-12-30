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

#ifndef CONTACTLIST_CONTACTLISTWINDOW_HPP
#define CONTACTLIST_CONTACTLISTWINDOW_HPP

#include "../../OpenIM/Events/AccountEvents.hpp"
#include "../../OpenIM/Events/ContactEvents.hpp"
#include "../../OpenIM/Events/GroupEvents.hpp"
#include "../../OpenIM/Events/EventBindings.hpp"
#include "../../OpenIM/WTL.hpp"

#include "ContactListCtrl.hpp"
#include "Resource.h"

const int StatusChangerID = 0x10000;

class ContactListWindow :
	public CFrameWindowImpl<ContactListWindow>
{
public:
	ContactListWindow();

public:
	void OnAccountConnect(AccountConnectEvent& e);
	void OnAccountCreate(AccountCreateEvent& e);
	void OnAccountDisconnect(AccountDisconnectEvent& e);
	void OnAccountDestroy(AccountDestroyEvent& e);
	void OnAccountError(AccountErrorEvent& e);
	void OnAccountUpdate(AccountUpdateEvent& e);

	void OnContactCreate(ContactCreateEvent& e);
	void OnContactDestroy(ContactDestroyEvent& e);
	void OnContactSignOff(ContactSignOffEvent& e);
	void OnContactSignOn(ContactSignOnEvent& e);
	void OnContactUpdate(ContactUpdateEvent& e);

	void OnGroupCreate(GroupCreateEvent& e);
	void OnGroupDestroy(GroupDestroyEvent& e);
	void OnGroupUpdate(GroupUpdateEvent& e);

public:
	DECLARE_FRAME_WND_CLASS_EX(L"OpenIM Contact List", 0, WS_OVERLAPPED, COLOR_3DFACE)

public:
	BEGIN_MSG_MAP(ContactListWindow)
		COMMAND_ID_HANDLER_EX(ID_FILE_EXIT, OnMenuFileExit)
		COMMAND_ID_HANDLER_EX(ID_VIEW_ALWAYSONTOP, OnMenuViewAlwaysOnTop)
		COMMAND_RANGE_HANDLER_EX(ID_OPACITY_20, ID_OPACITY_100, OnChangeTransparency)
		COMMAND_ID_HANDLER_EX(ID_TOOLS_ACCOUNTS, OnMenuToolsAccounts)
		COMMAND_ID_HANDLER_EX(ID_TOOLS_OPTIONS, OnMenuToolsOptions)
		MSG_WM_CREATE(OnCreate)
		MSG_WM_CLOSE(OnClose)
		MSG_WM_GETMINMAXINFO(OnGetMinMaxInfo)
		MSG_WM_INITMENUPOPUP(OnInitMenuPopup)
		MSG_WM_SIZE(OnSize)
		NOTIFY_CODE_HANDLER_EX(TBN_DROPDOWN, OnShowStatusMenu)
		CHAIN_MSG_MAP(CFrameWindowImpl<ContactListWindow>)
	END_MSG_MAP()

public:
	void	OnClose();
	void	OnChangeTransparency(UINT, int, HWND);
	LRESULT	OnCreate(CREATESTRUCT*);
	void	OnGetMinMaxInfo(MINMAXINFO* minMaxInfo);
	void	OnInitMenuPopup(HMENU menu, UINT, BOOL);
	void	OnMenuFileExit(UINT, int, HWND);
	void	OnMenuToolsAccounts(UINT, int, HWND);
	void	OnMenuToolsOptions(UINT, int, HWND);
	void	OnMenuViewAlwaysOnTop(UINT, int, HWND);
	LRESULT	OnShowStatusMenu(NMHDR*);
	void	OnSize(UINT, CSize);

private:
	bool	GetAlwaysOnTop() const;
	void	SetAlwaysOnTop(bool top);
	void	SetOpacity(int percent);

private:
	typedef BOOL (WINAPI * SetLayeredWindowAttributesFn)(HWND, COLORREF, BYTE, DWORD);

	ContactListCtrl					_ctrl;
	CToolBarCtrl					_toolBar;
	CMenu							_statusChangeMenu;
	IResourceManager*				_resourceManager;

	EventBindings					_bindings;
	CMenu							_opacityMenu;
	SetLayeredWindowAttributesFn	_setLayeredAttributes;
	int								_opacity;
	bool							_topMost;
};

#endif