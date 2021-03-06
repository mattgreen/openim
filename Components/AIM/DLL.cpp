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

#include "../../OpenIM/OpenIM.hpp"
#include "../../OpenIM/Interfaces/IAccountManager.hpp"

#include <ctime>
#include <cstdlib>

#include "AIMAccountFactory.hpp"
#include "Spry.hpp"

AIMAccountFactory _factory;

extern "C"
{
	EXPORTED void RegisterComponents()
	{
		srand(static_cast<unsigned int>(time(0)));

		Networking::Internal::Winsock::Initialize();

		IAccountManager* manager = ComponentManager::Get<IAccountManager>();
		manager->Register(&_factory);
	}

	EXPORTED void UnregisterComponents()
	{
		IAccountManager* manager = ComponentManager::Get<IAccountManager>();
		manager->Unregister(&_factory);

		Networking::Internal::Winsock::Uninitialize();
	}

	EXPORTED int GetAPIVersion()
	{
		return OPENIM_API_VERSION;
	} 
}