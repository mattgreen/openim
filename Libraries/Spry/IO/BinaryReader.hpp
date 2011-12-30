#ifndef SPRY_IO_BINARYREADER_HPP
#define SPRY_IO_BINARYREADER_HPP

#include <string>
#include <vector>

#include "../Buffer.hpp"
#include "../Encoding.hpp"
#include "../WCharBuffer.hpp"

#include "EndOfStreamException.hpp"
#include "Stream.hpp"

namespace Spry
{
namespace IO
{
	class BinaryReader
	{
	public:
		BinaryReader(Stream& stream) :
			_stream(&stream)
		{
		}

		Stream& GetBaseStream()
		{
			return *_stream;
		}

		template<typename T>
		BinaryReader& operator>>(T& r)
		{
			r = Read<T>();
			return *this;
		}

		template<typename T>
		T Read();

		template<>
		bool Read()
		{
			bool b = false;
			ReadBytes(reinterpret_cast<char*>(&b), sizeof(b));
			return b;
		}

		template<>
		char Read()
		{
			char c = 0;
			ReadBytes(static_cast<char*>(&c), 1);
			return c;
		}

		template<>
		signed char Read()
		{
			return static_cast<signed char>(Read<char>());
		}

		template<>
		unsigned char Read()
		{
			return static_cast<unsigned char>(Read<char>());
		}

		template<>
		signed short Read()
		{
			signed short n = 0;
			ReadBytes(reinterpret_cast<char*>(&n), sizeof(n));
			return n;
		}

		template<>
		unsigned short Read()
		{
			return static_cast<unsigned short>(Read<signed short>());
		}

		template<>
		signed int Read()
		{
			signed int n = 0;
			ReadBytes(reinterpret_cast<char*>(&n), sizeof(n));
			return n;
		}

		template<>
		unsigned int Read()
		{
			return static_cast<unsigned int>(Read<signed int>());
		}

		template<>
		signed long Read()
		{
			signed long n = 0;
			ReadBytes(reinterpret_cast<char*>(&n), sizeof(n));
			return n;
		}

		template<>
		unsigned long Read()
		{
			return static_cast<unsigned long>(Read<signed long>());
		}

		template<>
		signed __int64 Read()
		{
			signed __int64 n = 0;
			ReadBytes(reinterpret_cast<char*>(&n), sizeof(n));
			return n;
		}

		template<>
		unsigned __int64 Read()
		{
			return static_cast<unsigned __int64>(Read<signed __int64>());
		}

		template<>
		float Read()
		{
			char buffer[sizeof(float)];
			ReadBytes(buffer, sizeof buffer);

			return *reinterpret_cast<float*>(&buffer);
		}

		template<>
		double Read()
		{
			char buffer[sizeof(double)];
			ReadBytes(buffer, sizeof buffer);

			return *reinterpret_cast<double*>(&buffer);
		}

		void ReadBytes(char* buffer, int length)
		{
			// This comes up more than you'd think.
			if(length == 0)
				return;

			int read = _stream->Read(buffer, length);
			if(read != length)
				throw EndOfStreamException();
		}

		template<typename Encoding, int N>
		void ReadChars(WCharBuffer<N>& buffer, int bytesToRead)
		{
			wchar_t* buf = buffer;
			buffer.Length = ReadChars<Encoding>(buf, N, bytesToRead);
		}

		template<typename Encoding>
		int ReadChars(wchar_t* buffer, int bufferSize, int bytesToRead)
		{
			// Avoid belated pessimization: this stack-allocated buffer
			// should service most read requests without any need to hit the heap.
			char rawBuffer[8192];
			char* input = rawBuffer;

			// Of course, we want to be able to handle larger sizes as well.
			std::vector<char> rawBufferStore;
			if(bytesToRead > sizeof(rawBuffer))
			{
				rawBufferStore.resize(bytesToRead);
				input = &rawBufferStore[0];
			}

			// Read the bytes in from the attached stream, then determine how many
			// characters can be extracted. I'm not aware of any encodings that produce
			// more characters than provided bytes, but we cover this case for safety's sake.
			ReadBytes(input, bytesToRead);
			int chars = Encoding::GetCharCount(input, bytesToRead);
			if(chars > bufferSize)
				chars = bufferSize;

			int newLen = Encoding::GetChars(input, bytesToRead, buffer, chars);
			
			return newLen;
		}

		template<typename Encoding>
		std::wstring ReadString(int bytesToRead)
		{
			char rawBuffer[8192];
			char* bytes = rawBuffer;

			std::vector<char> growableRawBuffer;
			if(bytesToRead > sizeof(rawBuffer))
			{
				growableRawBuffer.resize(bytesToRead);
				bytes = &growableRawBuffer[0];
			}

			ReadBytes(bytes, bytesToRead);
			int chars = Encoding::GetCharCount(bytes, bytesToRead);
			
			std::wstring result;
			result.resize(chars);
			Encoding::GetChars(bytes, bytesToRead, &result[0], chars);
			return result;
		}


public:
		__declspec(property(get=GetBaseStream))	Stream&	BaseStream;

private:
		int ReadStringLength()
		{
			int length = 0;
			int shift = 0;
			unsigned char b = 0;

			do
			{
				b = Read<unsigned char>();
				length |= (b & 0x7F) << shift;
				shift += 7;
			}
			while ((b & 0x80) != 0);

			return length;
		}


	private:
		Stream*	_stream;
	};
}
}


#endif