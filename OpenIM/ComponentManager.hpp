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

#ifndef OPENIM_COMPONENTMANAGER_HPP
#define OPENIM_COMPONENTMANAGER_HPP

#include "OpenIM.hpp"

#include "ComponentID.hpp"

class Component;

class ComponentManager
{
public:
	template<typename Component>
	static void Add()
	{
		Assert(Get<Component>() == 0);

		DoAdd(new Component());
	}

	// Returns a pointer to the component with the given interface, if one is loaded.
	// Otherwise, this method returns the null pointer.
	template<typename Component>
	static Component* Get()
	{
		return static_cast<Component*>(DoGet(Component::ID));
	}

	template<typename Component>
	static void	Remove()
	{
		Component* component = Get<Component>();
		DoRemove(component);

		delete component;
	}

public:
	static void	Load();
	static void	Unload();

private:
	OPENIM_API static void			DoAdd(Component* component);
	OPENIM_API static Component*	DoGet(ComponentID id);
	OPENIM_API static void			DoRemove(Component* component);

private:
	ComponentManager();
	~ComponentManager();
};


#endif