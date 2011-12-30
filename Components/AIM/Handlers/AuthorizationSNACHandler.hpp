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

#ifndef AIM_AUTHORIZATIONSNACHANDLER_HPP
#define AIM_AUTHORIZATIONSNACHANDLER_HPP

#include "../SNACHandler.hpp"

class AuthorizationSNACHandler :
	public SNACHandler
{
public:
	AuthorizationSNACHandler(AIMContactList* list, Framer& framer, MemoryStream& input, MemoryStream& output);

	State			Process(const SNAC& snac);

public:
	ANSIString		BOSAddress;
	unsigned short	BOSPort;
	TLV<char[512]>	Cookie;
	uint16			ErrorCode;
	const wchar_t*	ErrorMessage;
	Buffer<256>		ErrorURL;
	WCharBuffer<16>	NormalizedScreenName;
	String			ScreenName;
	String			Password;

private:
	const wchar_t*	GetErrorMessage(unsigned short code);
	void			ProcessMD5KeyResponse(const SNAC& snac);
	bool			ProcessLogonResponse(const SNAC& snac);
};


#endif