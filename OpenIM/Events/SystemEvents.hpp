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

#ifndef OPENIM_EVENTS_SYSTEMEVENTS_HPP
#define OPENIM_EVENTS_SYSTEMEVENTS_HPP

#include "../OpenIM.hpp"

#include "Event.hpp"

struct LoadUserProfileEvent :
	public Event<LoadUserProfileEvent>
{
	LoadUserProfileEvent() :
		ProfileDirectory()
	{
	}

	static const wchar_t* GetName() { return L"LoadUserProfileEvent"; }

	const wchar_t*	ProfileDirectory;
};

struct RequestShutdownEvent :
	public Event<RequestShutdownEvent>
{
	RequestShutdownEvent() :
		Cancel(false)
	{
	}

	static const wchar_t* GetName() { return L"RequestShutdownEvent"; }
	
	bool Cancel;
};

struct ShutdownEvent :
	public Event<ShutdownEvent>
{
	static const wchar_t* GetName() { return L"ShutdownEvent"; }
};

struct StartupEvent :
	public Event<StartupEvent>
{
	static const wchar_t* GetName() { return L"StartupEvent"; }
};

struct UnloadUserProfileEvent :
	public Event<UnloadUserProfileEvent>
{
	UnloadUserProfileEvent() :
		ProfileDirectory()
	{
	}

	static const wchar_t* GetName() { return L"UnloadUserProfileEvent"; }

	const wchar_t*	ProfileDirectory;
};

#endif