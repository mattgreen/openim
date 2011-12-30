#ifndef SPRY_IO_FILESTREAM_HPP
#define SPRY_IO_FILESTREAM_HPP

#include "../Internal/FileHandle.hpp"
#include "../Utilities.hpp"

#include "IOException.hpp"
#include "Stream.hpp"

namespace Spry
{
namespace IO
{
	namespace Internal
	{
		inline HANDLE CreateFileImpl(const char* file,
			DWORD access, 
			DWORD share, 
			LPSECURITY_ATTRIBUTES attributes,
			DWORD createDisposition,
			DWORD flags,
			HANDLE templateFile)
		{
			return CreateFileA(file, access, share, attributes, createDisposition, flags, templateFile);
		}

		inline HANDLE CreateFileImpl(const wchar_t* file,
			DWORD access, 
			DWORD share, 
			LPSECURITY_ATTRIBUTES attributes,
			DWORD createDisposition,
			DWORD flags,
			HANDLE templateFile)
		{
			return CreateFileW(file, access, share, attributes, createDisposition, flags, templateFile);
		}
	}

	struct FileMode
	{
		enum
		{
			Open = 1,
			Create = 2
		};
	};

	struct FileAccess
	{
		enum
		{
			Read = GENERIC_READ,
			Write = GENERIC_WRITE,
			ReadWrite = Read | Write
		};
	};

	struct ShareMode
	{
		enum
		{
			None = 0,
			Read = FILE_SHARE_READ,
			Write = FILE_SHARE_WRITE
		};
	};

	class FileStream :
		public Stream
	{
	public:
		template<typename String>
		FileStream(const String& filename, int access = FileAccess::Read, int fileMode = FileMode::Open, int shareMode = ShareMode::None)
		{
			DWORD disposition = 0;
			switch(fileMode)
			{
			case FileMode::Create:
				disposition = CREATE_NEW;
				break;

			case FileMode::Open:
				disposition = OPEN_EXISTING;
				break;

			case FileMode::Create | FileMode::Open:
				disposition = OPEN_ALWAYS;
				break;
			};

			_handle = Internal::CreateFileImpl(
				Spry::Internal::GetStringBuffer(filename),
				access,
				shareMode,
				0,
				disposition,
				FILE_ATTRIBUTE_NORMAL,
				0);
			
			if(! _handle.IsValid())
				throw IOException();
		}

	public:
		void Close()
		{
			_handle.Close();
		}

		HANDLE GetHandle()
		{
			return _handle;
		}

		unsigned long GetLength()
		{
			unsigned long size = GetFileSize(_handle, 0);
			if(size == INVALID_FILE_SIZE)
				throw IOException();

			return size;
		}

		unsigned long GetPosition()
		{
			DWORD pos = SetFilePointer(_handle, 0, 0, FILE_CURRENT);
			if(pos == INVALID_SET_FILE_POINTER)
				throw IOException();

			return pos;
		}

		int Read(char* buffer, int length)
		{
			DWORD bytesRead = 0;
			BOOL result = ReadFile(_handle, buffer, length, &bytesRead, 0);
			if(result == FALSE)
				throw IOException();
			return bytesRead;
		}

		void Seek(long position)
		{
			unsigned long result = SetFilePointer(_handle, position, 0, FILE_BEGIN);
			if(result == INVALID_FILE_SIZE)
				throw IOException();
		}

		int Write(const char* buffer, int length)
		{
			DWORD bytesWritten = 0;
			BOOL result = WriteFile(_handle, buffer, length, &bytesWritten, 0);
			if(result == FALSE)
				throw IOException();
			return bytesWritten;
		}

		__declspec(property(get=GetLength))				unsigned long	Length;
		__declspec(property(get=GetPosition, put=Seek))	unsigned long	Position;

	private:
		Spry::Internal::FileHandle _handle;
	};
}
}

#endif