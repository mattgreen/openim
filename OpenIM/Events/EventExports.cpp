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

#include "EventExports.hpp"
#include "../ComponentManager.hpp"
#include "../Interfaces/IEventManager.hpp"
#include "../../Utilities/Assert.hpp"

extern "C"
{
	IEventManager* GetEventManager();

	void EventBind(const wchar_t* name, const EventHandler& handler)
	{
		Assert(name != 0);

		IEventManager* eventManager = GetEventManager();
		eventManager->Bind(name, handler);
	}

	void EventInvoke(const wchar_t* name, void* data, void (*invoker)(void*, const EventHandler& handler))
	{
		Assert(name != 0);
		Assert(data != 0);
		Assert(invoker != 0);

		IEventManager* eventManager = GetEventManager();		
		eventManager->Invoke(name, data, invoker);
	}

	void EventUnbind(const wchar_t* name, const EventHandler& handler)
	{
		Assert(name != 0);

		IEventManager* eventManager = GetEventManager();
		eventManager->Unbind(name, handler);
	}

	IEventManager* GetEventManager()
	{
		static IEventManager* instance = 0;
		
		if(instance == 0)
			instance = ComponentManager::Get<IEventManager>();
		Assert(instance != 0);
		
		return instance;
	}
}