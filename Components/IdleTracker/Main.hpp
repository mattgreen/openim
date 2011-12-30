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

#ifndef IDLETRACKER_MAIN_HPP
#define IDLETRACKER_MAIN_HPP

#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#define WM_STARTIDLE (WM_USER + 8)
#define WM_ENDIDLE (WM_USER + 9)

const int	NotificationThreshold = 1000 * 60 * 5; // 5 minutes

extern "C"
{
	__declspec(dllexport) long GetLastEventTime();
	__declspec(dllexport) void Hook(HWND);
	__declspec(dllexport) void Unhook();
}

#endif