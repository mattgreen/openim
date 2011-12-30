#ifndef SPRY_IO_FILE_HPP
#define SPRY_IO_FILE_HPP

#include "../OSException.hpp"
#include "../OSHeaders.hpp"
#include "../Utilities.hpp"

#include "FileNotFoundException.hpp"
#include "IOException.hpp"

namespace Spry
{
namespace IO
{
	namespace Internal
	{
		inline BOOL CopyFileImpl(const char* source, const char* dest, BOOL failIfExists)
		{
			return CopyFileA(source, dest, failIfExists);
		}

		inline BOOL CopyFileImpl(const wchar_t* source, const wchar_t* dest, BOOL failIfExists)
		{
			return CopyFileW(source, dest, failIfExists);
		}

		inline BOOL DeleteFileImpl(const char* file)
		{
			return DeleteFileA(file);
		}

		inline BOOL DeleteFileImpl(const wchar_t* file)
		{
			return DeleteFileW(file);
		}

		inline void DispatchFileError()
		{
			switch(GetLastError())
			{
			case ERROR_FILE_NOT_FOUND:
				throw FileNotFoundException();
				break;

			default:
				throw IOException();
			}
		}

		inline DWORD GetFileAttributesImpl(const char* filename)
		{
			return GetFileAttributesA(filename);
		}

		inline DWORD GetFileAttributesImpl(const wchar_t* filename)
		{
			return GetFileAttributesW(filename);
		}

		inline BOOL MoveFileImpl(const char* source, const char* dest)
		{
			return MoveFileA(source, dest);
		}

		inline BOOL MoveFileImpl(const wchar_t* source, const wchar_t* dest)
		{

			return MoveFileW(source, dest);
		}
	}

	class File
	{
	public:
		template<typename String>
		static void Copy(const String& source, const String& destination, bool overwrite = false)
		{
			BOOL fail = overwrite ? FALSE : TRUE;
			if(Internal::CopyFileImpl(Spry::Internal::GetStringBuffer(source), Spry::Internal::GetStringBuffer(destination), fail) == 0)
				Internal::DispatchFileError();
		}

		template<typename String1, typename String2>
		static void Copy(const String1& source, const String2& destination, bool overwrite = false)
		{
			return Copy(Spry::Internal::GetStringBuffer(source), Spry::Internal::GetStringBuffer(destination), overwrite);
		}

		template<typename String>
		static void Delete(const String& file)
		{
			if(! Internal::DeleteFileImpl(Spry::Internal::GetStringBuffer(file)))
				Internal::DispatchFileError();
		}

		template<typename String>
		static bool Exists(const String& file)
		{
			return
				GetFileAttributes(Spry::Internal::GetStringBuffer(file)) != INVALID_FILE_ATTRIBUTES &&
				GetLastError() != ERROR_FILE_NOT_FOUND;
		}

		template<typename String>
		static void Move(const String& source, const String& destination)
		{
			if(Internal::MoveFileImpl(Spry::Internal::GetStringBuffer(source), Spry::Internal::GetStringBuffer(destination)) == 0)
				Internal::DispatchFileError();
		}

		template<typename String1, typename String2>
		static void Move(const String1& source, const String2& destination)
		{
			return Move(Spry::Internal::GetStringBuffer(source), Spry::Internal::GetStringBuffer(destination));
		}

	private:
		File();
		~File();
	};
}
}

#ifdef SPRY_AUTO_USING_DECLARATIONS
using Spry::IO::File;
#endif

#endif