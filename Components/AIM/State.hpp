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

#ifndef AIM_STATE_HPP
#define AIM_STATE_HPP

enum State
{
	NoChange,				// The previous operation did not change the connection state

	Offline,				// Completely disconnected
	AuthorizerConnect,		// Connecting to authorizer
	AuthorizerGreeting,		// Waiting for authorizer greeting
	MD5Response,			// Awaiting response for MD5 key
	AuthorizerResponse,		// Awaiting authentication response
	AuthorizationSuccess,	// Authorization succeeded - continue connection
	AuthorizationFailure,	// Authorization failed - stop connecting
	
	BOSConnect,				// Connecting to BOS
	SendAuthorizationCookie,// Sending cookie from authorizer
	SupportedServices,		
	ServiceVersionNumbers,
	RateLimit,
	ServiceSetup,
	ServiceSetupResponses,

	Online,
	Migrating
};

#endif