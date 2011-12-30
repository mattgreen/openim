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

#include "TrayComponent.hpp"

#include "../../OpenIM/Win32Headers.hpp"

#include "../../OpenIM/Interfaces/IAccountManager.hpp"
#include "../../OpenIM/Interfaces/IContactListWindow.hpp"
//#include "../../OpenIM/Interfaces/PreferencesManager.hpp"

#include "Main.hpp"
#include "resource.h"

namespace
{
	const int		WM_TRAYICONNOTIFY = WM_USER + 25;

	HMENU			contextMenu = 0;
	HWND			window = 0;
	NOTIFYICONDATA	nid;
	UINT			taskbarCreated = 0;

	void AddIcon()
	{
		nid.cbSize = sizeof nid;
		wcscpy(nid.szTip, L"OpenIM");
		nid.hIcon = LoadIcon(0, IDI_APPLICATION);
		nid.hWnd = window;
		nid.uCallbackMessage = WM_TRAYICONNOTIFY;
		nid.uID = 0;
		nid.uFlags = NIF_TIP | NIF_ICON | NIF_MESSAGE;
		Shell_NotifyIcon(NIM_ADD, &nid);
	}

	LRESULT CALLBACK TrayWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch(msg)
		{
		case WM_CREATE:
			taskbarCreated = RegisterWindowMessage(L"TaskbarCreated");
			break;

		case WM_TRAYICONNOTIFY:
			if(lParam == WM_RBUTTONDOWN)
			{
				POINT pt;
				GetCursorPos(&pt);

				SetForegroundWindow(window);
				int item = static_cast<int>(TrackPopupMenu(GetSubMenu(contextMenu, 0),
					TPM_RIGHTALIGN | TPM_BOTTOMALIGN | TPM_NONOTIFY | TPM_RETURNCMD | TPM_RIGHTBUTTON,
					pt.x, pt.y, 0, window, 0));
				PostMessage(window, WM_NULL, 0, 0);

				switch(item)
				{
				case ID_TRAYMENU_SHOWCONTACTLIST:
					PostMessage(hwnd, WM_TRAYICONNOTIFY, 0, WM_LBUTTONDBLCLK);
					break;

				case ID_TRAYMENU_ACCOUNTS:
					if(IAccountManager* mgr = ComponentManager::Get<IAccountManager>())
						mgr->Show();
					
					break;
				
				case ID_TRAYMENU_PREFERENCES:
					{
//						if(PreferencesManager* prefsMgr = ComponentManager::Find<PreferencesManager>())
//						{
//							prefsMgr->Show();
//						}
					}
					break;

				case ID_TRAYMENU_EXIT:
					{
						RequestShutdownEvent e;
						e.Invoke();

						if(! e.Cancel)
							PostQuitMessage(0);
					}
					break;
				}
			}
			else if(lParam == WM_LBUTTONDBLCLK)
			{
				if(IContactListWindow* window = ComponentManager::Get<IContactListWindow>())
					window->Show();
				/*if(ContactList* contactList = ComponentManager::Find<ContactList>())
				{
					ShowWindow(contactList->GetHWND(), SW_RESTORE);
					SetForegroundWindow(contactList->GetHWND());
				}*/
			}

			break;

		default:
			if(msg == taskbarCreated)
				AddIcon();

			return DefWindowProc(hwnd, msg, wParam, lParam);
		}
		return 0;
	}
}

TrayComponent::TrayComponent()
{
	_bindings.Add<StartupEvent>(this, &TrayComponent::OnStartup);
	_bindings.Add<ShutdownEvent>(this, &TrayComponent::OnShutdown);
}

void TrayComponent::OnStartup(StartupEvent&)
{
	// create window
	WNDCLASSEX wc;
	wc.cbSize        = sizeof(WNDCLASSEX);
	wc.style         = 0;
	wc.lpfnWndProc   = TrayWindowProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = 0;
	wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wc.lpszMenuName  = NULL;
	wc.lpszClassName = L"OpenIMTrayWindow";
	wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassEx(&wc);

	window = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		L"OpenIMTrayWindow",
		L"OpenIM Tray Notifications",
		WS_OVERLAPPEDWINDOW,
		0, 0, 0, 0,
		NULL, NULL, NULL, NULL);

	if(window == NULL)
	{
		MessageBox(NULL, L"Window Creation Failed!", L"Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return;
	}

	// load menu
	contextMenu = LoadMenu(dllInstance, MAKEINTRESOURCE(IDR_TRAYMENU));
	SetMenuDefaultItem(GetSubMenu(contextMenu, 0), ID_TRAYMENU_SHOWCONTACTLIST, FALSE);

	AddIcon();
}

void TrayComponent::OnShutdown(ShutdownEvent&)
{
	Shell_NotifyIcon(NIM_DELETE, &nid);

	DestroyMenu(contextMenu);
	DestroyWindow(window);
}
