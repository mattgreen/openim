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

#include "Framer.hpp"

#include "FLAP.hpp"

Framer::Framer(AsyncStreamSocket& socket) :
	_socket(socket),
	_sequenceNumber(0)
{
}

void Framer::Send(Channel channel, MemoryStream& stream)
{
	if(TracingEnabled)
	{
		Trace("\n");
		Trace("SEND: %i bytes\n", stream.Length);
	}

	SendFLAP(channel, static_cast<unsigned short>(stream.Length));

	int sent = _socket.Send(stream.Buffer, static_cast<int>(stream.Length));
	Assert(sent == stream.Length);
	if(TracingEnabled)
	{
		Trace("DATA:\n");
		TraceHexDump(stream.Buffer, static_cast<int>(stream.Length));
	}

	stream.Clear();
}

void Framer::SendFLAP(Channel channel, unsigned short size)
{
	FLAP flapHeader;
	flapHeader.ID = 0x2a;
	flapHeader.Channel = static_cast<unsigned char>(channel);
	flapHeader.Sequence = HostToNetworkOrder(_sequenceNumber++);
	flapHeader.Size = HostToNetworkOrder(size);

	int sent = _socket.Send(reinterpret_cast<const char*>(&flapHeader), sizeof(flapHeader));
	
	Ignore(sent);
	Assert(sent == sizeof(flapHeader));
	
	if(TracingEnabled)
	{
		Trace("FLAP: (payload len = %u)\n", size);
		TraceHexDump(reinterpret_cast<const char*>(&flapHeader), sizeof(flapHeader));
	}
}

void Framer::SendSNAC(const SNAC& snac, MemoryStream& stream)
{
	if(TracingEnabled)
	{
		Trace("\n");
		Trace("SEND: %i bytes\n", stream.Length);
	}

	// HACK: partial sends are not handled!
	SendFLAP(DataChannel, static_cast<unsigned short>(stream.Length + sizeof(snac)));

	SNAC snacHeader;
	snacHeader.Service = HostToNetworkOrder(snac.Service);
	snacHeader.Subtype = HostToNetworkOrder(snac.Subtype);
	snacHeader.Flags = HostToNetworkOrder(snac.Flags);
	snacHeader.RequestID = HostToNetworkOrder(snac.RequestID);

	int sent = _socket.Send(reinterpret_cast<const char*>(&snacHeader), sizeof(snacHeader));
	Assert(sent == sizeof(snacHeader));
	
	if(TracingEnabled)
	{
		Trace("SNAC: (0x%02x:0x%02x, req id = %i)\n", snac.Service, snac.Subtype, snac.RequestID);
		TraceHexDump(reinterpret_cast<const char*>(&snacHeader), sizeof(snacHeader));
	}

	int bytesToSend = static_cast<int>(stream.Length);
	if(bytesToSend > 0)
	{
		sent = _socket.Send(stream.Buffer, bytesToSend);
		Assert(sent == static_cast<int>(stream.Length));
	}
	
	if(TracingEnabled)
	{
		Trace("DATA:\n");
		TraceHexDump(stream.Buffer, static_cast<int>(stream.Length));
	}

	stream.Clear();
}
