#ifndef SPRY_IO_BINARYWRITER_HPP
#define SPRY_IO_BINARYWRITER_HPP

#include "../Internal/Shims.hpp"
#include "../Metaprogramming/StringTraits.hpp"

#include "IOException.hpp"
#include "Stream.hpp"

namespace Spry
{
namespace IO
{
	class BinaryWriter
	{
	public:
		BinaryWriter(Stream& stream) :
			_stream(&stream)
		{
		}

		template<typename T>
		BinaryWriter& operator<<(const T& item)
		{
			Write<T>(item);

			return *this;
		}

		template<typename T>
		void Write(const T& obj)
		{
			::operator<<(*this, obj);
		}

		template<>
		void Write(const bool& b)
		{
			WriteBytes((char*)&b, sizeof(b));
		}

		template<>
		void Write(const char& c)
		{
			WriteBytes(&c, sizeof(c));
		}

		template<>
		void Write(const signed char& n)
		{
			WriteBytes((char*)&n, sizeof(n));
		}

		template<>
		void Write(const unsigned char& n)
		{
			WriteBytes((char*)&n, sizeof(n));
		}

		template<>
		void Write(const signed short& n)
		{
			WriteBytes((char*)&n, sizeof(n));
		}

		template<>
		void Write(const unsigned short& n)
		{
			WriteBytes((char*)&n, sizeof(n));
		}

		template<>
		void Write(const signed int& n)
		{
			WriteBytes((char*)&n, sizeof(n));
		}

		template<>
		void Write(const unsigned int& n)
		{
			WriteBytes((char*)&n, sizeof(n));
		}

		template<>
		void Write(const signed long& n)
		{
			WriteBytes((char*)&n, sizeof(n));
		}

		template<>
		void Write(const unsigned long& n)
		{
			WriteBytes((char*)&n, sizeof(n));
		}

		template<>
		void Write(const float& n)
		{
			WriteBytes((char*)&n, sizeof(n));
		}

		template<>
		void Write(const double& n)
		{
			WriteBytes((char*)&n, sizeof(n));
		}

		void WriteBytes(const char* buffer, int length)
		{
			int written = _stream->Write(buffer, length);
			if(written != length)
				throw IOException();
		}

		template<typename Encoding>
		void WriteChars(const wchar_t* buffer, int length)
		{
			char smallBuffer[16384];
			std::vector<char> bigBuffer;

			char* storage = smallBuffer;

			int len = Encoding::GetByteCount(buffer, length);
			if(len > sizeof(smallBuffer))
			{
				bigBuffer.resize(len);
				storage = &bigBuffer[0];
			}

			len = Encoding::GetBytes(buffer, length, storage, len);
			WriteBytes(storage, len);
		}

	private:
		Stream*	_stream;
	};
}
}


#endif