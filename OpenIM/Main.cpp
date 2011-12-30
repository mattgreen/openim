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

#include "OpenIM.hpp"

#include "ComponentManager.hpp"
#include "Components/StaticComponents.hpp"
#include "CrashHandler.hpp"
#include "Events/SystemEvents.hpp"
#include "Interfaces/IMessageLoop.hpp"
#include "Spry.hpp"

extern "C"
{
	EXPORTED void Run(const wchar_t*)
	{
		if(IMessageLoop* loop = ComponentManager::Get<IMessageLoop>())
			loop->Run();
	}

	EXPORTED void Shutdown(const wchar_t* profile)
	{
		UnloadUserProfileEvent unloadProfile;
		unloadProfile.ProfileDirectory = profile;
		unloadProfile.Invoke();

		ShutdownEvent shutdown;
		shutdown.Invoke();

		ComponentManager::Unload();
		UnregisterStaticComponents();
	}

	EXPORTED void Startup(const wchar_t* profile)
	{
		InstallCrashHandler();

		RegisterStaticComponents();
		ComponentManager::Load();

		StartupEvent startup;
		startup.Invoke();

		LoadUserProfileEvent loadProfile;
		loadProfile.ProfileDirectory = profile;
		loadProfile.Invoke();
	}
}