#ifndef SPRY_CONSOLE_HPP
#define SPRY_CONSOLE_HPP

#include <cstdio>
#include <fcntl.h>
#include <io.h>

#include "OSHeaders.hpp"

#include "Internal/Shims.hpp"
#include "Metaprogramming/Metaprogramming.hpp"

namespace Spry
{
	namespace Internal
	{
		class ConsoleImpl
		{
		public:
			template<typename T>
			static void Write(const T& obj)
			{
				WriteImpl(obj, Spry::Metaprogramming::IntToType<TypeTraits<T>::IsFundamental>());
			}

			template<typename T>
			static void WriteImpl(const T& obj, Spry::Metaprogramming::IntToType<true>)
			{
				printf(GetFormatString(obj), MakeStreamable(obj));
			}

			template<typename T>
			static void WriteImpl(const T& obj, Spry::Metaprogramming::IntToType<false>)
			{
				
			}
		};
	}

	class Console
	{
	public:
		static void Hide()
		{
			FreeConsole();
		}

		static void Show()
		{
			if(AllocConsole() != FALSE)
			{
				// set the screen buffer to be big enough to let us scroll text
				CONSOLE_SCREEN_BUFFER_INFO coninfo;
				::GetConsoleScreenBufferInfo(::GetStdHandle(STD_OUTPUT_HANDLE), &coninfo);
				coninfo.dwSize.Y = 500;
				::SetConsoleScreenBufferSize(::GetStdHandle(STD_OUTPUT_HANDLE), coninfo.dwSize);

				// redirect unbuffered STDOUT to the console
				intptr_t stdHandle = reinterpret_cast<intptr_t>(::GetStdHandle(STD_OUTPUT_HANDLE));
				int	conHandle = _open_osfhandle(stdHandle, _O_TEXT);
				FILE* file = _fdopen(conHandle, "w");
				*stdout = *file;
				setvbuf(stdout, NULL, _IONBF, 0);

				// redirect unbuffered STDIN to the console
				stdHandle = reinterpret_cast<intptr_t>(::GetStdHandle(STD_INPUT_HANDLE));
				conHandle = _open_osfhandle(stdHandle, _O_TEXT);
				file = _fdopen(conHandle, "r");
				*stdin = *file;
				setvbuf(stdin, NULL, _IONBF, 0);

				// redirect unbuffered STDERR to the console
				stdHandle = reinterpret_cast<intptr_t>(::GetStdHandle(STD_ERROR_HANDLE));
				conHandle = _open_osfhandle(stdHandle, _O_TEXT);
				file = _fdopen(conHandle, "w");
				*stderr = *file;
				setvbuf(stderr, NULL, _IONBF, 0);
			}
		}

		template<typename T>
		static void Write(const T& obj)
		{
			Internal::ConsoleImpl::Write(obj);
		}

		static void Write(const char* format, ...)
		{
			va_list args;
			va_start(args, format);
			vprintf(format, args);
			va_end(args);
		}

		template<typename T>
		static void WriteLine(const T& obj)
		{
			Write(obj);
			Write("\r\n");
		}

		static void WriteLine(const char* format, ...)
		{
			va_list args;
			va_start(args, format);
			vprintf(format, args);
			printf("\r\n");
			va_end(args);
		}
	};
}

#endif