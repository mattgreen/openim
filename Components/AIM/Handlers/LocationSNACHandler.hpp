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

#ifndef AIM_LOCATIONSNACHANDLER_HPP
#define AIM_LOCATIONSNACHANDLER_HPP

#include "../SNACHandler.hpp"

class LocationSNACHandler :
	public SNACHandler
{
public:
	LocationSNACHandler(AIMContactList* list, Framer& framer, MemoryStream& input, MemoryStream& output);

	void	GetAwayMessage(const wchar_t* sn);
	void	GetProfile(const char* sn, int length);
	bool	IsAway();
	State	Process(const SNAC& snac);
	void	SetAwayMessage(const wchar_t* awayMsg);
	void	SetProfile(const wchar_t* profile);

public:
	int		MaxProfileLength;

private:
	void	DoReset();
	void	DoSetup();
	void	GetUserInfo(const wchar_t* sn, bool profile, bool awayMessage);
	void	ProcessLimitations(const SNAC& snac);
	void	ProcessUserInfoResponse(const SNAC& snac);
	void	Update();

private:
	String	_awayMessage;
	String	_profile;
};

#endif