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

#include "ContactListWindow.hpp"

#include <cmath>

#include "../../OpenIM/Events/SystemEvents.hpp"
#include "../../OpenIM/Interfaces/IAccountManager.hpp"
#include "../../OpenIM/Interfaces/IPreferencesManager.hpp"
#include "../../OpenIM/Interfaces/IResourceManager.hpp"
#include "Main.hpp"

ContactListWindow::ContactListWindow() :
	_statusChangeMenu(LoadMenu(Module, MAKEINTRESOURCE(IDR_STATUS))),
	_resourceManager(ComponentManager::Get<IResourceManager>()),
	_opacityMenu(GetSubMenu(LoadMenu(Module, MAKEINTRESOURCE(IDR_OPACITYMENU)), 0)),
	_setLayeredAttributes(reinterpret_cast<SetLayeredWindowAttributesFn>(GetProcAddress(GetModuleHandle(L"user32.dll"), "SetLayeredWindowAttributes"))),
	_opacity(100),
	_topMost(false)
{
	Assert(_resourceManager != 0);

	_bindings.Add(this, &ContactListWindow::OnAccountConnect);
	_bindings.Add(this, &ContactListWindow::OnAccountCreate);
	_bindings.Add(this, &ContactListWindow::OnAccountDisconnect);
	_bindings.Add(this, &ContactListWindow::OnAccountDestroy);
	_bindings.Add(this, &ContactListWindow::OnAccountError);
	_bindings.Add(this, &ContactListWindow::OnAccountUpdate);

	_bindings.Add(this, &ContactListWindow::OnContactCreate);
	_bindings.Add(this, &ContactListWindow::OnContactDestroy);
	_bindings.Add(this, &ContactListWindow::OnContactSignOff);
	_bindings.Add(this, &ContactListWindow::OnContactSignOn);
	_bindings.Add(this, &ContactListWindow::OnContactUpdate);

	_bindings.Add(this, &ContactListWindow::OnGroupCreate);
	_bindings.Add(this, &ContactListWindow::OnGroupDestroy);
	_bindings.Add(this, &ContactListWindow::OnGroupUpdate);
}

bool ContactListWindow::GetAlwaysOnTop() const
{
	return _topMost;
}

void ContactListWindow::OnAccountConnect(AccountConnectEvent& e)
{
	_ctrl.OnAccountConnect(e.Account, e.Progress);
}

void ContactListWindow::OnAccountCreate(AccountCreateEvent& e)
{
	_ctrl.OnAccountCreate(e.Account, e.Position);
}

void ContactListWindow::OnAccountDestroy(AccountDestroyEvent& e)
{
	_ctrl.OnAccountDestroy(e.Account);
}

void ContactListWindow::OnAccountDisconnect(AccountDisconnectEvent& e)
{
	_ctrl.OnAccountDisconnect(e.Account);
}

void ContactListWindow::OnAccountError(AccountErrorEvent& e)
{
	MessageBox(e.Message, e.Caption, MB_ICONWARNING | MB_OK);
}

void ContactListWindow::OnAccountUpdate(AccountUpdateEvent& e)
{
	_ctrl.OnAccountUpdate(e.Account);
}

void ContactListWindow::OnClose()
{
	RequestShutdownEvent request;
	request.Cancel = false;
	request.Invoke();

	if(! request.Cancel)
		PostQuitMessage(0);
}

void ContactListWindow::OnChangeTransparency(UINT, int id, HWND)
{
	SetOpacity((2 + (id - ID_OPACITY_20)) * 10);
}

void ContactListWindow::OnContactCreate(ContactCreateEvent& e)
{
	_ctrl.OnContactCreate(e.Contact, e.Group, e.Position);
}

void ContactListWindow::OnContactDestroy(ContactDestroyEvent& e)
{
	_ctrl.OnContactDestroy(e.Contact);
}

void ContactListWindow::OnContactSignOff(ContactSignOffEvent& e)
{
	_ctrl.OnContactSignOff(e.Contact);
}

void ContactListWindow::OnContactSignOn(ContactSignOnEvent& e)
{
	_ctrl.OnContactSignOn(e.Contact);
}

void ContactListWindow::OnContactUpdate(ContactUpdateEvent& e)
{
	_ctrl.OnContactUpdate(e.Contact);
}

