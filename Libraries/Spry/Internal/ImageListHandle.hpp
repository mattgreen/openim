#ifndef SPRY_INTERNAL_IMAGELISTHANDLE_HPP
#define SPRY_INTERNAL_IMAGELISTHANDLE_HPP

#include "../OSHeaders.hpp"
#include "ReferenceLink.hpp"

#include "../UI/Size.hpp"

namespace Spry
{
namespace Internal
{
	class ImageListHandle
	{
	public:
		explicit ImageListHandle(Spry::UI::Size dimensions, int depthBits) :
			_handle(0),
			GrowSize(4),
			InitialSize(8),
			Dimensions(dimensions),
			Depth(depthBits)
		{
		}

		~ImageListHandle() throw()
		{
			if(_refs.IsLast())
				Release();
		}

		operator HIMAGELIST()
		{
			if(_handle == 0)
				Create();

			return _handle;
		}

		void Create()
		{
			Release();

			UINT flag = 0;
			switch(Depth)
			{
			case 4:
				flag = ILC_COLOR4;
				break;
			case 8:
				flag = ILC_COLOR8;
				break;
			case 16:
				flag = ILC_COLOR16;
				break;
			case 24:
				flag = ILC_COLOR24;
				break;
			case 32:
				flag = ILC_COLOR32;
				break;
			default:
				flag = ILC_COLOR8;
				break;
			}

			_handle = ImageList_Create(Dimensions.Width, Dimensions.Height, flag, InitialSize, GrowSize);
			if(_handle == 0)
				throw;
		}
	
	public:
		int				GrowSize;
		int				InitialSize;
		Spry::UI::Size	Dimensions;
		int				Depth;
	
	private:
		void Release() throw()
		{
			if(_handle == 0)
				return;

			ImageList_Destroy(_handle);
			_handle = 0;
		}

	private:
		HIMAGELIST		_handle;
		ReferenceLink	_refs;
	};
}
}

#endif