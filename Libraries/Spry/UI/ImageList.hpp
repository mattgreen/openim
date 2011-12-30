#ifndef SPRY_UI_IMAGELIST_HPP
#define SPRY_UI_IMAGELIST_HPP

#include "../Internal/ImageListHandle.hpp"
#include "Icon.hpp"
#include "Size.hpp"

namespace Spry
{
namespace UI
{
	class ImageList
	{
	public:
		explicit ImageList(Size dimensions = Size(16, 16), int depthBits = 8) :
			_handle(dimensions, depthBits)
		{
		}

		template<typename T>
		int Add(const T& item);

		void Clear()
		{
			ImageList_RemoveAll(_handle);
		}

		void Remove(int index)
		{
			ImageList_Remove(_handle, index);
		}

		operator HIMAGELIST()
		{
			return _handle;
		}

	private:
		Spry::Internal::ImageListHandle	_handle;
	};

	template<>
	inline int ImageList::Add(const Icon& icon)
	{
		return ImageList_AddIcon(_handle, icon);
	}
}
}

#endif