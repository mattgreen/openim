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

#ifndef AIM_SSIITEM_HPP
#define AIM_SSIITEM_HPP

#include "Spry.hpp"
#include "../../Utilities/Types.hpp"

struct SSIItem
{
	SSIItem() :
		GroupID(),
		ItemID(),
		Type()
	{
	}

	WCharBuffer<64>	Name;
	uint16			GroupID;
	uint16			ItemID;
	uint16			Type;
};

template<typename Stream>
Stream& operator<<(Stream& s, SSIItem& i)
{
	uint16 len = static_cast<uint16>(i.Name.Length);
	s << len;
	if(len > 0)
		s.WriteChars<Encoding::ASCII>(i.Name, i.Name.Length);

	s << i.GroupID;
	s << i.ItemID;
	s << i.Type;

	return s;
}

template<typename Stream>
Stream& operator>>(Stream& s, SSIItem& i)
{
	uint16 len = 0;
	s >> len;
	if(len > 0)
		s.ReadChars<Encoding::ASCII>(i.Name, len);

	s >> i.GroupID;
	s >> i.ItemID;
	s >> i.Type;

	return s;
}


#endif