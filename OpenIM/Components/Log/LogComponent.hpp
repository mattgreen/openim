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

#ifndef OPENIM_COMPONENTS_LOGCOMPONENT_HPP
#define OPENIM_COMPONENTS_LOGCOMPONENT_HPP

#include "../../Interfaces/ILog.hpp"
#include "LogWindow.hpp"

class LogComponent :
	public ILog
	
{
public:
	LogComponent();
	~LogComponent();

public:
	virtual void WriteError(const wchar_t* source, const wchar_t* format, va_list args);
	virtual void WriteInfo(const wchar_t* source, const wchar_t* format, va_list args);
	virtual void WriteWarning(const wchar_t* source, const wchar_t* format, va_list args);

public:
	LogWindow _window;
};


#endif