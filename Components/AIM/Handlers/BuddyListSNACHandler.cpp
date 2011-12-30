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

#include "BuddyListSNACHandler.hpp"

BuddyListSNACHandler::BuddyListSNACHandler(AIMContactList* list, Framer& framer, MemoryStream& input, MemoryStream& output) :
	SNACHandler(list, BuddyList::ServiceID, framer, input, output)
{
}

void BuddyListSNACHandler::DoSetup()
{
	SendSNAC(BuddyList::ServiceID, BuddyList::Setup);
}

State BuddyListSNACHandler::Process(const SNAC& snac)
{
	_setup = true;

	switch(snac.Subtype)
	{
	case BuddyList::SignOn:
		ProcessSignOn(snac);
		break;

	case BuddyList::SignOff:
		ProcessSignOff(snac);
		break;
	}

	return NoChange;
}

void BuddyListSNACHandler::ProcessSignOff(const SNAC&)
{
	UserInfo info(_input, _inputStream);

	_list->OnContactSignOff(info);
}

void BuddyListSNACHandler::ProcessSignOn(const SNAC&)
{
	UserInfo info(_input, _inputStream);

	_list->OnContactSignOn(info);
}
