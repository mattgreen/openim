#ifndef SPRY_UI_ICON_HPP
#define SPRY_UI_ICON_HPP

#include "../Internal/IconHandle.hpp"
#include "../Internal/Shims.hpp"

namespace Spry
{
namespace UI
{
	class Icon
	{
	public:
		explicit Icon(HICON icon) :
			_icon(icon)
		{
		}

	public:
		operator HICON()
		{
			return _icon;
		}

	public:
		template<typename String>
		static Icon FromFile(const String& filename)
		{
			return Icon(LoadImage(0, Spry::Internal::GetStringBuffer(filename), IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_VGACOLOR));
		}

		template<typename String>
		static Icon FromResource(HMODULE module, const String& name)
		{
			return Icon(reinterpret_cast<HICON>(LoadImage(module, Spry::Internal::GetStringBuffer(name), IMAGE_ICON, 0, 0, LR_VGACOLOR)));
		}

	private:
		Spry::Internal::IconHandle _icon;
	};
}
}

#endif