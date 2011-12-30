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

#ifndef AIM_SERVICES_HPP
#define AIM_SERVICES_HPP

#include "../../Utilities/Types.hpp"

struct Generic
{
	static const uint16 ServiceID = 0x01;

	static const uint16 Error = 0x01;
	static const uint16 ClientReady = 0x02;
	static const uint16 SupportedFamilies = 0x03;
	static const uint16 RateRequest = 0x06;
	static const uint16 RateResponse = 0x07;
	static const uint16 RateAck = 0x08;
	static const uint16 SetIdleTime = 0x11;
	static const uint16 ClientServiceVersionRequest = 0x17;
	static const uint16 ClientServiceVersionResponse = 0x18;
};

struct Location
{
	static const uint16	ServiceID = 0x02;
	
	static const uint16 Setup = 0x02;
	static const uint16 Limitations = 0x03;
	static const uint16 UserInfo = 0x04;
	static const uint16 UserInfoResponse = 0x06;
	static const uint16 UserInfoRequest = 0x15;
};

struct BuddyList
{
	static const uint16 ServiceID = 0x03;

	static const uint16 Setup = 0x02;
	static const uint16 SignOn = 0x0B;
	static const uint16 SignOff = 0x0C;
};

struct ICBM
{
	static const uint16 ServiceID = 0x04;

	static const uint16 SetParameters = 0x02;
	static const uint16 Setup = 0x04;
	static const uint16 SetupResponse = 0x05;
	static const uint16 OutgoingMessage = 0x06;
	static const uint16 IncomingMessage = 0x07;
	static const uint16 MissedMessage = 0x0a;
};

struct Invites
{
	static const uint16 ServiceID = 0x06;
};

struct Privacy
{
	static const uint16 ServiceID = 0x09;

	static const uint16 Setup = 0x02;
};

struct SSI
{
	static const uint16 ServiceID = 0x13;

	static const uint16 Setup = 0x02;
	static const uint16 Limitations = 0x03;
	static const uint16 CheckListModificationDate = 0x05;
	static const uint16 ContactList = 0x06;
	static const uint16 Activate = 0x07;
	static const uint16 AddItem = 0x08;
	static const uint16 ModifyItem = 0x09;
	static const uint16 RemoveItem = 0x0a;
	static const uint16 Acknowledge = 0x0e;
	static const uint16 BeginTransaction = 0x11;
	static const uint16 EndTransaction = 0x12;
};

struct Authorization
{
	static const uint16 ServiceID = 0x17;

	static const uint16 ClientLoginRequest = 0x0002;
	static const uint16 ClientLoginResponse = 0x0003;

	static const uint16 MD5Key = 0x0006;
	static const uint16 MD5KeyResponse = 0x0007;
};

#endif