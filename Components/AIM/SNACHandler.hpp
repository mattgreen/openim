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

#ifndef AIM_SNACFAMILYHANDLER_HPP
#define AIM_SNACFAMILYHANDLER_HPP

#include "../../OpenIM/String.hpp"

#include "../../Utilities/Assert.hpp"
#include "../../Utilities/NetworkBinaryReader.hpp"
#include "../../Utilities/NetworkBinaryWriter.hpp"
#include "../../Utilities/Traceable.hpp"
#include "../../Utilities/Types.hpp"

#include "AIMContactList.hpp"
#include "Framer.hpp"
#include "Services.hpp"
#include "SNAC.hpp"
#include "Spry.hpp"
#include "State.hpp"
#include "TLV.hpp"
#include "TLVReader.hpp"
#include "TLVs.hpp"
#include "UserInfo.hpp"

class SNACHandler :
	public Traceable
{
public:
	SNACHandler(AIMContactList* list, uint16 svc, Framer& framer, MemoryStream& input, MemoryStream& output) :
		ServiceID(svc),

		_list(list),
		_framer(framer),
		_input(input),
		_inputStream(input),
		_output(output),
		_outputStream(output),
		_setup(true)
	{
	}

	bool IsSetup() const
	{
		return _setup;
	}

	void Reset()
	{
		_setup = true;

		DoReset();
	}
	
	void Setup()
	{
		_setup = false;
		
		DoSetup();
	}

public:
	uint16				ServiceID;

protected:
	AIMContactList*		_list;
	NetworkBinaryReader	_input;
	MemoryStream&		_inputStream;

	NetworkBinaryWriter _output;
	MemoryStream&		_outputStream;

	bool				_setup;

protected:
	void SendSNAC(unsigned short service,
		unsigned short subtype,
		unsigned short flags = 0, 
		unsigned int request = 0);

private:
	virtual void DoReset();
	virtual void DoSetup();

private:
	SNACHandler(const SNACHandler&);
	SNACHandler& operator=(const SNACHandler&);

private:
	Framer&				_framer;
};

#endif