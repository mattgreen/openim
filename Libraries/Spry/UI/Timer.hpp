#ifndef SPRY_UI_TIMER_HPP
#define SPRY_UI_TIMER_HPP

#include "../Delegate.hpp"
#include "../Internal/MessageReactor.hpp"

namespace Spry
{
namespace UI
{
	typedef Spry::Delegate<void ()>	TimerTickHandler;

	class Timer
	{
	public:
		explicit Timer(int interval) :
			_interval(interval),
			_active(false),
			_wnd()
		{
		}

		~Timer() throw()
		{
			Stop();
		}

		void Start()
		{
			using Spry::Internal::MessageHandler;
			using Spry::Internal::MessageReactor;

			if(_active)
				return;

			_wnd = MessageReactor::GetInstance()->GetHWND();
			UINT_PTR result = 
				SetTimer(_wnd,
					reinterpret_cast<UINT_PTR>(this),
					static_cast<UINT>(_interval),
					0);
			
			if(result == 0)
				throw;

			_active = true;
			MessageReactor::GetInstance()->AddMessageHandler(WM_TIMER, MessageHandler(this, &Timer::OnMessage));
		}

		void Stop()
		{
			using Spry::Internal::MessageHandler;
			using Spry::Internal::MessageReactor;

			if(! _active)
				return;
			
			int result = KillTimer(_wnd, reinterpret_cast<UINT_PTR>(this));
			MessageReactor::GetInstance()->RemoveMessageHandler(WM_TIMER, MessageHandler(this, &Timer::OnMessage));

			if(result != 0)
				_active = false;
		}

		TimerTickHandler	OnTick;

	private:
		void OnMessage(WPARAM, LPARAM)
		{
			if(_active)
			{
				if(OnTick != 0)
				{
					OnTick();
				}
			}
		}

	private:
		int					_interval;
		bool				_active;
		HWND				_wnd;
	};
}
}

#endif