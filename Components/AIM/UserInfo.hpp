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

#ifndef AIM_USERINFO_HPP
#define AIM_USERINFO_HPP

#include "Spry.hpp"
#include "TLVReader.hpp"

struct UserInfo
{
public:
	UserInfo(NetworkBinaryReader& binReader, SeekableStream& stream)
	{
		IdlePresent = false;

		uint8 snLength = 0;
		binReader >> snLength;
		if(snLength > ScreenName.Capacity)
		{
			Fail("Screenname is too long!");
			return;
		}
		binReader.ReadChars<Encoding::ASCII>(ScreenName, snLength);

		binReader >> WarningLevel;
		
		uint16 num = 0;
		binReader >> num;

		TLVReader reader(binReader, stream);

		uint16 i = 0;
		
		reader.Read();
		do
		{
			switch(reader.Type)
			{
			case 0x01:
				reader >> Class;
				break;
			
			case 0x04:
				reader >> IdleStart;
				IdlePresent = true;
				break;

			case 0x0F:
				reader >> SignonTime;
				break;
			}

			reader.Read();
			i++;
		}
		while(i < num);
	}

	bool IsAway() const
	{
		return (Class & 0x20) != 0;
	}

	WCharBuffer<16>	ScreenName;
	
	uint16			Class;
	uint32			SignonTime;
	uint16			WarningLevel;
	uint16			IdleStart;
	bool			IdlePresent;
};



#endif