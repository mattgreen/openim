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

#ifndef OPENIM_UTILITIES_NETWORKBINARYWRITER_HPP
#define OPENIM_UTILITIES_NETWORKBINARYWRITER_HPP

#include "../Libraries/Spry/IO/BinaryWriter.hpp"
#include "../Libraries/Spry/IO/Networking/Networking.hpp"

class NetworkBinaryWriter
{
public:
	NetworkBinaryWriter(Spry::IO::Stream& stream) :
		_writer(stream)
	{
	}

	template<typename T>
	NetworkBinaryWriter& operator<<(const T& t)
	{
		Write<T>(t);
		return *this;
	}

	template<typename T>
	void Write(const T& obj)
	{
		::operator<<(*this, obj);
	}

	template<>
	void Write<char>(const char& n)
	{
		_writer.Write<char>(n);
	}

	template<>
	void Write<unsigned char>(const unsigned char& n)
	{
		_writer.Write<unsigned char>(n);
	}

	template<>
	void Write<signed char>(const signed char& n)
	{
		_writer.Write<signed char>(n);
	}

	template<>
	void Write<unsigned short>(const unsigned short& n)
	{
		_writer.Write<unsigned short>(Spry::IO::Networking::HostToNetworkOrder(n));
	}

	template<>
	void Write<signed short>(const signed short& n)
	{
		_writer.Write<signed short>(Spry::IO::Networking::HostToNetworkOrder(n));
	}

	template<>
	void Write<unsigned int>(const unsigned int& n)
	{
		_writer.Write<unsigned int>(Spry::IO::Networking::HostToNetworkOrder(n));
	}

	template<>
	void Write<signed int>(const signed int& n)
	{
		_writer.Write<signed int>(Spry::IO::Networking::HostToNetworkOrder(n));
	}

	template<>
	void Write<unsigned long>(const unsigned long& n)
	{
		_writer.Write<unsigned long>(Spry::IO::Networking::HostToNetworkOrder(n));
	}

	template<>
	void Write<signed long>(const signed long& n)
	{
		_writer.Write<signed long>(Spry::IO::Networking::HostToNetworkOrder(n));
	}

	void WriteBytes(const char* data, int length)
	{
		_writer.WriteBytes(data, length);
	}

	template<typename Encoding>
	void WriteChars(const wchar_t* buffer, int length)
	{
		_writer.WriteChars<Encoding>(buffer, length);
	}

private:
	Spry::IO::BinaryWriter _writer;
};


#endif