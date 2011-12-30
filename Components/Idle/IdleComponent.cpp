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

#include "IdleComponent.hpp"

#pragma warning(disable:4702)
#include <cmath>
#include <set>

#include "../../OpenIM/Account.hpp"
#include "../IdleTracker/Main.hpp"

typedef std::set<Account*> Accounts;

HINSTANCE		Module;

Accounts		_accounts;
bool			_idling;
unsigned int	_idleStart = 0;

static LRESULT CALLBACK IdleWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_STARTIDLE:
		if(! _idling)
		{
			_idling = true;
			_idleStart = GetTickCount();

			/*
			for(Accounts::iterator i = accounts.begin(); i != accounts.end(); ++i)
				if((*i)->Preferences->ReportIdleTime)
					(*i)->SetIdleTime(NotificationThreshold / (60 * 1000));
					*/
		}
		break;

	case WM_ENDIDLE:
		if(_idling)
		{
			_idling = false;

			/*for(Accounts::iterator i = accounts.begin(); i != accounts.end(); ++i)
				if((*i)->Preferences->ReportIdleTime)
					(*i)->SetIdleTime(0);*/
		}
		break;

	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}


IdleComponent::IdleComponent()
{
	_bindings.Add(this, &IdleComponent::OnAccountConnect);
	_bindings.Add(this, &IdleComponent::OnAccountDisconnect);
	_bindings.Add(this, &IdleComponent::OnTimerEvent);

	WNDCLASSEX wndClass;
	ZeroMemory(&wndClass, sizeof wndClass);
	wndClass.cbClsExtra = 0;
	wndClass.cbSize = sizeof wndClass;
	wndClass.cbWndExtra = 0;
	wndClass.hbrBackground = 0;
	wndClass.hCursor = LoadCursor(0, IDC_ARROW);
	wndClass.hIcon = LoadIcon(0, IDI_APPLICATION);
	wndClass.hInstance = Module;
	wndClass.lpfnWndProc = IdleWndProc;
	wndClass.lpszClassName = L"IdleTracker";
	wndClass.style = 0;

	RegisterClassEx(&wndClass);

	_wnd = CreateWindow(L"IdleTracker", L"IdleTracker", 0, 0, 0, 0, 0, GetDesktopWindow(), 0, Module, 0);
	Hook(_wnd);
}

IdleComponent::~IdleComponent()
{
	Unhook();

	DestroyWindow(_wnd);
}

void IdleComponent::OnAccountConnect(AccountConnectEvent& e)
{
	if(e.Progress == 100)
	{
		_accounts.insert(e.Account);

		// Update idle time if this account is done connecting.
		/*if(idling && e.Account->Preferences->ReportIdleTime)
		{
			unsigned int now = GetTickCount();
			int minutes = static_cast<int>(floor(static_cast<float>(now - idleStart) / static_cast<float>(60 * 1000)));
			e.Account->SetIdleTime(minutes);
		}*/
	}
}

void IdleComponent::OnAccountDisconnect(AccountDisconnectEvent& e)
{
	_accounts.erase(e.Account);
}

void IdleComponent::OnTimerEvent(TimerEvent& e)
{
	if(e.Seconds == 0)
	{
		if(! _idling)
		{
			int now = GetTickCount();
			if(now - GetLastEventTime() >= NotificationThreshold)
			{
				PostMessage(_wnd, WM_STARTIDLE, 0, 0);
			}
		}
	}
}
