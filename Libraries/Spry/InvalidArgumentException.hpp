#ifndef SPRY_INVALIDARGUMENTEXCEPTION_HPP
#define SPRY_INVALIDARGUMENTEXCEPTION_HPP

#include "Exception.hpp"

namespace Spry
{
	struct InvalidArgumentException :
		public Exception
	{
		InvalidArgumentException() :
			Exception("InvalidArgumentException")
		{
		}
	};
}


#endif