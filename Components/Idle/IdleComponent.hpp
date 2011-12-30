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

#ifndef IDLE_IDLECOMPONENT_HPP
#define IDLE_IDLECOMPONENT_HPP

#include "../../OpenIM/OpenIM.hpp"

#include "../../OpenIM/ComponentImpl.hpp"

#include "../../OpenIM/Events/AccountEvents.hpp"
#include "../../OpenIM/Events/EventBindings.hpp"
#include "../../OpenIM/Events/TimerEvents.hpp"

class IdleComponent :
	public ComponentImpl<0x49d1c5d7>
{
public:
	IdleComponent();
	~IdleComponent();

	void 			OnAccountConnect(AccountConnectEvent&);
	void 			OnAccountDisconnect(AccountDisconnectEvent&);
	void 			OnTimerEvent(TimerEvent&);

private:
	EventBindings	_bindings;
	HWND			_wnd;
};

#endif