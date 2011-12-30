#ifndef SPRY_EXCEPTION_HPP
#define SPRY_EXCEPTION_HPP

namespace Spry
{
	struct Exception
	{
		Exception(const char* name) :
			Name(name)
		{
		}

		const char* Name;
	};
}

#endif