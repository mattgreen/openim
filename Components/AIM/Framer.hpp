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

#ifndef AIM_FRAMER_HPP
#define AIM_FRAMER_HPP

#include "../../Utilities/Traceable.hpp"

#include "Spry.hpp"

#include "Channel.hpp"
#include "SNAC.hpp"

class Framer :
	public Traceable
{
public:
	Framer(AsyncStreamSocket& socket);

public:
	void Send(Channel channel, MemoryStream& stream);
	void SendFLAP(Channel channel, unsigned short size);
	void SendSNAC(const SNAC& snac, MemoryStream& stream);

private:
	Framer(const Framer&);
	Framer& operator=(const Framer&);

private:
	AsyncStreamSocket&	_socket;
	unsigned short		_sequenceNumber;
};

#endif