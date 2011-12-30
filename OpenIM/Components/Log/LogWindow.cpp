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

#include "LogWindow.hpp"

#include <ctime>

LogWindow::LogWindow() :
	_infoIndex(),
	_warningIndex(),
	_errorIndex()
{
	_imageList.Create(16, 16, ILC_COLOR32 | ILC_MASK, 3, 0);

	_infoIndex = _imageList.AddIcon(LoadIcon(0, IDI_INFORMATION));
	_warningIndex = _imageList.AddIcon(LoadIcon(0, IDI_WARNING));
	_errorIndex = _imageList.AddIcon(LoadIcon(0, IDI_ERROR));
}

LogWindow::~LogWindow()
{
	_imageList.Destroy();
}

void LogWindow::OnClose()
{
	ShowWindow(SW_HIDE);
}

LRESULT LogWindow::OnInitDialog(HWND, LPARAM)
{
	DlgResize_Init(false);

	_events.Attach(GetDlgItem(IDC_EVENTS));

	_events.AddColumn(L"Type", 1);
	_events.AddColumn(L"Timestamp", 2);
	_events.AddColumn(L"Source", 3);
	_events.AddColumn(L"Message", 4);
	
	_events.SetImageList(_imageList, LVSIL_SMALL);
	_events.SetExtendedListViewStyle(LVS_EX_FULLROWSELECT);

	_events.SetColumnWidth(0, 75);
	_events.SetColumnWidth(1, 75);
	_events.SetColumnWidth(2, 100);
	_events.SetColumnWidth(3, LVSCW_AUTOSIZE_USEHEADER);

	WriteInfo(L"System", TEXT(OPENIM_VERSION_STRING) L" (built: " TEXT(OPENIM_BUILD_TIMESTAMP) L")", 0);

	return 0;
}

void LogWindow::OnSize(UINT, CSize)
{
	_events.SetColumnWidth(3, LVSCW_AUTOSIZE_USEHEADER);
	_events.SetColumnWidth(3, _events.GetColumnWidth(3) - 5);
	
	SetMsgHandled(FALSE);
}

void LogWindow::Write(const wchar_t* source, const wchar_t* type, int image, const wchar_t* format, va_list args)
{
	int item = _events.InsertItem(_events.GetItemCount(), type, image);

	wchar_t timeBuffer[32] = { 0, };
	time_t nowSeconds = time(0);
	tm* now = localtime(&nowSeconds);
	wcsftime(timeBuffer, sizeof(timeBuffer) / sizeof(wchar_t), L"%H:%M:%S", now);
	_events.AddItem(item, 1, timeBuffer);

	_events.AddItem(item, 2, source);

	wchar_t buffer[1024] = { 0, };

	_vsnwprintf(buffer, sizeof(buffer) / sizeof(wchar_t), format, args);

	_events.AddItem(item, 3, buffer);
	
	int height = 0;
	int width = 0;
	_imageList.GetIconSize(width, height);
	SIZE bottom = { 0, _events.GetItemCount() * height };
	_events.Scroll(bottom);
}

void LogWindow::WriteError(const wchar_t* source, const wchar_t* format, va_list args)
{
	Write(source, L"Error", _errorIndex, format, args);
}

void LogWindow::WriteInfo(const wchar_t* source, const wchar_t* format, va_list args)
{
	Write(source, L"Info", _infoIndex, format, args);
}

void LogWindow::WriteWarning(const wchar_t* source, const wchar_t* format, va_list args)
{
	Write(source, L"Warning", _warningIndex, format, args);
}