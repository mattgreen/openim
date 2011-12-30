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

#include "TLVReader.hpp"

TLVReader::TLVReader(NetworkBinaryReader& reader, SeekableStream& stream) :
	Type(),
	Length(),
	_reader(reader),
	_stream(stream),
	_offset(stream.Position)
{
}

int TLVReader::GetBytes(char* buffer, int bufferLength)
{
	int len = Length;
	if(bufferLength < len)
		len = bufferLength;

	return _stream.Read(buffer, bufferLength);
}

void TLVReader::MarkOffset(unsigned short length)
{
	_offset = _stream.Position + length;
}

bool TLVReader::Read()
{
	Reposition();

	if(_stream.Position < _stream.Length)
		_reader >> Type;

	if(_stream.Position < _stream.Length)
		_reader >> Length;
	MarkOffset(Length);

	return _stream.Position < _stream.Length;
}

void TLVReader::Reposition()
{
	if(_stream.Position != _offset)
	{
		_stream.Position = _offset;
	}
}
