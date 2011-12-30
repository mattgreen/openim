#ifndef SPRY_DRAWING_COLOR_HPP
#define SPRY_DRAWING_COLOR_HPP

#include "../OSHeaders.hpp"
#include "../Spry.hpp"

namespace Spry
{
namespace Drawing
{
	struct Color
	{
		explicit Color(unsigned int color = 0)
		{
			Blue = static_cast<unsigned char>((color & 0xFF0000) >> 16);
			Green = static_cast<unsigned char>((color & 0x00FF00) >> 8);
			Red = static_cast<unsigned char>(color & 0x0000FF);
		}

		explicit Color(unsigned char red, unsigned char green, unsigned char blue) :
			Red(red),
			Green(green),
			Blue(blue)
		{
		}

		operator COLORREF()
		{
			return RGB(Red, Green, Blue);
		}

		unsigned char	Red;
		unsigned char	Green;
		unsigned char	Blue;
	};

	template<typename T>
	T& operator>>(T& in, Color& c)
	{
		in >> c.Red;
		in >> c.Green;
		in >> c.Blue;

		return in;
	}

	template<typename T>
	T& operator<<(T& out, const Color& c)
	{
		out << c.Red;
		out << c.Green;
		out << c.Blue;

		return out;
	}
}
}


#endif