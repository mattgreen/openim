#ifndef SPRY_OSEXCEPTION_HPP
#define SPRY_OSEXCEPTION_HPP

#include "Assert.hpp"
#include "Exception.hpp"
#include "OSHeaders.hpp"

namespace Spry
{
	struct OSException :
		public Exception
	{
		OSException(const char* name = "OSException") :
			Exception(name)
		{
			ErrorCode = GetLastError();
			unsigned long len = 
				FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
					0,
					ErrorCode,
					0,
					Description,
					sizeof(Description) / sizeof(char),
					0);
			Assert(len != 0);
			len;

			OutputDebugStringA(Name);
			OutputDebugStringA(": ");
			OutputDebugStringA(Description);
			//OutputDebugStringA("\r\n");
		}

		char			Description[1024];
		unsigned long	ErrorCode;
	};
}

#endif