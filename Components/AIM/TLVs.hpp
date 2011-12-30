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

#ifndef AIM_TLVS_HPP
#define AIM_TLVS_HPP

struct TLVs
{
	static const unsigned short ProfileEncoding = 0x01;
	static const unsigned short	ScreenName = 0x01;
	static const unsigned short UserFlags = 0x0001;
	static const unsigned short ProfileContent = 0x02;
	static const unsigned short	Message = 0x0002;
	static const unsigned short AccountCreationDate = 0x0002;
	static const unsigned short AwayMessageEncoding = 0x03;
	static const unsigned short ClientIDString = 0x03;
	static const unsigned short OnSinceDate = 0x0003;
	static const unsigned short RequestConfirmation = 0x0003;
	static const unsigned short Autoresponse = 0x0004;
	static const unsigned short AwayMessage = 0x04;
	static const unsigned short ErrorURL = 0x04;
	static const unsigned short IdleTime = 0x0004;

	static const unsigned short BOSIP = 0x05;
	static const unsigned short Capability = 0x05;
	static const unsigned short MemberSinceDate = 0x0005;

	static const unsigned short AuthorizationCookie = 0x06;
	static const unsigned short ICQStatus = 0x0006;

	static const unsigned short ErrorCode = 0x08;
	static const unsigned short HasTypingNotification = 0x000b;
	static const unsigned short CapabilityBlocksLong = 0x000d;

	static const unsigned short Country = 0x0e;

	static const unsigned short AIMSessionLength = 0x000f;

	static const unsigned short Language = 0x0f;

	static const unsigned short AOLSessionLength = 0x0010;
	
	static const unsigned short Distribution = 0x14;
	static const unsigned short ClientID = 0x16;
	static const unsigned short ClientMajorVersion = 0x17;
	static const unsigned short ClientMinorVersion = 0x18;
	static const unsigned short ClientLesserVersion = 0x19;
	static const unsigned short ShortCapabilityBlocks = 0x0019;
	static const unsigned short ClientBuildVersion = 0x1a;
	static const unsigned short SecurityCertificateInfo = 0x001b;
	static const unsigned short ExtraInformation = 0x001d;
	static const unsigned short PasswordHash = 0x25;
	static const unsigned short SSI = 0x4a;

	static const unsigned short Encryption = 0x0d01;
	static const unsigned short MessageData = 0x0101;
	static const unsigned short Features = 0x0501;
};

#endif