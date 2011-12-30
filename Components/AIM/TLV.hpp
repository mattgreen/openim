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

#ifndef AIM_TLV_HPP
#define AIM_TLV_HPP

namespace Internal
{
	template<typename T>
	struct Writer
	{
		template<typename Stream>
		static Stream& Write(Stream& s, int, const T& data)
		{
			s << data;

			return s;
		}
	};

	template<>
	struct Writer<char*>
	{
		template<typename Stream>
		static Stream& Write(Stream& s, int length, const char* data)
		{
			s.WriteBytes(data, length);
			return s;
		}
	};

	template<>
	struct Writer<const char*>
	{
		template<typename Stream>
		static Stream& Write(Stream& s, int length, const char* const& data)
		{
			for(int i = 0; i < length; i++)
				s << data[i];
			return s;
		}
	};
}

	template<typename T>
	struct TLV
	{
		TLV() :
			Type(),
			Length(),
			Value()
		{
		}

		explicit TLV(unsigned short type) :
			Type(type),
			Length(sizeof(T))
		{
		}

		TLV(unsigned short type, unsigned short length) :
			Type(type),
			Length(length)
		{
		}

		template<typename U>
		TLV(unsigned short type, unsigned short length, U value) :
			Type(type),
			Length(length),
			Value(value)
		{
		}
		
		unsigned short	Type;
		unsigned short	Length;
		T				Value;
	};

	typedef TLV<char[16]>	ScreenNameTLV;


template<typename Stream, typename T>
Stream& operator<<(Stream& s, const TLV<T>& tlv)
{
	s << tlv.Type;
	s << tlv.Length;
	return ::Internal::Writer<T>::Write(s, tlv.Length, tlv.Value);
}

template<typename Stream, typename T, int N>
Stream& operator<<(Stream& s, const TLV<T[N]>& tlv)
{
	s << tlv.Type;
	s << tlv.Length;
	return ::Internal::Writer<T*>::Write(s, tlv.Length, tlv.Value);
}

template<typename Stream, typename T>
Stream& operator>>(Stream& s, const TLV<T>& tlv)
{
	s >> tlv.Type;
	s >> tlv.Length;
	s >> tlv.Value;

	return s;
}

#endif