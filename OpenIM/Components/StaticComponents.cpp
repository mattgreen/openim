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

#include "StaticComponents.hpp"
#include "../ComponentManager.hpp"

#include "AccountManager/AccountManager.hpp"
#include "EventManager/EventManager.hpp"
#include "Log/LogComponent.hpp"
#include "ResourceManager/ResourceManager.hpp"
#include "Spry/SpryComponent.hpp"
#include "Timer/TimerComponent.hpp"
#include "WTL/WTLComponent.hpp"

void RegisterStaticComponents()
{
	ComponentManager::Add<LogComponent>();
	ComponentManager::Add<EventManager>();
	ComponentManager::Add<SpryComponent>();
	ComponentManager::Add<TimerComponent>();
	ComponentManager::Add<AccountManager>();
	ComponentManager::Add<ResourceManager>();
	ComponentManager::Add<WTLComponent>();
}

void UnregisterStaticComponents()
{
	ComponentManager::Remove<WTLComponent>();
	ComponentManager::Remove<ResourceManager>();
	ComponentManager::Remove<AccountManager>();
	ComponentManager::Remove<TimerComponent>();
	ComponentManager::Remove<SpryComponent>();
	ComponentManager::Remove<EventManager>();
	ComponentManager::Remove<LogComponent>();
}