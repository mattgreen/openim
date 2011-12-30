#ifndef SPRY_UI_ICONLOADEXCEPTION_HPP
#define SPRY_UI_ICONLOADEXCEPTION_HPP

#include "../OSException.hpp"

namespace Spry
{
namespace UI
{
	struct IconLoadException :
		public Spry::OSException
	{
	public:
		IconLoadException() :
		  Spry::OSException("IconLoadException")
		{
		}
	};
}
}


#endif