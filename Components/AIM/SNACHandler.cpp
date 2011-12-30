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

#include "SNACHandler.hpp"

void SNACHandler::DoReset()
{
}

void SNACHandler::DoSetup()
{
	_setup = true;
}

void SNACHandler::SendSNAC(unsigned short service,
	unsigned short subtype,
	unsigned short flags, 
	unsigned int request)
{
	SNAC snac(service, subtype);
	snac.Flags = flags;
	snac.RequestID = request;

	_framer.SendSNAC(snac, _outputStream);
}
