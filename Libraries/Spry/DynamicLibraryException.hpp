#ifndef SPRY_DYNAMICLIBRARYEXCEPTION_HPP
#define SPRY_DYNAMICLIBRARYEXCEPTION_HPP

#include "OSException.hpp"

namespace Spry
{
	struct DynamicLibraryException :
		public OSException
	{
		DynamicLibraryException() :
			OSException("DynamicLibraryException")
		{
		}
	};
}

#endif