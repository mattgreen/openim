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

#ifndef OPENIM_EVENTS_EVENT_HPP
#define OPENIM_EVENTS_EVENT_HPP

#include "../OpenIM.hpp"

#include "EventBinding.hpp"
#include "EventExports.hpp"
#include "EventHandler.hpp"
#include "EventHandlerPriority.hpp"

template<typename DerivedEvent>
struct Event
{
	template<typename Class>
	static EventBinding Bind(Class* instance, void (Class::* function)(DerivedEvent&), EventHandlerPriority priority = Normal)
	{
		const wchar_t* name = DerivedEvent::GetName();
		EventHandler handler(instance, function, priority);
		
		EventBind(name, handler);

		return EventBinding(name, handler);
	}

	static EventBinding Bind(void (*function)(DerivedEvent&), EventHandlerPriority priority = Normal)
	{
		const wchar_t* name = DerivedEvent::GetName();
		EventHandler handler(function, priority);
		
		EventBind(name, handler);

		return EventBinding(name, handler);
	}

	void Invoke()
	{
		EventInvoke(DerivedEvent::GetName(), static_cast<DerivedEvent*>(this), DoInvokeHandler);
	}

	template<typename Class>
	static void Unbind(Class* instance, void (Class::* function)(DerivedEvent&))
	{
		EventUnbind(DerivedEvent::GetName(), EventHandler(instance, function));
	}

	static void Unbind(void (*function)(DerivedEvent&))
	{
		EventUnbind(DerivedEvent::GetName(), EventHandler(function));
	}

private:
	static void DoInvokeHandler(void* event, const EventHandler& handler)
	{
		Delegate<void (DerivedEvent&)> target;
		target.SetMemento(handler.Target);
		target(*static_cast<DerivedEvent*>(event));
	}

};

#endif