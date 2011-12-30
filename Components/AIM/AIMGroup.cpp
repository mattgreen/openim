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

#include "AIMGroup.hpp"

#include "AIMAccount.hpp"
#include "AIMContact.hpp"

#include "../../OpenIM/Events/GroupEvents.hpp"

AIMGroup::AIMGroup(AIMAccount* acct, const wchar_t* name, uint16 gid, int position) :
	_account(acct),
	Name(name),
	GroupID(gid),
	Position(position)
{
}

bool AIMGroup::operator <(const AIMGroup& r) const
{
	return Position < r.Position;
}

Account* AIMGroup::GetAccount()
{
	return _account;
}

const wchar_t* AIMGroup::GetName()
{
	return Name;
}

void AIMGroup::Rename(const wchar_t* name)
{
	_account->RenameGroup(name, GroupID);
}

void AIMGroup::OnCreate()
{
	GroupCreateEvent e;
	e.Group = this;
	e.Parent = 0;
	e.Position = Position;
	e.Invoke();
}

void AIMGroup::OnDestroy()
{
	GroupDestroyEvent e;
	e.Group = this;
	e.Invoke();
}

void AIMGroup::OnUpdate()
{
	GroupUpdateEvent e;
	e.Group = this;
	e.Invoke();
}