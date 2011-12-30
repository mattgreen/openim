#ifndef SPRY_NEW_HPP
#define SPRY_NEW_HPP

#include <new>

#include "OutOfMemoryException.hpp"

namespace Spry
{
namespace Internal
{
	struct NewHandlerInstaller
	{
		NewHandlerInstaller()
		{
			std::set_new_handler(NewHandlerInstaller::OnOutOfMemory);
		}

		static void OnOutOfMemory()
		{
			throw Spry::OutOfMemoryException();	
		}
	};

	__declspec(selectany) NewHandlerInstaller newHandlerInstaller;
}
}

#endif