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

#ifndef AIM_AIMCONTACT_HPP
#define AIM_AIMCONTACT_HPP

#include <ctime>

#include "../../OpenIM/ContactImpl.hpp"
#include "../../Utilities/Types.hpp"

#include "AIMContactStatus.hpp"
#include "UserInfo.hpp"
#include "Spry.hpp"

class AIMAccount;
class AIMGroup;

class AIMContact :
	public ContactImpl
{
public:
	AIMContact(AIMAccount* account, AIMGroup* group, const wchar_t* name, uint16 iid, uint16 gid, int pos);

public:
	virtual Account*		GetAccount();
	virtual const wchar_t*	GetAlias();
	virtual int				GetIdleTime();
	virtual const wchar_t*	GetStatus();
	virtual const wchar_t*	GetUserID();
	virtual bool			IsConnected();
	virtual bool			SetAlias(const wchar_t* alias);

public:
	void					OnCreate();
	void					OnDestroy();
	void					OnSignOff();
	void					OnSignOn();
	void					OnStatusChange();
	void					OnUpdate();
	void					Update(const UserInfo& info);

public:
	AIMAccount*				Account;
	String					Alias;
	AIMGroup*				Group;
	uint16					GroupID;
	time_t					IdleStart;
	uint16					ItemID;
	int						Position;
	Spry::WCharBuffer<16>	ScreenName;
	AIMContactStatus		Status;
};


#endif