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

#include "../OpenIM/OpenIM.hpp"
#include "../OpenIM/Win32Headers.hpp"

#include <shlobj.h>
#include <commctrl.h>
#include <objbase.h>

typedef void (* CoreFunction)(const wchar_t*);

void FixCurrentDirectory()
{
	wchar_t applicationDir[MAX_PATH];
	GetModuleFileName(GetModuleHandle(0), applicationDir, MAX_PATH);
	wchar_t* endPath = wcsrchr(applicationDir, L'\\');
	if(endPath)
		*endPath = 0;
	SetCurrentDirectory(applicationDir);
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, char*, int)
{
	CoInitialize(0);
	InitCommonControls();
	FixCurrentDirectory();

	// Form profile directory path.
	wchar_t profileDir[MAX_PATH];
	SHGetSpecialFolderPath(0, profileDir, CSIDL_APPDATA, TRUE);
	wsprintf(profileDir, L"%s\\OpenIM\\", profileDir);

	HMODULE core = LoadLibrary(L"OpenIM.dll");
	if(core != 0)
	{
		CoreFunction start = reinterpret_cast<CoreFunction>(GetProcAddress(core, "Startup"));
		CoreFunction stop = reinterpret_cast<CoreFunction>(GetProcAddress(core, "Shutdown"));
		CoreFunction run = reinterpret_cast<CoreFunction>(GetProcAddress(core, "Run"));

		if(start != 0 && stop != 0 && run != 0)
		{
			start(profileDir);
			run(profileDir);
			stop(profileDir);
		}

		FreeLibrary(core);
	}
	else
	{
		if(GetLastError() == ERROR_MOD_NOT_FOUND)
		{
			MessageBox(0,
				L"Problem: Could not load OpenIM.dll.\n\n"
				L"Resolution: Repair the installation using the Add/Remove Programs applet, and relaunch OpenIM.",
				L"OpenIM startup error",
				MB_ICONERROR);
		}
	}

	CoUninitialize();

	return 0;
}