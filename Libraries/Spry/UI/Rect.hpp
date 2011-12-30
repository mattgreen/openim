#ifndef SPRY_UI_RECT_HPP
#define SPRY_UI_RECT_HPP

namespace Spry
{
namespace UI
{
	struct Rect
	{
		Rect(long l = 0, long t = 0, long r = 0, long b = 0) :
			Left(l),
			Top(t),
			Right(r),
			Bottom(b)
		{
		}

		operator RECT() const
		{
			RECT r;
			r.left = Left;
			r.top = Top;
			r.right = Right;
			r.bottom = Bottom;
			return r;
		}

		operator RECT&()
		{
			return reinterpret_cast<RECT&>(*this);
		}

		long Left;
		long Top;
		long Right;
		long Bottom;
	};
}
}

#endif