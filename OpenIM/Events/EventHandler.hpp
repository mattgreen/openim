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

#ifndef OPENIM_EVENTS_EVENTHANDLER_HPP
#define OPENIM_EVENTS_EVENTHANDLER_HPP

#include "Delegate.hpp"

struct EventHandler
{
	EventHandler() :
		Priority()
	{
	}

	template<typename Event>
	explicit EventHandler(void (* handler)(Event&), int priority) :
		Priority(priority)
	{
		Delegate<void (Event&)> d(handler);
		Target = d.GetMemento();
	}

	template<typename Class, typename Event>
	EventHandler(Class* instance, void (Class::* handler)(Event&), int priority) :
		Priority(priority)
	{
		Delegate<void (Event&)> d(instance, handler);
		Target = d.GetMemento();
	}

	bool operator<(const EventHandler& other) const
	{
		return Priority < other.Priority;
	}

	bool operator==(const EventHandler& other) const
	{
		return Target.IsEqual(other.Target);
	}

	DelegateMemento	Target;
	int				Priority;
};


#endif