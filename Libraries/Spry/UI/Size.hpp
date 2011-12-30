#ifndef SPRY_UI_SIZE_HPP
#define SPRY_UI_SIZE_HPP

namespace Spry
{
namespace UI
{
	struct Size
	{
	public:
		Size(int width = 0, int height = 0) :
			Width(width),
			Height(height)
		{
		}

		int Width;
		int Height;
	};
}
}

#endif