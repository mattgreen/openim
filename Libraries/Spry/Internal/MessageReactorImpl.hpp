#ifndef SPRY_INTERNAL_MESSAGEREACTORIMPL_HPP
#define SPRY_INTERNAL_MESSAGEREACTORIMPL_HPP

#include <algorithm>

#pragma warning(push)
#pragma warning(disable:4702)
#include <map>
#include <vector>
#pragma warning(pop)

#include "../Assert.hpp"
#include "../Foreach.hpp"
#include "../OSException.hpp"
#include "MessageReactor.hpp"

namespace Spry
{
namespace Internal
{
	class MessageReactorImpl :
		public MessageReactor
	{
	public:
		MessageReactorImpl() :
			_wnd()
		{
			Runtime* runtime = Runtime::GetInstance();
			if(runtime == 0)
				throw Spry::OSException();

#define CLASSNAME "MessageReactor"
			static const char* WndTitle	= "";

			WNDCLASS wc;
			wc.cbClsExtra		= 0;
			wc.cbWndExtra		= 0;
			wc.hbrBackground	= static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
			wc.hCursor			= LoadCursor(NULL, IDC_ARROW);
			wc.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
			wc.hInstance		= runtime->Owner;
			wc.lpszClassName	= TEXT(CLASSNAME);
			wc.lpfnWndProc		= MessageReactorImpl::OnMessage;
			wc.lpszMenuName		= NULL;
			wc.style			= 0;

			if(! RegisterClass(&wc))
				throw Spry::OSException();

			_wnd = CreateWindowA(CLASSNAME,
				WndTitle,
				0,
				0,0,0,0,
				GetDesktopWindow(),
				0,
				runtime->Owner,
				0);
#undef CLASSNAME
		}

		~MessageReactorImpl()
		{
			for(Handlers::iterator i = _messageHandlers.begin(); i != _messageHandlers.end(); ++i)
				delete *i;
		}

		void AddMessageHandler(unsigned int message, MessageHandler msgHandler)
		{
			DoAddHandler(message, msgHandler, false);
		}

		void AddMessageHandler(unsigned int message, WPARAM wParam, MessageHandler msgHandler)
		{
			DoAddHandler(message, msgHandler, true, wParam);
		}

		HWND GetHWND() const
		{
			return _wnd;
		}

		static LRESULT CALLBACK OnMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
		{
			static MessageReactorImpl* impl = 0;
			if(impl == 0 && Runtime::HasComponent<MessageReactor>())
				impl = static_cast<MessageReactorImpl*>(MessageReactorImpl::GetInstance());

			if(impl != 0)
			{
				for(Handlers::iterator i = impl->_messageHandlers.begin(); i != impl->_messageHandlers.end(); i++)
				{
					impl->_currentMessages[msg] = true;
					
					if((*i)->Message == msg)
					{
						if((*i)->GeneralHandlers)
						{
							for(HandlerList::iterator j = (*i)->GeneralHandlers->begin(); j != (*i)->GeneralHandlers->end(); ++j)
							{
								(*j)(wParam, lParam);
							}
						}
						
						if((*i)->SpecificHandlers)
						{
							for(SpecificHandlerList::iterator j = (*i)->SpecificHandlers->begin(); j != (*i)->SpecificHandlers->end(); ++j)
							{
								if(j->WParam == wParam)
								{
									j->Handler(wParam, lParam);
								}
							}
						}
					}

					impl->_currentMessages[msg] = false;

					if(! impl->_addedSpecificHandlers.empty())
					{
						foreach(SpecificHandler h, impl->_addedSpecificHandlers)
							impl->DoAddHandler(msg, h.Handler, true, h.WParam);
						
						impl->_addedSpecificHandlers.clear();
					}

					if(! impl->_removedSpecificHandlers.empty())
					{
						foreach(SpecificHandler h, impl->_removedSpecificHandlers)
							impl->DoRemoveHandler(msg, h.Handler, true, h.WParam);
						
						impl->_removedSpecificHandlers.clear();
					}
				}

				if(! impl->_addedMessageHandlers.empty())
				{
					foreach(Handler* h, impl->_addedMessageHandlers)
						impl->_messageHandlers.push_back(h);
					impl->_addedMessageHandlers.clear();
				}

				if(! impl->_addedGeneralHandlers.empty())
				{
					foreach(MessageHandler h, impl->_addedGeneralHandlers)
						impl->DoAddHandler(msg, h, false);

					impl->_addedGeneralHandlers.clear();
				}

				if(! impl->_removedGeneralHandlers.empty())
				{
					foreach(MessageHandler h, impl->_removedGeneralHandlers)
						impl->DoRemoveHandler(msg, h, false);

					impl->_removedGeneralHandlers.clear();
				}
			}

			return DefWindowProc(hwnd, msg, wParam, lParam);
		}

		void RemoveMessageHandler(unsigned int message, MessageHandler msgHandler)
		{
			DoRemoveHandler(message, msgHandler, false);
		}

		void RemoveMessageHandler(unsigned int message, WPARAM wParam, MessageHandler msgHandler)
		{
			DoRemoveHandler(message, msgHandler, true, wParam);
		}

	private:
		void DoAddHandler(unsigned int message, MessageHandler msgHandler, bool specific, WPARAM wParam = 0)
		{
			// See if we have a handler entry already.
			Handler* handler = 0;
			for(Handlers::iterator i = _messageHandlers.begin(); i != _messageHandlers.end(); ++i)
			{
				if((*i)->Message == message)
				{
					handler = *i;
					break;
				}
			}

			// If not, make one and insert it right now.
			if(handler == 0)
			{
				handler = new Handler(message);
				if(_currentMessages.empty())
					_messageHandlers.push_back(handler);
				else
					_addedMessageHandlers.push_back(handler);
			}

			// Create a handler list if need be, and insert the message handler.
			if(specific)
			{
				if(handler->SpecificHandlers == 0)
					handler->SpecificHandlers = new SpecificHandlerList();

				SpecificHandler h;
				h.WParam = wParam;
				h.Handler = msgHandler;

				if(_currentMessages.find(message) == _currentMessages.end() || _currentMessages[message] == false)
					handler->SpecificHandlers->push_back(h);
				else
					_addedSpecificHandlers.push_back(h);
			}
			else
			{
				if(handler->GeneralHandlers == 0)
					handler->GeneralHandlers = new HandlerList();

				if(_currentMessages.find(message) == _currentMessages.end() || _currentMessages[message] == false)
					handler->GeneralHandlers->push_back(msgHandler);
				else
					_addedGeneralHandlers.push_back(msgHandler);
			}
		}

		void DoRemoveHandler(unsigned int message, MessageHandler msgHandler, bool specific, WPARAM wParam = 0)
		{
			// See if we have a handler entry already.
			for(Handlers::iterator i = _messageHandlers.begin(); i != _messageHandlers.end(); ++i)
			{
				if((*i)->Message == message)
				{
					if(! specific)
					{
						HandlerList*& genHandlers = (*i)->GeneralHandlers;
						if(genHandlers != 0)
						{
							//if(_currentMessages.find(message) == _currentMessages.end())
							//	genHandlers->erase(std::remove(genHandlers->begin(), genHandlers->end(), msgHandler));
							//else
								_removedGeneralHandlers.push_back(msgHandler);
						}
					
						break;
					}
					else
					{
						SpecificHandler h;
						h.Handler = msgHandler;
						h.WParam = wParam;
						
						SpecificHandlerList* specificHandlers = (*i)->SpecificHandlers;
						if(specificHandlers != 0)
						{
							//if(_currentMessages.find(message) == _currentMessages.end())
							//	specificHandlers->erase(std::remove(specificHandlers->begin(), specificHandlers->end(), h));
							//else
								_removedSpecificHandlers.push_back(h);
						}

						break;
					}
				}
			}
		}

	private:
		MessageReactorImpl& operator=(const MessageReactorImpl&);

	private:
		struct SpecificHandler
		{
			WPARAM			WParam;
			MessageHandler	Handler;

			bool operator==(const SpecificHandler& h)
			{
				return	WParam == h.WParam &&
						Handler == h.Handler;
			}
		};

		typedef std::vector<MessageHandler>		HandlerList;
		typedef std::vector<SpecificHandler>	SpecificHandlerList;

		struct Handler
		{
			Handler(unsigned int msg) :
				Message(msg),
				GeneralHandlers(),
				SpecificHandlers()
			{
			}

			~Handler()
			{
				delete GeneralHandlers;
				delete SpecificHandlers;
			}

			unsigned int			Message;
			HandlerList*			GeneralHandlers;
			SpecificHandlerList*	SpecificHandlers;
		};

		typedef std::vector<Handler*> Handlers;
		
		typedef std::map<unsigned int, bool> RaisedMessages;
		
		RaisedMessages		_currentMessages;
		Handlers			_messageHandlers;
		Handlers			_addedMessageHandlers;
		Handlers			_removedMessageHandlers;
		HWND				_wnd;

		HandlerList			_addedGeneralHandlers;
		HandlerList			_removedGeneralHandlers;
		SpecificHandlerList	_addedSpecificHandlers;
		SpecificHandlerList	_removedSpecificHandlers;
	};
}
}



#endif