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

#include "ICBMSNACHandler.hpp"

ICBMSNACHandler::ICBMSNACHandler(AIMContactList* list, Framer& framer, MemoryStream& input, MemoryStream& output) :
	SNACHandler(list, ICBM::ServiceID, framer, input, output)
{
}

void ICBMSNACHandler::DoReset()
{
	MaxMessageLength = 0;
}

void ICBMSNACHandler::DoSetup()
{
	SendSNAC(ICBM::ServiceID, ICBM::Setup);
}

State ICBMSNACHandler::Process(const SNAC& snac)
{
	switch(snac.Subtype)
	{
	case ICBM::SetupResponse:
		ProcessSetupResponse(snac);
		break;

	case ICBM::IncomingMessage:
		ProcessIncomingMessage(snac);
		break;

	case ICBM::MissedMessage:
		ProcessMissedMessage(snac);
		break;
	}

	return NoChange;
}

void ICBMSNACHandler::ProcessIncomingMessage(const SNAC& snac)
{
	uint64 cookie = 0;
	_input >> cookie;

	uint16 channel = 0;
	_input >> channel;
	
	switch(channel)
	{
	case 0x01:
		ProcessIncomingMessageBasic(snac);
		break;
	}
}

void ICBMSNACHandler::ProcessIncomingMessageBasic(const SNAC&)
{
	UserInfo userInfo(_input, _inputStream);

	String message;
	bool autoresponse = false;

	TLVReader reader(_input, _inputStream);
	while(reader.Read())
	{
		switch(reader.Type)
		{
		case TLVs::Autoresponse:
			autoresponse = true;
			break;

		case TLVs::MessageData:
			{
				uint16 len = reader.Length;

				// Hey, this part is actually useful, what were the developers thinking?
				uint16 charSet = 0;
				_input >> charSet;
				len -= sizeof(charSet);
				
				// Almost always 0xffff.
				uint16 charSubset = 0;
				_input >> charSubset;
				len -= sizeof(charSubset);

				String messagePart;
				switch(charSet)
				{
				case 0x00:
					messagePart = _input.ReadString<Encoding::ASCII>(len);
					break;

				case 0x02:
					messagePart = _input.ReadString<Encoding::UTF16BE>(len);
					break;

				case 0x03:
					messagePart = _input.ReadString<Encoding::ISO88591>(len);
					break;
				}

				message += messagePart;
			}
			break;
		}
	}

	if(OnMessageReceive != 0)
		OnMessageReceive(userInfo.ScreenName, message.c_str(), autoresponse);
}

void ICBMSNACHandler::ProcessMissedMessage(const SNAC&)
{
	uint16 channel = 0;
	_input >> channel;

	UserInfo userInfo(_input, _inputStream);
	
	uint16 numMessagesMissed = 0;
	_input >> numMessagesMissed;

	uint16 reason = 0;
	_input >> reason;
}

void ICBMSNACHandler::ProcessSetupResponse(const SNAC&)
{
	static const unsigned char MessagesAllowed = 1;
	static const unsigned char MissedMessageNotification = 2;
	static const unsigned char TypingNotifications = 8;

	struct ICBMParameters
	{
		uint16	Channel;
		uint32	Flags;
		uint16	MessageSize;
		uint16	MaxSendWarning;
		uint16	MaxReceiverWarning;
		uint16	MinimumMessageInterval;
		uint16	Unknown;
	};
	
	ICBMParameters params;
	params.Channel = 0;
	params.Flags = MessagesAllowed | MissedMessageNotification | TypingNotifications;
	params.MessageSize = 8000;
	params.MaxSendWarning = 999;
	params.MaxReceiverWarning = 999;
	params.MinimumMessageInterval = 0;
	params.Unknown = 0;

	_output << params.Channel;
	_output << params.Flags;
	_output << params.MessageSize;
	_output << params.MaxSendWarning;
	_output << params.MaxReceiverWarning;
	_output << params.MinimumMessageInterval;
	_output << params.Unknown;

	SendSNAC(ICBM::ServiceID, ICBM::SetParameters);

	MaxMessageLength = params.MessageSize;

	_setup = true;	
}

void ICBMSNACHandler::Send(const wchar_t* user, const wchar_t* msg, bool autoresponse)
{
	// Random junk for the cookie, it is useless.
	char cookie[8] = { 0, };
	for(int i = 0; i < sizeof(cookie); i++)
		cookie[i] = static_cast<char>(rand() % 255);
	_output.WriteBytes(cookie, sizeof(cookie));

	// Just a basic text message, thankyouverymuch!
	_output << static_cast<uint16>(0x01);

	// Recipient screenname.
	Buffer<16> screenName;
	screenName.Length = Encoding::ASCII::GetBytes(user, screenName, screenName.Capacity);
	
	_output << static_cast<uint8>(screenName.Length);
	_output.WriteBytes(screenName, screenName.Length);
	
	static const char features[] =  { 0x01, 0x01, 0x01, 0x02 };

	uint16 messageDataLen = 0;
	messageDataLen += 1 + 1;			// 0x05, 0x01 type
	messageDataLen += sizeof(uint16);	// 0x05, 0x01 length
	messageDataLen += sizeof(features);	// 0x05, 0x01 data

	messageDataLen += 1 + 1;			// 0x01, 0x01 fragment
	messageDataLen += sizeof(uint16);	// 0x01, 0x01 length
	messageDataLen += 2 + 2;			// charset, charsubset

	uint16 charset = 0;
	uint16 charsubset = 0xffff;

	// Determine the lowest possible encoding suitable for the message.
	Buffer<8192> msgBuffer;
	if(Encoding::ASCII::CanMap(msg))
	{
		msgBuffer.Length = Encoding::ASCII::GetBytes(msg, msgBuffer, msgBuffer.Capacity);

		charset = 0;
	}
	else if(Encoding::ISO88591::CanMap(msg))
	{
		msgBuffer.Length = Encoding::ISO88591::GetBytes(msg, msgBuffer, msgBuffer.Capacity);

		charset = 3;
	}
	else
	{
		msgBuffer.Length = Encoding::UTF16BE::GetBytes(msg, msgBuffer, msgBuffer.Capacity);

		charset = 2;
	}

	messageDataLen = messageDataLen + static_cast<uint16>(msgBuffer.Length);

	// Write TLV out manually since this is so freakin complicated.
	_output << static_cast<uint16>(0x02);				// Message TLV
	_output << messageDataLen;							// Message TLV len
	
	_output << static_cast<uint8>(0x05);				// Features fragment type
	_output << static_cast<uint8>(0x01);				// Features fragment version
	_output << static_cast<uint16>(sizeof(features));	// Features fragment len
	_output.WriteBytes(features, sizeof(features));		// Features block

	_output << static_cast<uint8>(0x01);				// Message text fragment
	_output << static_cast<uint8>(0x01);				// Message text fragment version
	_output << static_cast<uint16>(msgBuffer.Length + sizeof(uint16) + sizeof(uint16));
	_output << charset;
	_output << charsubset;
	_output.WriteBytes(msgBuffer, msgBuffer.Length);

	if(autoresponse)
		_output << TLV<char*>(0x04, 0x00);

	SendSNAC(ICBM::ServiceID, ICBM::OutgoingMessage, 0, 0);
}
