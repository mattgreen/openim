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

#ifndef OPENIM_COMPONENTS_EVENTMANAGER_EVENTMANAGER_HPP
#define OPENIM_COMPONENTS_EVENTMANAGER_EVENTMANAGER_HPP

#pragma warning(push)
#pragma warning(disable:4702)
#include <vector>
#include <hash_map>
#include <set>
#pragma warning(pop)

#include "../../Events/EventHandler.hpp"
#include "../../Interfaces/IEventManager.hpp"

#include "BindingDelta.hpp"
#include "EventHandlerChain.hpp"

class EventManager :
	public IEventManager
{
public:
	EventManager();
	~EventManager();

public:
	virtual void Bind(const wchar_t* name, const EventHandler& handler);
	virtual void Invoke(const wchar_t* name, void* data, void (*invoker)(void*, const EventHandler&));
	virtual void Unbind(const wchar_t* name, const EventHandler& handler);

private:
	typedef stdext::hash_map<unsigned int, EventHandlerChain*>	EventHandlers;
	typedef std::set<BindingDelta> BindingDeltas;

	EventHandlers	_handlers;
	BindingDeltas	_deltas;
};

#endif