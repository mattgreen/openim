#ifndef SPRY_BUFFER_HPP
#define SPRY_BUFFER_HPP

#include "Spry.hpp"

#include "Internal/Shims.hpp"

#include "Assert.hpp"
#include "BufferOverflowException.hpp"
#include "InvalidArgumentException.hpp"
#include <memory.h>

namespace Spry
{
	template<int Size, typename T = char>
	struct Buffer
	{
	public:
		enum
		{
			Capacity = Size
		};

		Buffer()
		{
			Zero();
		}

		Buffer(const T* buffer)
		{
			Assign(buffer);
		}

		Buffer(const T* buffer, int length)
		{
			Assign(buffer, length);
		}

		T& operator[](int i)
		{
			Assert(i >= 0 && i < Capacity);

			return _data[i];
		}

		operator const T*() const
		{
			return _data;
		}

		operator T*()
		{
			return _data;
		}

		void Append(T c)
		{
			Enforce<BufferOverflowException>((Length+1) <= Capacity);

			_data[Length++] = c;
		}

		void Assign(const T* buffer)
		{
			Assign(buffer, Spry::Internal::GetStringLength(buffer));
		}

		void Assign(const T* buffer, int len)
		{
			Assert(len >= 0);
			Assert(len <= Capacity);

			Zero();
			Length = len;
			memcpy(_data, buffer, len * sizeof(T));
		}

		void Write(const T* buffer, int len)
		{
			Assert(len > 0);
			Enforce<BufferOverflowException>((Length+len) <= Capacity);

			memcpy(_data+Length, buffer, len * sizeof(T));
			Length += len;
		}

		void Zero()
		{
			Length = 0;
			memset(_data, 0, (Size+1) * sizeof(T));
		}

	public:
		int Length;

	protected:
		T _data[Size+1];
	};
}

#endif