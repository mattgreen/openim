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

#ifndef AIM_AIMGROUP_HPP
#define AIM_AIMGROUP_HPP

#include <vector>

#include "../../OpenIM/Group.hpp"
#include "../../Utilities/Types.hpp"

#include "Spry.hpp"

class AIMAccount;
class AIMContact;

class AIMGroup :
	public Group
{
public:
	AIMGroup(AIMAccount* acct, const wchar_t* name, uint16 gid, int position);
	
public:
	bool operator<(const AIMGroup&) const;

public:
	virtual Account*		GetAccount();
	virtual const wchar_t*	GetName();
	virtual void			Rename(const wchar_t* name);

public:
	void					OnCreate();
	void					OnDestroy();
	void					OnUpdate();

public:
	typedef std::vector<AIMContact*>	AIMContacts;

	AIMContacts				Contacts;
	static const uint16		ItemID = 0;
	uint16					GroupID;
	WCharBuffer<48>			Name;
	int						Position;

private:

	AIMAccount*				_account;
	
};


#endif