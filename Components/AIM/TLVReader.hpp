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

#ifndef AIM_TLVREADER_HPP
#define AIM_TLVREADER_HPP

#include "../../OpenIM/String.hpp"

#include "../../Utilities/NetworkBinaryReader.hpp"

#include "Spry.hpp"
#include "TLV.hpp"

class TLVReader
{
public:
	TLVReader(NetworkBinaryReader& reader, SeekableStream& stream);

public:
	template<typename T>
	TLVReader& operator>>(TLV<T>& tlv)
	{
		tlv.Type = Type;
		tlv.Length = Length;
		_reader >> tlv.Value;

		return *this;
	}

	template<int N>
	TLVReader& operator>>(char (& buffer)[N])
	{
		int bytes = Length;
		if(bytes > N)
		{
			Assert("Stack buffer not large enough for TLV data!" && 0);
			bytes = N;
		}
		_stream.Read(buffer, bytes);
		
		return *this;
	}

	template<int N>
	TLVReader& operator>>(TLV<char[N]>& tlv)
	{
		tlv.Type = Type;
		tlv.Length = Length;
		*this >> tlv.Value;

		return *this;
	}

	template<typename T>
	TLVReader& operator>>(T& value)
	{
		value = Get<T>();

		return *this;
	}

public:
	template<typename T>
	T Get()
	{
		TLV<T> value;
		*this >> value;
		return value.Value;
	}

	int GetBytes(char* buffer, int bufferLength);
	
	template<typename Encoding>
	String GetString()
	{
		return _reader.ReadString<Encoding>(Length);
	}

	bool Read();

public:
	unsigned short Type;
	unsigned short Length;

private:
	void MarkOffset(unsigned short length);
	void Reposition();

private:
	TLVReader(const TLVReader&);
	TLVReader& operator=(const TLVReader&);

private:
	NetworkBinaryReader&	_reader;
	SeekableStream&			_stream;
	StreamOffset			_offset;
};

#endif