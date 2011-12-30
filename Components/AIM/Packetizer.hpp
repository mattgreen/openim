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

#ifndef AIM_FLAPPACKETIZER_HPP
#define AIM_FLAPPACKETIZER_HPP

#pragma warning(push)
#pragma warning(disable:4702)
#include <queue>
#include <vector>
#pragma warning(pop)

#include "Spry.hpp"

// Breaks a reliable, sequenced stream of bytes into discrete packets,
// suitable for	further processing. 
class Packetizer
{
public:
	Packetizer();

	// Appends the contents of the specified buffer onto
	// the internal one, and attempts to form a packet.
	void	Append(char* buffer, int length);

	// Clears the internal buffer.
	void	Clear();

	// Copies the contents of the next queued packet into the
	// specified buffer. Make sure that the buffer is at least
	// 4096 bytes long. Returns the number of bytes copied into the buffer.
	int		GetNextPacket(char* buffer, int length);

	// Returns true if one or more packets have been completely formed.
	bool	HasMorePackets() const;

private:
	typedef std::vector<char>	Buffer;
	typedef std::queue<int>		QueuedPacketLengths;

	Buffer						_buffer;
	QueuedPacketLengths			_lengths;
};

#endif