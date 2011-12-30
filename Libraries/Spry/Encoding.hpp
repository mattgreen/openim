#ifndef SPRY_ENCODING_HPP
#define SPRY_ENCODING_HPP

#include "Buffer.hpp"
#include "Internal/Shims.hpp"
#include "OSHeaders.hpp"

namespace Spry
{
namespace Internal
{
	template<typename T, int CP>
	struct EncodingBase
	{
		enum
		{
			CodePage = CP
		};

		static bool CanMap(const wchar_t* s)
		{
			return T::CanMap(s, -1);
		}

		static bool CanMap(const wchar_t* s, int chars)
		{
			BOOL usedDefaultChar = FALSE;
			WideCharToMultiByte(CodePage,
				0,
				s,
				chars,
				0,
				0,
				0,
				&usedDefaultChar);

			return usedDefaultChar == FALSE;
		}

		template<typename String>
		static bool CanMap(const String& s)
		{
			return T::CanMap(GetStringBuffer(s), GetStringLength(s));
		}

		template<typename String, int N>
		static int GetBytes(const String& s, char destination[N])
		{
			return T::GetBytes(s, destination, N);
		}

		template<typename String>
		static int GetBytes(const String& s, char* destination, int destinationBytes)
		{
			return T::GetBytes(GetStringBuffer(s), GetStringLength(s), destination, destinationBytes);
		}

		static int GetBytes(const wchar_t* input, char* destination, int destinationBytes)
		{
			return T::GetBytes(input, -1, destination, destinationBytes);
		}

		static int GetBytes(const wchar_t* input, int inputChars, char* destination, int destinationBytes)
		{
			if(inputChars == 0)
				return 0;

			int length = WideCharToMultiByte(CodePage,
				0,
				input,
				inputChars,
				destination,
				destinationBytes,
				0,
				0);

			if(length == 0)
				throw OSException();

			return length;
		}

		static int GetByteCount(const wchar_t* buffer, int len)
		{
			return WideCharToMultiByte(CodePage, 0, buffer, len, 0, 0, 0, 0);
		}

		template<typename String>
		static int GetByteCount(const String& s)
		{
			return T::GetByteCount(GetStringBuffer(s), GetStringLength(s));
		}

		static int GetCharCount(const unsigned char* buffer, int length)
		{
			return T::GetCharCount(reinterpret_cast<const char*>(buffer), length);
		}

		static int GetCharCount(const char* buffer, int length)
		{
			return MultiByteToWideChar(CodePage, 0, reinterpret_cast<LPCSTR>(buffer), length, 0, 0);
		}

		static int GetChars(const char* buffer, int bufferLength, wchar_t* output, int outputChars)
		{
			if(bufferLength == 0)
				return 0;

			int length = MultiByteToWideChar(
				CodePage,
				0,
				buffer,
				bufferLength,
				output,
				outputChars);

			if(length == 0)
				throw OSException();

			return length;
		}

		template<typename String>
		static int GetString(const char* buffer, int bufferLength, String& s)
		{
			int length = T::GetCharCount(buffer, bufferLength);
			s.resize(length);
			T::GetChars(buffer, bufferLength, &s[0], length);
			return length;
		}

	protected:
		typedef EncodingBase<T, CP> Base;
	};

	template<typename T, int CP>
	struct UnicodeEncodingBase :
		public EncodingBase<T, CP>
	{
		static int GetBytes(const wchar_t* input, int inputChars, char* destination, int destinationBytes)
		{
			int bytes = T::GetByteCount(input, inputChars);

			int len = bytes;
			if(destinationBytes < bytes)
				len = destinationBytes;

			int chars = len / sizeof(wchar_t);
			for(int i = 0; i < chars; i++)
				T::DoGetByte(input[i], &destination[i*2]);
			
			return len;
		}
		using Base::GetBytes;

		static int GetByteCount(const wchar_t*, int len)
		{
			return len * sizeof(wchar_t);
		}
		using Base::GetByteCount;

		static int GetCharCount(const char*, int length)
		{
			Assert(length % 2 == 0);

			return length / 2;
		}
		using Base::GetCharCount;

		static int GetChars(const char* buffer, int bufferLength, wchar_t* output, int outputChars)
		{
			int chars = T::GetCharCount(buffer, bufferLength);

			int len = chars;
			if(outputChars < chars)
				len = outputChars;

			for(int i = 0; i < len; i++)
			{
				output[i] = T::DoGetChar(buffer);
				buffer += 2;
			}

			return chars;
		}
		using Base::GetChars;
	};
}
	class Encoding
	{
	public:
		class ASCII :
			public Spry::Internal::EncodingBase<ASCII, 20127>
		{
		public:
			static int GetByteCount(const wchar_t*, int len)
			{
				return len;
			}
			using Base::GetByteCount;

			static int GetCharCount(const char*, int length)
			{
				return length;
			}
			using Base::GetCharCount;

			static int GetChars(const char* buffer, int bufferLength, wchar_t* output, int outputChars)
			{
				Assert(buffer != 0);
				Assert(bufferLength >= 0);
				Assert(output != 0);
				Assert(outputChars >= 0);

				int chars = outputChars;
				if(chars > bufferLength)
					chars = bufferLength;
				
				for(int i = 0; i < chars; i++)
					output[i] = static_cast<wchar_t>(buffer[i]);

				return chars;
			}
			using Base::GetChars;

		};

		class Default :
			public Spry::Internal::EncodingBase<Default, CP_ACP>
		{
		};

		class ISO88591 :
			public Spry::Internal::EncodingBase<ISO88591, 28591>
		{
		};

		class UTF7 :
			public Spry::Internal::EncodingBase<UTF7, CP_UTF7>
		{
		};

		class UTF8 :
			public Spry::Internal::EncodingBase<UTF8, CP_UTF8>
		{
		};

		class UTF16BE :
			public Spry::Internal::UnicodeEncodingBase<UTF16BE, 1201>
		{
		public:
			static void DoGetByte(wchar_t input, char* output)
			{
				output[0] = static_cast<char>(((input & 0xFF00) >> 8));
				output[1] = static_cast<char>(input & 0xFF);
			}

			static wchar_t DoGetChar(const char* input)
			{
				return static_cast<wchar_t>((input[0] << 8) | input[1]);
			}
		};

		class UTF16LE :
			public Spry::Internal::UnicodeEncodingBase<UTF16LE, 1200>
		{
		public:
			static void DoGetByte(wchar_t input, char* output)
			{
				output[0] = static_cast<char>(input & 0xFF);
				output[1] = static_cast<char>((input >> 8) & 0xFF);
			}

			static wchar_t DoGetChar(const char* input)
			{
				return static_cast<wchar_t>((input[1] << 8) | (input[0]));
			}
		};
	};
}

#endif