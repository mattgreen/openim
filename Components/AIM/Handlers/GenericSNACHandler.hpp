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

#ifndef AIM_GENERICSNACHANDLER_HPP
#define AIM_GENERICSNACHANDLER_HPP

#include <set>

#include "../Service.hpp"
#include "../SNACHandler.hpp"

class GenericSNACHandler :
	public SNACHandler
{
public:
	GenericSNACHandler(AIMContactList* list, Framer& framer, MemoryStream& input, MemoryStream& output);

	State		Process(const SNAC& snac);

	typedef std::set<Service> Services;
	Services	KnownServices;

	void		SetIdleTime(uint32 seconds);

private:
	void		DoReset();
	void		DoSetup();
	void 		ProcessError(const SNAC& snac);
	void		ProcessRateResponse(const SNAC&);
	void 		ProcessServiceVersionResponse(const SNAC& snac);
	void 		ProcessSupportedFamilies(const SNAC& snac);

private:
	uint32		_idleStart;
};

#endif