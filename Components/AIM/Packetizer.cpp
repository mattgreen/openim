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

#include "Packetizer.hpp"
#include "FLAP.hpp"

#include <algorithm>

using namespace std;

Packetizer::Packetizer()
{
	_buffer.reserve(8192);
}

void Packetizer::Append(char* buffer, int length)
{
	copy(buffer, buffer+length, back_inserter(_buffer));
	
	int bytesRemaining = static_cast<int>(_buffer.size());
	int offset = 0;

	while(bytesRemaining >= static_cast<int>(sizeof(FLAP)))
	{
		const FLAP* header = reinterpret_cast<const FLAP*>(&_buffer[offset]);
		Assert(header->ID == 0x2a);

		int packetSize = NetworkToHostOrder(header->Size) + sizeof(FLAP);

		if(packetSize <= bytesRemaining)
			_lengths.push(packetSize);
	
		offset += packetSize;
		bytesRemaining -= packetSize;
	}
}

void Packetizer::Clear()
{
	_buffer.erase(_buffer.begin(), _buffer.end());
	while(! _lengths.empty())
		_lengths.pop();
}

int Packetizer::GetNextPacket(char* buffer, int length)
{
	int bytes = 0;
	if(HasMorePackets())
	{
		bytes = _lengths.front();
		if(bytes > length)
			return -1;

		_lengths.pop();

		std::copy(_buffer.begin(), _buffer.begin()+bytes, buffer);
		_buffer.erase(_buffer.begin(), _buffer.begin()+bytes);
	}

	return bytes;
}

bool Packetizer::HasMorePackets() const
{
	return ! _lengths.empty();
}
