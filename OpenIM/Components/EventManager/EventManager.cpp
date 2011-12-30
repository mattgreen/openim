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

#include "EventManager.hpp"

unsigned int Hash(const wchar_t* str, size_t len)
{
	unsigned int hash = 0;
	unsigned int x  = 0;

	for(size_t i = 0; i < len; str++, i++)
	{
		hash = (hash << 4) + (*str);
		if((x = hash & 0xF0000000L) != 0)
		{
			hash ^= (x >> 24);
			hash &= ~x;
		}
	}

	return (hash & 0x7FFFFFFF);
}

EventManager::EventManager()
{
}

EventManager::~EventManager()
{
	for(EventHandlers::iterator i = _handlers.begin(); i != _handlers.end(); ++i)
	{
		delete i->second;
	}
}

void EventManager::Bind(const wchar_t* name, const EventHandler& handler)
{
	if(name == 0)
		return;

	unsigned int hash = Hash(name, wcslen(name));

	BindingDeltas::iterator alreadyRaised = _deltas.find(hash);
	if(alreadyRaised == _deltas.end())
	{
		EventHandlers::iterator i = _handlers.find(hash);
		if(i == _handlers.end())
		{
			_handlers[hash] = new EventHandlerChain();
			i = _handlers.find(hash);	
		}

		i->second->push_back(handler);
	}
	else
	{
		if(alreadyRaised->NewlyBound == 0)
			alreadyRaised->NewlyBound = new EventHandlerChain();
		alreadyRaised->NewlyBound->push_back(handler);
	}
}

void EventManager::Invoke(const wchar_t* name, void* data, void (*invoker)(void*, const EventHandler&))
{
	if(name == 0 || data == 0)
		return;

	// Prevent infinitely recursive events.
	unsigned int hash = Hash(name, wcslen(name));
	if(_deltas.find(hash) != _deltas.end())
		return;

	// If we don't have any handlers listed, leave.
	EventHandlers::iterator handlers = _handlers.find(hash);
	if(handlers == _handlers.end())
		return;

	// Add this event to the list of currently raised events.
	BindingDeltas::iterator delta = _deltas.insert(_deltas.begin(), hash);

	// Invoke handlers
	// TODO: Find some way to sink only C++ exceptions (not SEH)
	for(EventHandlerChain::iterator i = handlers->second->begin(); i != handlers->second->end(); ++i)
	{
		invoker(data, *i);
	}

	// Cleanup
	if(delta->NewlyBound != 0)
	{
		for(EventHandlerChain::iterator i = delta->NewlyBound->begin(); i != delta->NewlyBound->end(); ++i)
			Bind(name, *i);
	}
	delete delta->NewlyBound;

	if(delta->NewlyUnbound != 0)
	{
		for(EventHandlerChain::iterator i = delta->NewlyUnbound->begin(); i != delta->NewlyUnbound->end(); ++i)
			Unbind(name, *i);
	}
	delete delta->NewlyUnbound;
	_deltas.erase(delta);
}

void EventManager::Unbind(const wchar_t* name, const EventHandler& handler)
{
	if(name == 0)
		return;
	unsigned int hash = Hash(name, wcslen(name));

	BindingDeltas::iterator alreadyRaised = _deltas.find(hash);
	if(alreadyRaised == _deltas.end())
	{
		EventHandlers::iterator i = _handlers.find(hash);
		if(i != _handlers.end())
		{
			for(EventHandlerChain::iterator j = i->second->begin(); j != i->second->end(); ++j)
			{
				if((*j) == (handler))
				{
					i->second->erase(j);
					break;
				}
			}
		}
	}
	else
	{
		if(alreadyRaised->NewlyUnbound == 0)
			alreadyRaised->NewlyUnbound = new EventHandlerChain();
		alreadyRaised->NewlyUnbound->push_back(handler);
	}
}
