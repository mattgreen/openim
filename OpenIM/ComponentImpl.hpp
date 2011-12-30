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

#ifndef OPENIM_COMPONENTIMPL_HPP
#define OPENIM_COMPONENTIMPL_HPP

#include "OpenIM.hpp"

#include "Component.hpp"
#include "ComponentID.hpp"
#include "ComponentManager.hpp"
#include "Events/EventBindings.hpp"

// Base class for all OpenIM subsystems.
// 
template<ComponentID CID>
class ComponentImpl :
	public Component
{
public:
	static const ComponentID ID = CID;

public:
	virtual ComponentID GetID()
	{
		return ID;
	}
};


#endif