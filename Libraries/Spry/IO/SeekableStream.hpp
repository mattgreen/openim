#ifndef SPRY_IO_SEEKABLESTREAM_HPP
#define SPRY_IO_SEEKABLESTREAM_HPP

#include "StreamOffset.hpp"
#include "Stream.hpp"

namespace Spry
{
namespace IO
{
	class SeekableStream :
		public virtual Stream
	{
	public:
		virtual StreamOffset	GetLength() = 0;
		virtual StreamOffset	GetPosition() = 0;
		virtual void			Seek(const StreamOffset& position) = 0;

		__declspec(property(get=GetLength))				StreamOffset	Length;
		__declspec(property(get=GetPosition, put=Seek))	StreamOffset	Position;
	};
}
}

#endif