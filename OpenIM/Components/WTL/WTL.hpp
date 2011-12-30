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

#ifndef OPENIM_COMPONENTS_WTL_WTL_HPP
#define OPENIM_COMPONENTS_WTL_WTL_HPP

#include "../../OpenIM.hpp"

#define _WTL_NO_CSTRING
#define _WTL_NEW_PAGE_NOTIFY_HANDLERS
#include <atlbase.h>
#include <atlapp.h>

extern "C" OPENIM_API CAppModule _Module;

#include <atlwin.h>
#include <atldlgs.h>
#include <atlframe.h>
#include <atlmisc.h>
#include <atlctrls.h>
#include <atlctrlx.h>
#include <atlmisc.h>
#include <atlcrack.h>
#include <atlddx.h>

#define DDX_COMBO_TEXT(nID, var) \
		if(nCtlID == (UINT)-1 || nCtlID == nID) \
		{ \
			if(!DDX_Combo_Text(nID, var, sizeof(var), bSaveAndValidate)) \
				return FALSE; \
		}

template<typename T>
class WinDataExchangeEx :
	public CWinDataExchange<T>
{
public:
	BOOL DDX_Combo_Text(UINT nID, LPTSTR lpstrText, int cbSize, BOOL bSave, BOOL bValidate = FALSE, int nLength = 0)
	{
		T* pT = static_cast<T*>(this);
		BOOL bSuccess = TRUE;

		if(bSave)
		{
			HWND hWndCtrl = pT->GetDlgItem(nID);
			LRESULT index = SendMessage(hWndCtrl, CB_GETCURSEL, 0, 0);
			LRESULT len = SendMessage(hWndCtrl, CB_GETLBTEXTLEN, index, 0);
			if(len < static_cast<LRESULT>(cbSize / sizeof(TCHAR)))
				SendMessage(hWndCtrl, CB_GETLBTEXT, index, reinterpret_cast<LPARAM>(lpstrText));
			else
				bSuccess = FALSE;
		}
		else
		{
			ATLASSERT(!bValidate || lstrlen(lpstrText) <= nLength);
			LRESULT index = SendMessage(pT->GetDlgItem(nID), CB_FINDSTRINGEXACT, (WPARAM)0, (LPARAM)lpstrText);
			if(index != CB_ERR)
				SendMessage(pT->GetDlgItem(nID), CB_SETCURSEL, index, 0);
			else
				bSuccess = FALSE;
		}

		if(! bSuccess)
		{
			pT->OnDataExchangeError(nID, bSave);
		}
		else if(bSave && bValidate)   // validation
		{
			ATLASSERT(nLength > 0);
			if(lstrlen(lpstrText) > nLength)
			{
				_XData data = { ddxDataText };
				data.textData.nLength = lstrlen(lpstrText);
				data.textData.nMaxLength = nLength;
				pT->OnDataValidateError(nID, bSave, data);
				bSuccess = FALSE;
			}
		}
		return bSuccess;
	}
};

#endif