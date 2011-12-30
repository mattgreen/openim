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

#include "LocationSNACHandler.hpp"

#include "../Capabilities.hpp"

LocationSNACHandler::LocationSNACHandler(AIMContactList* list, Framer& framer, MemoryStream& input, MemoryStream& output) :
	SNACHandler(list, Location::ServiceID, framer, input, output)
{
}

void LocationSNACHandler::DoReset()
{
	MaxProfileLength = 0;
}

void LocationSNACHandler::DoSetup()
{
	SendSNAC(Location::ServiceID, Location::Setup);
}

void LocationSNACHandler::GetAwayMessage(const wchar_t* sn)
{
	GetUserInfo(sn, false, true);
}

void LocationSNACHandler::GetProfile(const char* sn, int length)
{
	//GetUserInfo(sn, length, true, false);
}

void LocationSNACHandler::GetUserInfo(const wchar_t* sn, bool profile, bool awayMessage)
{
	uint32 flags = 0;
	if(profile)
		flags |= 0x01;
	if(awayMessage)
		flags |= 0x02;
	_output << flags;

	unsigned char len = static_cast<unsigned char>(wcslen(sn));
	_output << len;
	_output.WriteChars<Encoding::ASCII>(sn, len);

	SendSNAC(Location::ServiceID, Location::UserInfoRequest);
}

bool LocationSNACHandler::IsAway()
{
	return ! _awayMessage.empty();
}

State LocationSNACHandler::Process(const SNAC& snac)
{
	switch(snac.Subtype)
	{
	case Location::UserInfoResponse:
		ProcessUserInfoResponse(snac);
		break;

	case Location::Limitations:
		ProcessLimitations(snac);
		break;
	}

	return NoChange;
}

void LocationSNACHandler::ProcessLimitations(const SNAC&)
{
	_setup = true;

	TLVReader reader(_input, _inputStream);
	while(reader.Read())
	{
		switch(reader.Type)
		{
		case 0x01:
			MaxProfileLength = reader.Get<uint16>();
			break;
		}
	}

	Update();
}

void LocationSNACHandler::ProcessUserInfoResponse(const SNAC&)
{
	unsigned char snLen = 0;
	_input >> snLen;

	wchar_t sn[32] = { 0, };
	_input.ReadChars<Encoding::ASCII>(sn, 32, snLen);
	
	uint16 warning = 0;
	_input >> warning;

	uint16 count = 0;
	_input >> count;

	uint32 userClass = 0;

	TLVReader reader(_input, _inputStream);
	while(reader.Read())
	{
		switch(reader.Type)
		{
		case 0x01:
			reader >> userClass;
			break;
		}
	}
	
	int a = 0;
}

void LocationSNACHandler::SetAwayMessage(const wchar_t* awayMsg)
{
	_awayMessage.assign(awayMsg);

	Update();
}

void LocationSNACHandler::SetProfile(const wchar_t* profile)
{
//	if(static_cast<int>(profile.Length) > MaxProfileLength)
//		throw InvalidProfileException();

	_profile.assign(profile);
	Update();
}

void LocationSNACHandler::Update()
{
	static const char ContentType[] = "text/aolrtf; charset=\"us-ascii\"";
	static const char DefaultProfile[] = "OpenIM 0.30";

	_output << TLV<const char*>(TLVs::ProfileEncoding, sizeof(ContentType)-1, ContentType);
	_output << TLV<const char*>(TLVs::ProfileContent, sizeof(DefaultProfile)-1, DefaultProfile);
	_output << TLV<const char*>(TLVs::AwayMessageEncoding, sizeof(ContentType)-1, ContentType);
	_output << TLV<const char*>(TLVs::AwayMessage, 0, "");
	
	SendSNAC(Location::ServiceID, Location::UserInfo);
}