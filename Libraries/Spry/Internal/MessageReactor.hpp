#ifndef SPRY_INTERNAL_MESSAGEREACTOR_HPP
#define SPRY_INTERNAL_MESSAGEREACTOR_HPP

#include "../Delegate.hpp"
#include "../OSHeaders.hpp"

#include "SingletonComponent.hpp"

namespace Spry
{
namespace Internal
{
	typedef Spry::Delegate<void (WPARAM, LPARAM)>	MessageHandler;

	// Maps window messages to callbacks, enabling asynchronous notification
	// for constructs such as network sockets or system timers.
	class MessageReactor :
		public SingletonComponent<MessageReactor, 0xdf74016b>
	{
	public:	
		// Invokes the message handler for all future messages sent to the notification
		// window with the specified message ID.
		virtual void	AddMessageHandler(unsigned int message, MessageHandler handler) = 0;

		virtual void	AddMessageHandler(unsigned int message, WPARAM wParam, MessageHandler handler) = 0;

		// Retrieve the handle of the window that receives asynchronous notifications.
		virtual HWND	GetHWND() const = 0;

		// Removes the message handler from the message ID's handler list.
		virtual void	RemoveMessageHandler(unsigned int message, MessageHandler handler) = 0;

		virtual void	RemoveMessageHandler(unsigned int message, WPARAM wParam, MessageHandler handler) = 0;
	};
}
}


#endif