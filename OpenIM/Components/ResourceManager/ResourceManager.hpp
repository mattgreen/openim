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

#ifndef OPENIM_COMPONENTS_RESOURCEMANAGER_RESOURCEMANAGER_HPP
#define OPENIM_COMPONENTS_RESOURCEMANAGER_RESOURCEMANAGER_HPP

#include <map>

#include "../../OpenIM.hpp"

#include "../../Account.hpp"
#include "../../Contact.hpp"
#include "../../Group.hpp"
#include "../../String.hpp"

#include "../../Events/ModuleEvents.hpp"
#include "../../Events/EventBindings.hpp"

#include "../../Interfaces/IResourceManager.hpp"

#include "../../WTL.hpp"

class ResourceManager :
	public IResourceManager
{
public:
	ResourceManager();
	~ResourceManager();

public:
	void				Add(const String& name, HICON icon);
	virtual int			GetGroupImageIndex();
	virtual int			GetImageIndex(const wchar_t* protocol);
	virtual int			GetImageIndex(Account* account);
	virtual int			GetImageIndex(Contact* contact);
	virtual int			GetImageIndex(Group* group);
	virtual HIMAGELIST	GetImageList();

private:
	int					Lookup(const String& protocol, const String& status);
	void				OnModuleLoad(ModuleLoadEvent& e);

private:
	typedef std::map<String, int>				StatusIndexes;
	typedef std::map<String, StatusIndexes*>	ProtocolIndexes;
	
	EventBindings		_bindings;
	ProtocolIndexes		_indexes;
	CImageList			_imageList;
	int					_groupIndex;
};


#endif