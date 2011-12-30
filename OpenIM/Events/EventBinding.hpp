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

#ifndef OPENIM_EVENTS_EVENTBINDING_HPP
#define OPENIM_EVENTS_EVENTBINDING_HPP

#include "../OpenIM.hpp"

#include "EventHandler.hpp"
#include "EventExports.hpp"

class EventBinding
{
public:
	EventBinding(const wchar_t* event, EventHandler target) :
		_event(event),
		_target(target)
	{
	}

public:
	bool operator==(const EventBinding& r) const
	{
		return _target == r._target;
	}

	void Unbind()
	{
		EventUnbind(_event, _target);
	}

private:
	const wchar_t*	_event;
	EventHandler	_target;
};


#endif