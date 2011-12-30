#ifndef SPRY_WCHARBUFFER_HPP
#define SPRY_WCHARBUFFER_HPP

#include "Buffer.hpp"

namespace Spry
{
	template<int Size>
	struct WCharBuffer :
		public Buffer<Size, wchar_t>
	{
		WCharBuffer()
		{
		}

		WCharBuffer(const wchar_t* buffer) :
			Buffer<Size, wchar_t>(buffer)
		{
		}

		WCharBuffer(const wchar_t* buffer, int length) :
			Buffer<Size, wchar_t>(buffer, length)
		{
		}
	};
}

#endif