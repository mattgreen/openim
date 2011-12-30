#ifndef SPRY_ENVIRONMENT_HPP
#define SPRY_ENVIRONMENT_HPP

#include "String.hpp"

namespace Spry
{
	class Environment
	{
	public:
		static String GetWorkingDirectory()
		{
			TCHAR result[MAX_PATH+1] = { 0, };
			DWORD len = GetCurrentDirectory(MAX_PATH, result);
			return String(result, len);
		}

	private:
		Environment();
		~Environment();
	};
}

#endif