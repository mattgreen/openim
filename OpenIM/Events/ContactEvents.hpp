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

#ifndef OPENIM_EVENTS_CONTACTEVENTS_HPP
#define OPENIM_EVENTS_CONTACTEVENTS_HPP

#include "../OpenIM.hpp"

#include "../Account.hpp"
#include "../Contact.hpp"
#include "../Group.hpp"

#include "Event.hpp"

struct ContactCreateEvent :
	public Event<ContactCreateEvent>
{
	static const wchar_t* GetName() { return L"ContactCreateEvent"; }
	
	Contact*	Contact;
	Group*		Group;
	int			Position;
};

struct ContactDestroyEvent :
	public Event<ContactDestroyEvent>
{
	static const wchar_t* GetName() { return L"ContactDestroyEvent"; }
	
	Contact*	Contact;
	Group*		Group;
};

struct ContactMovedEvent :
	public Event<ContactMovedEvent>
{
	static const wchar_t* GetName() { return L"ContactMovedEvent"; }

	Contact*	Contact;
	Group*		Source;
	Group*		Destination;
	int			Position;
};

struct ContactSignOffEvent :
	public Event<ContactSignOffEvent>
{
	static const wchar_t* GetName() { return L"ContactSignOffEvent"; }
	
	Contact*	Contact;
};

struct ContactSignOnEvent :
	public Event<ContactSignOnEvent>
{
	static const wchar_t* GetName() { return L"ContactSignOnEvent"; }
	
	Contact*	Contact;
};

struct ContactStatusChangeEvent :
	public Event<ContactStatusChangeEvent>
{
	static const wchar_t* GetName() { return L"ContactStatusChangeEvent"; }
	
	Contact*	Contact;
};

struct ContactUpdateEvent :
	public Event<ContactUpdateEvent>
{
	static const wchar_t* GetName() { return L"ContactStatusChangeEvent"; }
	
	Contact*	Contact;
};

#endif