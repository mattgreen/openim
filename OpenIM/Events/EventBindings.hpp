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

#ifndef OPENIM_EVENTS_EVENTBINDINGS_HPP
#define OPENIM_EVENTS_EVENTBINDINGS_HPP

#include "../OpenIM.hpp"

#pragma warning(push)
#pragma warning(disable:4702)
#include <algorithm>
#include <vector>
#pragma warning(pop)

#include "EventBinding.hpp"
#include "EventHandler.hpp"
#include "EventHandlerPriority.hpp"

class EventBindings
{
public:
	~EventBindings()
	{
		for(Bindings::iterator i = _bindings.begin(); i != _bindings.end(); i++)
			i->Unbind();
		_bindings.clear();
	}

	template<typename Event>
	void Add(void (*function)(Event&), EventHandlerPriority priority = Normal)
	{
		_bindings.push_back(Event::Bind(function, priority));
	}

	template<typename Event, typename Class>
	void Add(Class* instance, void (Class::* function)(Event&), EventHandlerPriority priority = Normal)
	{
		_bindings.push_back(Event::Bind(instance, function, priority));
	}

	template<typename Event>
	void Remove(void (* function)(Event&))
	{
		EventBinding binding(Event::GetName(), EventHandler(function, Normal));
		_bindings.erase(std::find(_bindings.begin(), _bindings.end(), binding));
	}

	template<typename Event, typename Class>
	void Remove(Class* instance, void (Class::* function)(Event&))
	{
		EventBinding binding(Event::GetName(), instance, function);
		_bindings.erase(std::find(_bindings.begin(), _bindings.end(), binding)); 
	}


private:
	typedef std::vector<EventBinding> Bindings;

	Bindings _bindings;
};


#endif