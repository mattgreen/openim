#ifndef SPRY_DEBUG_HPP
#define SPRY_DEBUG_HPP

#include "OSHeaders.hpp"

namespace Spry
{
	namespace Internal
	{
		inline void DebugPrint(const char* text)
		{
			OutputDebugStringA(text);
		}

		inline void DebugPrint(const wchar_t* text)
		{
			OutputDebugStringW(text);
		}
	}

	class Debug
	{
	public:
		static void Break()
		{
			DebugBreak();
		}

		static bool Launch()
		{
			return DebugActiveProcess(GetCurrentProcessId()) != FALSE;
		}

		template<typename String>
		static void Write(const String& text)
		{
			Internal::DebugPrint(GetStringBuffer(text));
		}

		template<typename String>
		static void WriteLine(const String& text)
		{
			Write(text);
			Write("\r\n");
		}

	private:
		Debug();
		~Debug();
	};
}


#endif