#ifndef SPRY_IO_FILENOTFOUNDEXCEPTION_HPP
#define SPRY_IO_FILENOTFOUNDEXCEPTION_HPP

#include "IOException.hpp"

namespace Spry
{
namespace IO
{
	struct FileNotFoundException :
		public IOException
	{
		FileNotFoundException() :
			IOException("FileNotFoundException")
		{
		}
	};
}
}

#ifdef SPRY_AUTO_USING_DECLARATIONS
using Spry::IO::FileNotFoundException;
#endif

#endif