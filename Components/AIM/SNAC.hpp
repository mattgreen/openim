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

#ifndef AIM_SNAC_HPP
#define AIM_SNAC_HPP

#pragma pack(push,1)
struct SNAC
{
	SNAC(unsigned short service = 0, unsigned short subtype = 0) :
		Service(service),
		Subtype(subtype),
		Flags(0),
		RequestID(0)
	{
	}

	unsigned short	Service;
	unsigned short	Subtype;
	unsigned short	Flags;
	unsigned int	RequestID;
};
#pragma pack(pop)

template<typename Stream>
Stream& operator>>(Stream& s, SNAC& snac)
{
	s >> snac.Service;
	s >> snac.Subtype;
	s >> snac.Flags;
	s >> snac.RequestID;
			
	if(snac.Flags & 0x8000)
	{
		unsigned short len = 0;
		s >> len;
		
		char c = 0;
		for(int i = 0; i < len; i++)
			s >> c;
	}

	return s;
}


template<typename Stream>
Stream& operator<<(Stream& s, SNAC& snac)
{
	s << snac.Service;
	s << snac.Subtype;
	s << snac.Flags;
	s << snac.RequestID;

	return s;
}

#endif