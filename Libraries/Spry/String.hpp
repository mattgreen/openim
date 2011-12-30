#ifndef SPRY_STRING_HPP
#define SPRY_STRING_HPP

#include <tchar.h>

#include <string>
#include <vector>

namespace Spry
{
	typedef std::basic_string<TCHAR>	String;
	typedef std::vector<String>			StringList;
}

#endif