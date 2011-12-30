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

#ifndef OPENIM_UTILITIES_NETWORKBINARYREADER_HPP
#define OPENIM_UTILITIES_NETWORKBINARYREADER_HPP

#include "../Libraries/Spry/IO/BinaryReader.hpp"
#include "../Libraries/Spry/IO/Networking/Networking.hpp"

class NetworkBinaryReader :
	public Spry::IO::BinaryReader
{
public:
	NetworkBinaryReader(Spry::IO::Stream& stream) :
		BinaryReader(stream)
	{
	}

public:
	NetworkBinaryReader& operator>>(char& n)
	{
		n = Read<char>();
		return *this;
	}

	NetworkBinaryReader& operator>>(signed char& n)
	{
		n = Read<signed char>();
		return *this;
	}

	NetworkBinaryReader& operator>>(unsigned char& n)
	{
		n = Read<unsigned char>();
		return *this;
	}

	NetworkBinaryReader& operator>>(unsigned short& n)
	{
		n = Read<unsigned short>();
		return *this;
	}

	NetworkBinaryReader& operator>>(signed short& n)
	{
		n = Read<signed short>();
		return *this;
	}

	NetworkBinaryReader& operator>>(unsigned int& n)
	{
		n = Read<unsigned int>();
		return *this;
	}

	NetworkBinaryReader& operator>>(signed int& n)
	{
		n = Read<signed int>();
		return *this;
	}

	NetworkBinaryReader& operator>>(unsigned long& n)
	{
		n = Read<unsigned long>();
		return *this;
	}

	NetworkBinaryReader& operator>>(signed long& n)
	{
		n = Read<signed long>();
		return *this;
	}

	NetworkBinaryReader& operator>>(unsigned __int64& n)
	{
		n = Read<unsigned __int64>();
		return *this;
	}

	NetworkBinaryReader& operator>>(signed __int64& n)
	{
		n = Read<signed __int64>();
		return *this;
	}

public:
	template<typename T>
	T Read();

	template<>
	char Read<>()
	{
		return BinaryReader::Read<char>();
	}

	template<>
	signed char Read<>()
	{
		return BinaryReader::Read<signed char>();
	}

	template<>
	unsigned char Read<>()
	{
		return BinaryReader::Read<unsigned char>();
	}

	template<>
	unsigned short Read<>()
	{
		return Spry::IO::Networking::NetworkToHostOrder(BinaryReader::Read<unsigned short>());
	}

	template<>
	signed short Read<>()
	{
		return Spry::IO::Networking::NetworkToHostOrder(BinaryReader::Read<signed short>());
	}

	template<>
	unsigned int Read<>()
	{
		return Spry::IO::Networking::NetworkToHostOrder(BinaryReader::Read<unsigned int>());
	}

	template<>
	signed int Read<>()
	{
		return Spry::IO::Networking::NetworkToHostOrder(BinaryReader::Read<signed int>());
	}

	template<>
	unsigned long Read<>()
	{
		return Spry::IO::Networking::NetworkToHostOrder(BinaryReader::Read<unsigned long>());
	}

	template<>
	signed long Read<>()
	{
		return Spry::IO::Networking::NetworkToHostOrder(BinaryReader::Read<signed long>());
	}

	template<>
	unsigned __int64 Read<>()
	{
		return Spry::IO::Networking::NetworkToHostOrder(BinaryReader::Read<unsigned __int64>());
	}

	template<>
	signed __int64 Read<>()
	{
		return Spry::IO::Networking::NetworkToHostOrder(BinaryReader::Read<signed __int64>());
	}
};


#endif