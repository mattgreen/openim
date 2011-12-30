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

#ifndef OPENIM_COMPONENTS_LOG_LOGWINDOW_HPP
#define OPENIM_COMPONENTS_LOG_LOGWINDOW_HPP

#include "../../Resource.h"
#include "../../WTL.hpp"

class LogWindow :
	public CDialogImpl<LogWindow>,
	public CDialogResize<LogWindow>
{
public:
	LogWindow();
	~LogWindow();

public:
	enum
	{
		IDD = IDD_LOGDISPLAY
	};

	BEGIN_MSG_MAP(LogWindow)
		MSG_WM_INITDIALOG(OnInitDialog)
		MSG_WM_CLOSE(OnClose)
		MSG_WM_SIZE(OnSize)
		CHAIN_MSG_MAP(CDialogResize<LogWindow>)
	END_MSG_MAP()

    BEGIN_DLGRESIZE_MAP(LogWindow)
		DLGRESIZE_CONTROL(IDC_EVENTS, DLSZ_SIZE_X | DLSZ_SIZE_Y)
    END_DLGRESIZE_MAP()

private:
	void	OnClose();
	LRESULT OnInitDialog(HWND, LPARAM);
	void	OnSize(UINT, CSize);

public:
	void	WriteInfo(const wchar_t* source, const wchar_t* format, va_list args);
	void	WriteWarning(const wchar_t* source, const wchar_t* format, va_list args);
	void	WriteError(const wchar_t* source, const wchar_t* format, va_list args);

private:
	void	Write(const wchar_t* source, const wchar_t* type, int image, const wchar_t* format, va_list args);

private:
	CListViewCtrl	_events;
	CImageList		_imageList;
	int				_infoIndex;
	int				_warningIndex;
	int				_errorIndex;
};


#endif