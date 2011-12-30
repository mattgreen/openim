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

#include "ResourceManager.hpp"

static BOOL CALLBACK EnumerateIcons(HMODULE module, LPCTSTR, LPTSTR name, LONG_PTR instance)
{
	Assert(instance != 0);
	ResourceManager* manager = reinterpret_cast<ResourceManager*>(instance);	

	if(! IS_INTRESOURCE(name))
	{
		CIcon icon(reinterpret_cast<HICON>(LoadImage(module, name, IMAGE_ICON, 0, 0, LR_VGACOLOR)));
		if(! icon.IsNull())
			manager->Add(name, icon);
	}
	return TRUE;
}

void MakeUppercase(String& s)
{
	for(String::iterator i = s.begin(); i != s.end(); ++i)
		*i = static_cast<wchar_t>(toupper(*i));
}

ResourceManager::ResourceManager()
{
	_bindings.Add(this, &ResourceManager::OnModuleLoad);

	_imageList.Create(16, 16, ILC_COLOR32 | ILC_MASK, 8, 8);
	
	HICON folder = LoadIcon(GetModuleHandle(L"shell32.dll"), MAKEINTRESOURCE(4));
	Assert(folder != 0);

	_groupIndex = _imageList.AddIcon(folder);
	Assert(_groupIndex != -1);
}

ResourceManager::~ResourceManager()
{
	for(ProtocolIndexes::iterator i = _indexes.begin(); i != _indexes.end(); ++i)
		delete i->second;
}

void ResourceManager::Add(const String& name, HICON icon)
{
	String::size_type divider = name.find(L'.');
	Assert(divider != String::npos);

	String protocol = name.substr(0, divider);
	String status = name.substr(divider + 1);
	
	Assert(! protocol.empty());
	Assert(! status.empty());

	StatusIndexes* statusIndexes = 0;
	
	ProtocolIndexes::iterator i = _indexes.find(protocol);
	if(i == _indexes.end())
	{
		statusIndexes = new StatusIndexes();
		_indexes[protocol] = statusIndexes;
	}
	else
		statusIndexes = _indexes[protocol];
	
	Assert(statusIndexes != 0);
	
	int index = _imageList.AddIcon(icon);
	Assert(index != -1);

	if(index != -1)
		(*statusIndexes)[status] = index;
}

int ResourceManager::GetGroupImageIndex()
{
	return _groupIndex;
}

int	ResourceManager::GetImageIndex(const wchar_t* protocol)
{
	Assert(protocol != 0);

	String status = L"ONLINE";

	return Lookup(protocol, status);
}

int ResourceManager::GetImageIndex(Account* account)
{
	Assert(account != 0);
	Assert(account->GetProtocolName() != 0);

	String protocol = account->GetProtocolName();

	String status = L"ONLINE";
	if(account->GetConnectionStatus() == Disconnected)
		status = L"OFFLINE";

	return Lookup(protocol, status);
}

int ResourceManager::GetImageIndex(Contact* contact)
{
	Assert(contact != 0);
	Assert(contact->GetAccount() != 0);
	Assert(contact->GetAccount()->GetProtocolName() != 0);

	String status = contact->GetStatus();
	MakeUppercase(status);

	return Lookup(contact->GetAccount()->GetProtocolName(), status);
}

int ResourceManager::GetImageIndex(Group*)
{
	return GetGroupImageIndex();
}

HIMAGELIST ResourceManager::GetImageList()
{
	return _imageList.m_hImageList;
}

int ResourceManager::Lookup(const String& protocol, const String& status)
{
	ProtocolIndexes::iterator i = _indexes.find(protocol);
	if(i == _indexes.end())
		return -1;

	Assert(i->second != 0);
	StatusIndexes::iterator j = i->second->find(status);
	if(j == i->second->end())
		return -1;

	return j->second;
}

void ResourceManager::OnModuleLoad(ModuleLoadEvent& e)
{
	EnumResourceNames(e.Module, RT_GROUP_ICON, EnumerateIcons, reinterpret_cast<LONG_PTR>(this));
}