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

#include "Spry.hpp"
#include "Win32Headers.hpp"

typedef API_VERSION*	(WINAPI *GetDBGAPIVersion)();
typedef BOOL			(WINAPI *MiniDumpWriteDumpFn)(HANDLE, DWORD, HANDLE, MINIDUMP_TYPE, PMINIDUMP_EXCEPTION_INFORMATION, PMINIDUMP_USER_STREAM_INFORMATION, PMINIDUMP_CALLBACK_INFORMATION);

bool CreateMinidump(const wchar_t* filename, EXCEPTION_POINTERS* exception)
{
	if(filename == 0)
		return false;
	if(exception == 0)
		return false;

	// Load the debug helper library, if present.
	HMODULE mod = LoadLibrary(L"dbghelp.dll");
	if(mod == 0)
		return false;

	Spry::DynamicLibrary debugHelper(mod);

	// Create the minidump and go about our way.
    MiniDumpWriteDumpFn minidump = debugHelper.GetSymbol<MiniDumpWriteDumpFn>("MiniDumpWriteDump");
	if(minidump == 0)
		return false;

	MINIDUMP_EXCEPTION_INFORMATION exceptionInfo;
	exceptionInfo.ThreadId = GetCurrentThreadId();
	exceptionInfo.ExceptionPointers = exception;
	exceptionInfo.ClientPointers = FALSE;

	HANDLE file = CreateFile(filename, GENERIC_READ | GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if(file == INVALID_HANDLE_VALUE)
		return false;

	BOOL result = minidump(GetCurrentProcess(),
			GetCurrentProcessId(),
			file,
			MiniDumpFilterMemory,
			&exceptionInfo,
			0,
			0);

	CloseHandle(file);
	file = 0;

	return result == TRUE;		
}

LONG WINAPI OnUnhandledException(EXCEPTION_POINTERS* exception)
{
	if(exception == 0)
		return EXCEPTION_EXECUTE_HANDLER;

	SYSTEMTIME date;
	GetLocalTime(&date);

	wchar_t filename[MAX_PATH] = { 0, };
	wsprintf(filename, L".\\OpenIM.minidump.%d-%d-%d.%d-%d.dmp",
		date.wYear,
		date.wMonth,
		date.wDay,
		date.wHour,
		date.wMinute);

	CreateMinidump(filename, exception);
	
	return EXCEPTION_EXECUTE_HANDLER;
}

void InstallCrashHandler()
{
	SetUnhandledExceptionFilter(OnUnhandledException);
}