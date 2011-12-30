#ifndef SPRY_IO_MEMORYSTREAM_HPP
#define SPRY_IO_MEMORYSTREAM_HPP

#pragma warning(push)
#pragma warning(disable:4702)
#include <vector>
#pragma warning(pop)

#include "SeekableStream.hpp"

namespace Spry
{
namespace IO
{
	class MemoryStream :
		public SeekableStream
	{
	public:
		explicit MemoryStream(int capacity = 1024) :
			_index(),
			_length()
		{
			_buffer.reserve(capacity);
		}

		void Clear()
		{
			_length = 0;
			_index = 0;
		}

		const char* GetBuffer() const
		{
			return &_buffer[0];
		}

		StreamOffset GetCapacity() const
		{
			return static_cast<StreamOffset>(_buffer.capacity());
		}

		StreamOffset GetLength()
		{
			return static_cast<StreamOffset>(_length);
		}

		StreamOffset GetPosition()
		{
			return _index;
		}

		int Read(char* buffer, int length)
		{
			int bytesToRead = static_cast<int>(GetLength());
			if(bytesToRead > length)
				bytesToRead = length;
			else if(bytesToRead <= 0)
				return 0;

			std::copy(_buffer.begin()+_index, _buffer.begin()+_index+bytesToRead, buffer);
			_index += bytesToRead;

			return bytesToRead;
		}

		void Seek(const StreamOffset& newPosition)
		{
			_index = static_cast<int>(newPosition);
		}

		int Write(const char* buffer, int length)
		{
			if(length == 0)
				return 0;

			int written = 0;
			int remaining = length;
			if(static_cast<size_t>(_index) < _buffer.size())
			{
				int bytesToCopy = length;
				if(static_cast<size_t>(_index + length) > _buffer.size())
					bytesToCopy = static_cast<int>(_buffer.size()) - _index;
				
				std::copy(buffer, buffer+bytesToCopy, _buffer.begin()+_index);
				
				written += bytesToCopy;
				remaining -= bytesToCopy;
				buffer += bytesToCopy;
				_index += bytesToCopy;
			}

			if(written != length && static_cast<size_t>(_index) == _buffer.size())
			{
				std::copy(buffer, buffer + remaining, std::back_inserter(_buffer));
				
				written += remaining;
				_index += remaining;
			}

			_length += written;

			return written;
		}

		__declspec(property(get=GetBuffer))				const char* Buffer;

	private:
		typedef std::vector<char>	BackingStore;

		BackingStore	_buffer;
		int				_index;
		int				_length;
		
	};
}
}

#endif