#ifndef SPRY_IO_STREAM_HPP
#define SPRY_IO_STREAM_HPP

namespace Spry
{
namespace IO
{
	class Stream
	{
	public:
		virtual int Read(char* buffer, int length) = 0;
		virtual int Write(const char* buffer, int length) = 0;
	};
}
}

#endif