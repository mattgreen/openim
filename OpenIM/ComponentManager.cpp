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

#include "ComponentManager.hpp"
#include "Component.hpp"

#pragma warning(push)
#pragma warning(disable:4702)
#include <algorithm>
#include <vector>
#pragma warning(pop)

#include "Log.hpp"

#include "../Libraries/Spry/DynamicLibrary.hpp"
#include "../Libraries/Spry/Environment.hpp"
#include "../Libraries/Spry/IO/Directory.hpp"
#include "../Libraries/Spry/IO/Path.hpp"
#include "../Libraries/Spry/String.hpp"

#include "Events/ModuleEvents.hpp"

using namespace Spry;
using namespace Spry::IO;

typedef int (* ComponentVersionFunction)();
typedef void (* ComponentSetupFunction)();

typedef std::vector<Component*>		Components;
typedef std::vector<DynamicLibrary>	DynamicLibraries;

static Components		_components;
static DynamicLibraries	_libraries;
static Log				_log(L"System");

void ComponentManager::DoAdd(Component* component)
{
	_components.push_back(component);
}

Component* ComponentManager::DoGet(ComponentID id)
{
	for(Components::iterator i = _components.begin(); i != _components.end(); ++i)
	{
		if((*i)->GetID() == id)
			return *i;
	}

	return 0;
}

void ComponentManager::DoRemove(Component* component)
{
	_components.erase(std::remove(_components.begin(), _components.end(), component));
}

void ComponentManager::Load()
{
	String componentsDirectory = Path::Combine(Environment::GetWorkingDirectory(), L"Components\\");

	StringList components = Directory::GetFiles(componentsDirectory, L"*.dll");
	for(StringList::iterator i = components.begin(); i != components.end(); ++i)
	{
		_log.Write(L"Loading components in %s", i->c_str());

		try
		{
			DynamicLibrary library(Path::Combine(componentsDirectory, *i));
			if( ! library.HasSymbol("RegisterComponents") ||
				! library.HasSymbol("UnregisterComponents") ||
				! library.HasSymbol("GetAPIVersion"))
			{
				wchar_t msg[1024];
				wsprintf(msg, L"%s is missing one or more required exports:\r\n"
					L"   RegisterComponents()\r\n"
					L"   UnregisterComponents()\r\n"
					L"   GetAPIVersion()\r\n\r\n"
					L"The component will not be loaded.", i->c_str());

				_log.WriteError(L"Error loading %s: missing required exports", i->c_str());

				MessageBox(0, msg, L"ComponentManager: Error loading component", MB_ICONASTERISK | MB_OK);

				continue;
			}

			int apiVersion = (library.GetSymbol<ComponentVersionFunction>("GetAPIVersion"))();
			if(apiVersion != OPENIM_API_VERSION)
			{
				_log.WriteError(L"Error loading %s: wrong API version (reported %d, expected %d)",
					i->c_str(), apiVersion, OPENIM_API_VERSION);

				continue;
			}
			if(library.HasSymbol("InitializeComponent"))
				(library.GetSymbol<ComponentSetupFunction>("InitializeComponents"))();

			(library.GetSymbol<ComponentSetupFunction>("RegisterComponents"))();

			_libraries.push_back(library);

		}
		catch(DynamicLibraryException& e)
		{
			_log.WriteError(L"Error loading %s: %s", i->c_str(), e.Description);
		}
	}

	for(DynamicLibraries::iterator i = _libraries.begin(); i != _libraries.end(); ++i)
	{
		ModuleLoadEvent e;
		e.Module = i->GetHandle();
		e.Invoke();
	}
}

void ComponentManager::Unload()
{
	for(DynamicLibraries::reverse_iterator i = _libraries.rbegin(); i != _libraries.rend(); ++i)
	{
		wchar_t path[MAX_PATH] = { 0, };
		GetModuleFileName(i->GetHandle(), path, sizeof(path) / sizeof(wchar_t));
		wchar_t* filename = wcsrchr(path, L'\\') + 1;
		_log.Write(L"Unloading components in %s", filename);

		(i->GetSymbol<ComponentSetupFunction>("UnregisterComponents"))();

		if(i->HasSymbol("UninitializeComponents"))
			(i->GetSymbol<ComponentSetupFunction>("UninitializeComponents"))();
	}

	_libraries.clear();
}