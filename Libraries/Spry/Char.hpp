#ifndef SPRY_CHAR_HPP
#define SPRY_CHAR_HPP

#include <cctype>

namespace Spry
{
	struct Char
	{
		template<typename T>
		static bool IsLowercase(T c)
		{
			return std::islower(c);
		}

		template<typename T>
		static bool IsUppercase(T c)
		{
			return std::isupper(c);
		}

		template<typename T>
		static bool IsWhitespace(T c)
		{
			return std::isspace(c) != 0;
		}

		template<typename T>
		static bool IsLetter(T c)
		{
			return std::isalpha(c) != 0;
		}

		template<typename T>
		static bool IsDigit(T c)
		{
			return std::isdigit(c) != 0;
		}

		template<typename T>
		static bool IsPunctuation(T c)
		{
			return std::ispunct(c) != 0;
		}

		template<typename T>
		static bool IsControl(T c)
		{
			return std::iscntrl(c) != 0;
		}

		template<typename T>
		static bool IsPrintable(T c)
		{
			return isprint(c) != 0;
		}

		template<typename T>
		static T ToLowercase(T c)
		{
			return static_cast<T>(std::tolower(c));
		}

		template<typename T>
		static T ToUppercase(T c)
		{
			return static_cast<T>(std::toupper(c));
		}

	private:
		~Char();
	};
};

#endif