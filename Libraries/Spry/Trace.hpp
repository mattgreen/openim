#ifndef SPRY_TRACE_HPP
#define SPRY_TRACE_HPP

#include <stdlib.h>
#include <stdarg.h>

#include "Console.hpp"

inline void TraceHexDump(const char* buffer, int length)
{
	for(int i = 0; i < length; i++)
	{
		printf("%02x ", (unsigned char)buffer[i]);
		if(i > 0 && i % 16 == 0)
			printf("\n");
	}

	printf("\n");
}

inline void Trace(const char* format, ...)
{
	Spry::Console::Show();

	va_list args;

	va_start(args, format);
	vprintf(format, args);
	va_end(args);
}

#endif