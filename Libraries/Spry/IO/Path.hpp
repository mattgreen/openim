#ifndef SPRY_IO_PATH_HPP
#define SPRY_IO_PATH_HPP

#include "../Char.hpp"
#include "../String.hpp"

#include "../Metaprogramming/StringTraits.hpp"

namespace Spry
{
namespace IO
{
	class Path
	{
	public:
		static const TCHAR VolumeSeparator = TEXT(':');
		static const TCHAR DirectorySeparator = TEXT('\\');
		static const TCHAR AltDirectorySeparator = TEXT('/');
		static const TCHAR PathSeparator = TEXT(';');

		template<typename String1, typename String2>
		static String Combine(const String1& path1, const String2& path2)
		{
			using Spry::Metaprogramming::StringTraits;

			int path1Len = StringTraits::GetLength(Spry::Internal::GetStringBuffer(path1));
			int path2Len = StringTraits::GetLength(Spry::Internal::GetStringBuffer(path2));

			if(path1Len == 0)
				return String(Spry::Internal::GetStringBuffer(path2), path2Len);

			if(path2Len == 0)
				return String(Spry::Internal::GetStringBuffer(path1), path1Len);

			TCHAR c = path1[path1Len - 1];
			if(c != VolumeSeparator && c != DirectorySeparator && c != AltDirectorySeparator)
				return String(Spry::Internal::GetStringBuffer(path1)) + DirectorySeparator + String(Spry::Internal::GetStringBuffer(path2));
			else
				return String(Spry::Internal::GetStringBuffer(path1)) + String(Spry::Internal::GetStringBuffer(path2));
		}

		static String GetDrive(const String& path)
		{
			String::size_type volumeIndex = path.find(VolumeSeparator);
			if(volumeIndex != String::npos)
			{
				String drive;
				drive += Char::ToUppercase(path[volumeIndex-1]);
				return drive;
			}
			else
			{
				if(path[0] == DirectorySeparator || path[0] == AltDirectorySeparator)
				{
					String result;
					result += DirectorySeparator;
					return result;
				}
				else
					return TEXT("");
			}
		}

		static String GetExtension(const String& path)
		{
			String::size_type pos = path.find_last_of(TEXT('.'));
			if(pos == -1)
				return String();

			return path.substr(pos+1);
		}

		static String GetFileName(const String& path)
		{
			String::size_type pos = path.find_last_of(DirectorySeparator);
			if(pos == -1)
				pos = path.find_last_of(AltDirectorySeparator);

			if(pos == -1)
				return String();
	
			return path.substr(pos + 1);
		}

		static String GetFileNameWithoutExtension(const String& path)
		{
			String filename = GetFileName(path);

			String::size_type pos = filename.find_last_of(TEXT('.'));
			if(pos != String::npos)
				filename = filename.substr(0, pos);

			return filename;
		}

		static bool IsAbsolute(const String& path)
		{
			String drive = GetDrive(path);
			
			return ! drive.empty() &&
				drive != String(&DirectorySeparator, 1) && 
				drive != String(&AltDirectorySeparator, 1);
		}



	private:
		Path();
	};
}
}

#endif