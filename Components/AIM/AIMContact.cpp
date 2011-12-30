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

#include <cmath>

#include "AIMContact.hpp"
#include "AIMAccount.hpp"
#include "AIMGroup.hpp"

#include "../../OpenIM/Events/ContactEvents.hpp"

AIMContact::AIMContact(AIMAccount* account, AIMGroup* group, const wchar_t* name, uint16 iid, uint16 gid, int pos) :
	ItemID(iid),
	GroupID(gid),
	Position(pos),
	Status(StatusDisconnected),
	Account(account),
	Group(group),
	ScreenName(name),
	IdleStart(0)
{
}

Account* AIMContact::GetAccount()
{
	return Account;
}

const wchar_t* AIMContact::GetAlias()
{
	return Alias.c_str();
}

const wchar_t* AIMContact::GetStatus()
{
	if(! IsConnected())
		return L"Offline";

	switch(Status)
	{
	default:
	case StatusAvailable:
		return L"Online";
		break;

	case StatusInvisible:
		return L"Invisible";
		break;

	case StatusAway:
		return L"Away";
		break;
	}
}

const wchar_t* AIMContact::GetUserID()
{
	return ScreenName;
}

int AIMContact::GetIdleTime()
{
	// Not online? Then there's no idle time to report!
	if(! IsConnected())
		return 0;

	// Idle start of zero indicates the user is not idle.
	if(IdleStart == 0)
		return 0;

	return static_cast<int>(static_cast<double>(time(0) - IdleStart) / 60.0);
}

bool AIMContact::IsConnected()
{
	return Status != StatusDisconnected;
}

void AIMContact::OnCreate()
{
	ContactCreateEvent e;
	e.Contact = this;
	e.Group = Group;
	e.Position = Position;
	e.Invoke();
}

void AIMContact::OnDestroy()
{
	ContactDestroyEvent e;
	e.Contact = this;
	e.Group = Group;
	e.Invoke();
}

void AIMContact::OnSignOff()
{
	ContactSignOffEvent e;
	e.Contact = this;
	e.Invoke();
}

void AIMContact::OnSignOn()
{
	ContactSignOnEvent e;
	e.Contact = this;
	e.Invoke();
}

void AIMContact::OnStatusChange()
{
	ContactStatusChangeEvent e;
	e.Contact = this;
	e.Invoke();
}

void AIMContact::OnUpdate()
{
	ContactUpdateEvent e;
	e.Contact = this;
	e.Invoke();
}

bool AIMContact::SetAlias(const wchar_t* alias)
{
	Account->AliasContact(alias, GroupID, ItemID);

	return true;
}

void AIMContact::Update(const UserInfo& info)
{
	if(info.IsAway())
		Status = StatusAway;
	else
		Status = StatusAvailable;

	if(info.IdlePresent)
	{
		if(info.IdleStart == 0)
			IdleStart = 0;
		else
			IdleStart = time(0) - (info.IdleStart * 60);
	}

	ScreenName.Assign(info.ScreenName);
}