LRESULT ContactListWindow::OnCreate(CREATESTRUCT*)
{
	_toolBar.Create(*this, 0, 0,
		WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE | TBSTYLE_FLAT | TBSTYLE_LIST | CCS_BOTTOM);
	_toolBar.SetButtonStructSize(sizeof(TBBUTTON));
	_toolBar.SetButtonSize(160, 32);
	_toolBar.SetPadding(0,0);
	//_toolBar.SetImageList(mgr->GetImageList());
	DWORD size = _toolBar.GetButtonSize();

	TBBUTTON button;
	button.iBitmap = _resourceManager->GetImageIndex(L"AIM");
	button.idCommand = StatusChangerID;
	button.iString = reinterpret_cast<int>(L"Status: Disconnected");
	button.fsStyle = TBSTYLE_DROPDOWN | BTNS_WHOLEDROPDOWN;
	button.fsState = TBSTATE_ENABLED;
	_toolBar.AddButtons(1, &button);
	
	ZeroMemory(&button, sizeof(button));
	button.fsStyle = TBSTYLE_SEP;
	button.fsState = TBSTATE_ENABLED;
	_toolBar.AddButtons(1, &button);

	_ctrl.Create(*this, 0, 0, WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE);
	Assert(_ctrl != 0);

	ModifyStyle(WS_MAXIMIZEBOX, 0);
	SetMenu(LoadMenu(Module, MAKEINTRESOURCE(IDR_MAINMENU)));
	SetWindowText(L"OpenIM Contact List");

	// Win32 can bite me; the resource editor doesn't provide a way to assign IDs to submenus.
	// So, I do it at runtime.
	CMenuHandle menu(GetMenu());
	CMenuItemInfo info;
	info.fMask = MIIM_SUBMENU;
	info.hSubMenu = _opacityMenu;
	menu.SetMenuItemInfo(ID_VIEW_OPACITY, FALSE, &info);
	
	// Enable the Opacity menu item if the OS supports it.
	if(_setLayeredAttributes != 0)
		menu.EnableMenuItem(ID_VIEW_OPACITY, MF_ENABLED);	

	return 0;
}

void ContactListWindow::OnGetMinMaxInfo(MINMAXINFO* minMaxInfo)
{
	minMaxInfo->ptMinTrackSize.x = 150;
	minMaxInfo->ptMinTrackSize.y = 250;
}

void ContactListWindow::OnGroupCreate(GroupCreateEvent& e)
{
	_ctrl.OnGroupCreate(e.Group, e.Parent, e.Position, true);
}

void ContactListWindow::OnGroupDestroy(GroupDestroyEvent& e)
{
	_ctrl.OnGroupDestroy(e.Group);
}

void ContactListWindow::OnGroupUpdate(GroupUpdateEvent& e)
{
	_ctrl.OnGroupUpdate(e.Group);	
}

void ContactListWindow::OnInitMenuPopup(HMENU menu, UINT, BOOL)
{
	if(menu == _opacityMenu)
	{
		int opacityIndex = (_opacity / 10) - 2;
		CheckMenuRadioItem(_opacityMenu, ID_OPACITY_20, ID_OPACITY_100, ID_OPACITY_20 + opacityIndex, MF_BYCOMMAND);
	}
}

void ContactListWindow::OnMenuFileExit(UINT, int, HWND)
{
	PostMessage(WM_CLOSE);
}

void ContactListWindow::OnMenuToolsAccounts(UINT, int, HWND)
{
	if(IAccountManager* mgr = ComponentManager::Get<IAccountManager>())
		mgr->Show();
}

void ContactListWindow::OnMenuToolsOptions(UINT, int, HWND)
{
	if(IPreferencesManager* mgr = ComponentManager::Get<IPreferencesManager>())
		mgr->Show();
}

void ContactListWindow::OnMenuViewAlwaysOnTop(UINT, int, HWND)
{
	SetAlwaysOnTop(! GetAlwaysOnTop());
}

LRESULT ContactListWindow::OnShowStatusMenu(NMHDR*)
{
	CRect windowPos;
	GetWindowRect(&windowPos);

	_statusChangeMenu.GetSubMenu(0).TrackPopupMenu(
		TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD,
		windowPos.left + 6,
		windowPos.bottom - 7,
		*this);

	return TBDDRET_DEFAULT;
}


void ContactListWindow::OnSize(UINT, CSize size)
{
	_ctrl.MoveWindow(0, 0, size.cx, size.cy/* - 20*/);
	
	TBBUTTONINFO info;
	info.cbSize = sizeof info;
	info.dwMask = TBIF_BYINDEX | TBIF_SIZE;
	info.cx = static_cast<WORD>(size.cx);
	_toolBar.SetButtonInfo(0, &info);
	_toolBar.AutoSize();

	// Pass on to status bar.
	SetMsgHandled(FALSE);	
}

void ContactListWindow::SetAlwaysOnTop(bool top)
{
	CMenuHandle menu(GetMenu());

	DWORD result = menu.CheckMenuItem(ID_VIEW_ALWAYSONTOP, top ? MF_CHECKED : MF_UNCHECKED);
	Assert(result != -1);
	Ignore(result);

	SetWindowPos(top ? HWND_TOPMOST : HWND_NOTOPMOST, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED); 
	_topMost = top;
}

void ContactListWindow::SetOpacity(int percent)
{
	if(_setLayeredAttributes == 0)
		return;

	_opacity = percent;
	if(_opacity == 100)
		ModifyStyleEx(WS_EX_LAYERED, 0, SWP_FRAMECHANGED);
	else
	{
		ModifyStyleEx(0, WS_EX_LAYERED, SWP_FRAMECHANGED);

		unsigned char opacity = static_cast<char>(255 * (static_cast<float>(_opacity) / 100.0f));
		_setLayeredAttributes(*this, RGB(0, 0, 0), opacity, LWA_ALPHA);
	}
}