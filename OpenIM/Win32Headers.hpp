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

#ifndef OPENIM_WIN32HEADERS_HPP
#define OPENIM_WIN32HEADERS_HPP

#define _WIN32_WINNT 0x500
#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN

#ifndef STRICT
#define STRICT
#endif

#include <winsock2.h>
#include <windows.h>
#include <shellapi.h>
#include <objidl.h>
#include <commctrl.h>
#include <dbghelp.h>

#endif