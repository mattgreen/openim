#ifndef SPRY_IO_DIRECTORY_HPP
#define SPRY_IO_DIRECTORY_HPP

#include "../OSHeaders.hpp"
#include "../String.hpp"
#include "../Utilities.hpp"

#include "IOException.hpp"

namespace Spry
{
namespace IO
{
	namespace Internal
	{
		inline BOOL CreateDirectoryImpl(const char* dir, SECURITY_ATTRIBUTES* sa)
		{
			return CreateDirectoryA(dir, sa);
		}

		inline BOOL CreateDirectoryImpl(const wchar_t* dir, SECURITY_ATTRIBUTES* sa)
		{
			return CreateDirectoryW(dir, sa);
		}

		inline void DispatchDirectoryError()
		{
			switch(GetLastError())
			{
			case 0:
			default:
				throw IOException();
			}
		}

		inline BOOL RemoveDirectoryImpl(const char* dir)
		{
			return RemoveDirectoryA(dir);
		}

		inline BOOL RemoveDirectoryImpl(const wchar_t* dir)
		{
			return RemoveDirectoryW(dir);
		}
	}

	class Directory
	{
	public:
		template<typename String>
		static void Create(const String& path)
		{
			if(Internal::CreateDirectoryImpl(Spry::Internal::GetStringBuffer(path), 0) == 0)
				Internal::DispatchDirectoryError();
		}

		template<typename String>
		static void Delete(const String& directory)
		{
			if(Internal::RemoveDirectoryImpl(Spry::Internal::GetStringBuffer(directory)) == 0)
				Internal::DispatchDirectoryError();
		}

		template<typename StringT1, typename StringT2>
		static StringList GetFiles(const StringT1& path, const StringT2& filter)
		{
			String pathToSearch = Spry::Internal::GetStringBuffer(path);
			if(! (pathToSearch[pathToSearch.length()-1] == Path::DirectorySeparator) &&
				 (pathToSearch[pathToSearch.length()-1] == Path::AltDirectorySeparator))
				pathToSearch += Path::DirectorySeparator;
			pathToSearch += Spry::Internal::GetStringBuffer(filter);

			StringList directories;

			WIN32_FIND_DATA data;
			HANDLE search = FindFirstFile(pathToSearch.c_str(), &data);
			if(search != INVALID_HANDLE_VALUE)
			{
				do
				{
					if((data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY)
						directories.push_back(data.cFileName);
				}
				while(FindNextFile(search, &data));
				FindClose(search);
			}

			return directories;
		}

		template<typename String>
		static StringList GetFiles(const String& path)
		{
			return GetFiles(path, "*");
		}

	private:
		Directory();
		~Directory();
	};
}
}

#endif