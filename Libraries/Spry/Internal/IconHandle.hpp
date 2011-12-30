#ifndef SPRY_INTERNAL_ICONHANDLE_HPP
#define SPRY_INTERNAL_ICONHANDLE_HPP

namespace Spry
{
namespace Internal
{
	class IconHandle
	{
	public:
		IconHandle(HICON icon) :
			_handle(icon)
		{
		}

		~IconHandle()
		{
			if(_refs.IsLast() && _handle != 0)
			{
				DestroyIcon(_handle);
				_handle = 0;
			}
		}

		operator HICON()
		{
			return _handle;
		}

	private:
		HICON			_handle;
		ReferenceLink	_refs;
	};
}
}

#endif