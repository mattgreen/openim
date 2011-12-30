//
// Copyright (C) OpenIM developers
//  
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software 
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

#ifndef AIM_ICBMSNACHANDLER_HPP
#define AIM_ICBMSNACHANDLER_HPP

#include "../SNACHandler.hpp"


typedef Spry::Delegate<void (const wchar_t*, const wchar_t*, bool)>		MessageHandler;
typedef Spry::Delegate<void (const wchar_t*, const wchar_t*)>			MessageErrorHandler;
typedef Spry::Delegate<void (const wchar_t*, int)>						TypingHandler;

class ICBMSNACHandler :
	public SNACHandler
{
public:
	ICBMSNACHandler(AIMContactList* list, Framer& framer, MemoryStream& input, MemoryStream& output);

	State				Process(const SNAC& snac);
	void				Send(const wchar_t* user, const wchar_t* msg, bool autoresponse);

public:
	int					MaxMessageLength;
	
public:
	MessageHandler		OnMessageReceive;
	MessageHandler		OnMessageSend;
	MessageErrorHandler	OnMessageSendError;
	TypingHandler		OnTypingUpdate;

private:
	void				DoReset();
	void				DoSetup();
	void				ProcessIncomingMessage(const SNAC& snac);
	void				ProcessIncomingMessageBasic(const SNAC& snac);
	void				ProcessMissedMessage(const SNAC& snac);
	void				ProcessSetupResponse(const SNAC& snac);
};

#endif