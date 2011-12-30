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

#include "GenericSNACHandler.hpp"

#include <ctime>

GenericSNACHandler::GenericSNACHandler(AIMContactList* list, Framer& framer, MemoryStream& input, MemoryStream& output) :
	SNACHandler(list, Generic::ServiceID, framer, input, output),

	_idleStart()
{
}

void GenericSNACHandler::DoReset()
{
	KnownServices.clear();
}

void GenericSNACHandler::DoSetup()
{
	//if(_idleStart > 0)
	//	SetIdleTime(static_cast<uint32>(time(0)) - _idleStart);

	_setup = true;
}

State GenericSNACHandler::Process(const SNAC& snac)
{
	State newState = NoChange;

	switch(snac.Subtype)
	{
	case Generic::Error:
		break;

	case Generic::SupportedFamilies:
		ProcessSupportedFamilies(snac);
		break;

	case Generic::RateResponse:
		ProcessRateResponse(snac);
		newState = ServiceSetup;
		break;

	case Generic::ClientServiceVersionResponse:
		ProcessServiceVersionResponse(snac);
		break;
	}

	return newState;
}

void GenericSNACHandler::ProcessError(const SNAC&)
{
}

void GenericSNACHandler::ProcessServiceVersionResponse(const SNAC&)
{
	SendSNAC(Generic::ServiceID, Generic::RateRequest);
}

void GenericSNACHandler::ProcessRateResponse(const SNAC&)
{
	uint16 count = 0;
	_input >> count;

	while(count-- > 0)
	{
		uint16 rateClass = 0;
		_input >> rateClass;
		_output << rateClass;

		// skip the rate info right now
		_inputStream.Seek(_inputStream.Position + 33);
	}

	SendSNAC(Generic::ServiceID, Generic::RateAck);
}

void GenericSNACHandler::ProcessSupportedFamilies(const SNAC&)
{
	while(_inputStream.Position < _inputStream.Length)
	{
		uint16 id;
		_input >> id;

		Service service(id);
		service.Version = 1;
		if(id == Generic::ServiceID || id == SSI::ServiceID)
			service.Version = 3;
		KnownServices.insert(service);

		_output << service.ID;
		_output << service.Version;
	}

	SendSNAC(Generic::ServiceID, Generic::ClientServiceVersionRequest);
}

void GenericSNACHandler::SetIdleTime(uint32 seconds)
{
	_idleStart = static_cast<uint32>(time(0)) - seconds;

	_output << seconds;
	SendSNAC(Generic::ServiceID, Generic::SetIdleTime);
}
