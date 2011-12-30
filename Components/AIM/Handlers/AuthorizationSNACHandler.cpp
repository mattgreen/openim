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

#include "AuthorizationSNACHandler.hpp"
#include "../MD5.h"

namespace
{
	const char		MD5String[] = "AOL Instant Messenger (SM)";
	const char		ClientName[] = "AOL Instant Messenger, version 5.2.3292/WIN32";
	const uint16	ClientID =  0x0109;
	const uint16	ClientMajorVersion = 0x0005;
	const uint16	ClientMinorVersion = 0x0002;
	const uint16	ClientPointVersion = 0x0000;
	const uint16	ClientBuildVersion = 3292;
}

AuthorizationSNACHandler::AuthorizationSNACHandler(AIMContactList* list, Framer& framer, MemoryStream& input, MemoryStream& output) :
	SNACHandler(list, Authorization::ServiceID, framer, input, output),

	ErrorMessage(L""),
	ScreenName(),
	Password()
{
}

const wchar_t* AuthorizationSNACHandler::GetErrorMessage(unsigned short code)
{
	switch(code)
	{
	case 0x01:
	case 0x04:
		return L"The screen name or password you entered is not valid.";

	case 0x05:
		return L"The password you entered is not valid.";

	case 0x18:
	case 0x1d:
		return L"You are attempting to sign on again too soon. Wait at least ten minutes and try again.";

	case 0x0c:
	case 0x0d:
	case 0x12:
	case 0x13:
	case 0x14:
	case 0x15:
	case 0x1a:
	case 0x1f:
		return L"The AIM service is temporarily unavailable.";
	
	default:
		return L"Unknown error.";
	}
}

State AuthorizationSNACHandler::Process(const SNAC& snac)
{
	State newState = NoChange;

	switch(snac.Subtype)
	{
	case Authorization::MD5KeyResponse:
		ProcessMD5KeyResponse(snac);
		break;

	case Authorization::ClientLoginResponse:
		if(ProcessLogonResponse(snac))
			newState = AuthorizationSuccess;
		else
			newState = AuthorizationFailure;
	
		break;
	}

	return newState;
}

bool AuthorizationSNACHandler::ProcessLogonResponse(const SNAC&)
{
	Trace("Processing logon response.\n");

	bool success = true;

	TLVReader reader(_input, _inputStream);
	while(reader.Read())
	{
		switch(reader.Type)
		{
		case TLVs::ScreenName:
			ScreenName = reader.GetString<Encoding::ASCII>();

			NormalizedScreenName.Zero();
			for(String::iterator i = ScreenName.begin(); i != ScreenName.end(); ++i)
			{
				if(Char::IsLetter(*i) || Char::IsDigit(*i))
					NormalizedScreenName.Append(Char::ToLowercase(*i));
			}
			break;

		case TLVs::ErrorURL:
			ErrorURL.Zero();
			ErrorURL.Length = reader.GetBytes(ErrorURL, ErrorURL.Capacity);
			break;

		case TLVs::ErrorCode:			
			ErrorCode = reader.Get<uint16>();
			ErrorMessage = GetErrorMessage(ErrorCode);

			success = false;
		
			break;

		case TLVs::BOSIP:
			{
				TLV<char[256]> bosIP;
				reader >> bosIP;

				BOSAddress.assign(bosIP.Value, bosIP.Length);

				ANSIString::size_type colon = BOSAddress.find(':');
				BOSPort = UInt16::Parse(BOSAddress.c_str() + colon + 1);
				BOSAddress = BOSAddress.substr(0, colon);
			}
			break;

		case TLVs::AuthorizationCookie:
			reader >> Cookie;
			break;
		}
	}

	return success;
}

void AuthorizationSNACHandler::ProcessMD5KeyResponse(const SNAC&)
{
	Trace("Processing MD5 key response.\n");

	// Convert username to ASCII and attach to outgoing packet.
	Assert(ScreenName.length() <= 16);
	TLV<char[16]> screenNameTlv(TLVs::ScreenName);
	screenNameTlv.Length =
		static_cast<unsigned short>(Encoding::ASCII::GetBytes(ScreenName, screenNameTlv.Value, 16));
	_output << screenNameTlv;

	// Convert password to ASCII.
	Assert(Password.length() < 32);
	char password[32] = { 0, };
	int passwordLength = Encoding::ASCII::GetBytes(Password, password, DimensionOf(password));

	// Read MD5 key.
	char key[256] = { 0, };
	uint16 length = 0;
	_input >> length;
	Assert(length < sizeof(key));
	_input.ReadBytes(key, length);
	
	md5_byte_t authHash[16];
	md5_state_t state;
	md5_init(&state);
	md5_append(&state, reinterpret_cast<md5_byte_t*>(key), length);
	md5_append(&state, reinterpret_cast<md5_byte_t*>(password), passwordLength);
	md5_append(&state, reinterpret_cast<const md5_byte_t*>(MD5String), sizeof(MD5String)-1);
	md5_finish(&state, reinterpret_cast<md5_byte_t*>(authHash));
	_output << TLV<char*>(TLVs::PasswordHash, sizeof(authHash), reinterpret_cast<char*>(authHash));

	_output << TLV<const char*>(TLVs::ClientIDString, sizeof(ClientName)-1, ClientName);
	_output << TLV<uint16>(TLVs::ClientID, 2, ClientID);
	
	_output << TLV<uint16>(TLVs::ClientMajorVersion, sizeof ClientMajorVersion, ClientMajorVersion);
	_output << TLV<uint16>(TLVs::ClientMinorVersion, sizeof ClientMinorVersion, ClientMinorVersion);
	_output << TLV<uint16>(TLVs::ClientLesserVersion, sizeof ClientPointVersion, ClientPointVersion);
	_output << TLV<uint16>(TLVs::ClientBuildVersion, sizeof ClientBuildVersion, ClientBuildVersion);
	
	_output << TLV<const char*>(TLVs::Country, 2, "us");
	_output << TLV<const char*>(TLVs::Language, 2, "en");
	_output << TLV<const char*>(TLVs::SSI, 1, "\1");
	
	SendSNAC(Authorization::ServiceID, Authorization::ClientLoginRequest);
}
