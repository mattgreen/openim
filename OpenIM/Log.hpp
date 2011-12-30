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

#ifndef OPENIM_LOG_HPP
#define OPENIM_LOG_HPP

#include <cstdarg>

#include "OpenIM.hpp"
#include "Interfaces/ILog.hpp"

class Log
{
public:
	explicit Log(const wchar_t* source) :
		_logger(),
		_source(source)
	{
	}

	void Write(const wchar_t* format, ...)
	{
		if(_logger == 0)
		{
			_logger = ComponentManager::Get<ILog>();
			Assert(_logger != 0);
			if(_logger == 0)
				return;
		}

		va_list list;
		va_start(list, format);
		_logger->WriteInfo(_source, format, list);
		va_end(list);
	}

	void WriteError(const wchar_t* format, ...)
	{
		if(_logger == 0)
		{
			_logger = ComponentManager::Get<ILog>();
			Assert(_logger != 0);
			if(_logger == 0)
				return;
		}

		va_list list;
		va_start(list, format);
		_logger->WriteError(_source, format, list);
		va_end(list);
	}

	void WriteWarning(const wchar_t* format, ...)
	{
		if(_logger == 0)
		{
			_logger = ComponentManager::Get<ILog>();
			Assert(_logger != 0);
			if(_logger == 0)
				return;
		}

		va_list list;
		va_start(list, format);
		_logger->WriteWarning(_source, format, list);
		va_end(list);
	}

private:
	ILog*			_logger;
	const wchar_t*	_source;
};


#endif