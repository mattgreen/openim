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

#include "Main.hpp"

#pragma data_seg(".Idling")
volatile HWND		Window = 0;
volatile POINT		Cursor = { -1, -1 };
volatile HHOOK		KeyboardHookHandle = 0;
volatile HHOOK		MouseHookHandle = 0;
volatile long		LastEventTime = 0;
#pragma data_seg()
#pragma comment(linker, "/section:.Idling,rws")

HINSTANCE	dllInstance = 0;

LRESULT CALLBACK KeyboardHook(int, WPARAM, LPARAM);
LRESULT CALLBACK MouseHook(int, WPARAM, LPARAM);

BOOL WINAPI DllMain(HINSTANCE instance, DWORD reason, LPVOID)
{
	switch(reason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(instance);

		dllInstance = instance;
		break;
	}

	return TRUE;
}

LRESULT CALLBACK KeyboardHook(int code, WPARAM wParam, LPARAM lParam)
{
	if(code == HC_ACTION)
	{
		long now = GetTickCount();

		if(now - LastEventTime >= NotificationThreshold)
			PostMessage(Window, WM_ENDIDLE, 0, 0);

		InterlockedExchange(&LastEventTime, now);
	}

	return CallNextHookEx(KeyboardHookHandle, code, wParam, lParam);
}

LRESULT CALLBACK MouseHook(int code, WPARAM wParam, LPARAM lParam)
{
	if(code == HC_ACTION)
	{
		bool moved = false;

		POINT pt;
		GetCursorPos(&pt);

		if(pt.x != Cursor.x)
		{
			InterlockedExchange(&Cursor.x, pt.x);
			moved = true;
		}

		if(pt.y != Cursor.y)
		{
			InterlockedExchange(&Cursor.y, pt.y);
			moved = true;
		}

		if(moved)
		{
			long now = GetTickCount();

			if(now - LastEventTime >= NotificationThreshold)
				PostMessage(Window, WM_ENDIDLE, 0, 0);

			InterlockedExchange(&LastEventTime, now);
		}
	}

	return CallNextHookEx(MouseHookHandle, code, wParam, lParam);
}

__declspec(dllexport) long GetLastEventTime()
{
	return LastEventTime;
}

__declspec(dllexport) void Hook(HWND target)
{
	Window = target;
	LastEventTime = GetTickCount();

	KeyboardHookHandle = SetWindowsHookEx(WH_KEYBOARD, KeyboardHook, dllInstance, 0);
	MouseHookHandle = SetWindowsHookEx(WH_MOUSE, MouseHook, dllInstance, 0);

}

__declspec(dllexport) void Unhook()
{
	if(KeyboardHookHandle)
	{
		UnhookWindowsHookEx(KeyboardHookHandle);
		KeyboardHookHandle = 0;
	}

	if(MouseHookHandle)
	{
		UnhookWindowsHookEx(MouseHookHandle);
		MouseHookHandle = 0;
	}
}