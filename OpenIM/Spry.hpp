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

#ifndef OPENIM_SPRY_HPP
#define OPENIM_SPRY_HPP

#define SPRY_AUTO_USING_DECLARATIONS

#ifdef OPENIM_EXPORTS
#ifndef SPRY_API
	#define SPRY_API __declspec(dllexport)
#endif
#else
#ifndef SPRY_API
	#define SPRY_API __declspec(dllimport)
#endif
#endif

#include "../Libraries/Spry/Assert.hpp"
#include "../Libraries/Spry/AutoPtr.hpp"
#include "../Libraries/Spry/Collections/ArrayList.hpp"
#include "../Libraries/Spry/DynamicLibrary.hpp"
#include "../Libraries/Spry/Environment.hpp"
#include "../Libraries/Spry/Foreach.hpp"
#include "../Libraries/Spry/Integer.hpp"
#include "../Libraries/Spry/Drawing/Color.hpp"
#include "../Libraries/Spry/IO/BinaryReader.hpp"
#include "../Libraries/Spry/IO/BinaryWriter.hpp"
#include "../Libraries/Spry/IO/Directory.hpp"
#include "../Libraries/Spry/IO/File.hpp"
#include "../Libraries/Spry/IO/FileStream.hpp"
#include "../Libraries/Spry/IO/Path.hpp"
#include "../Libraries/Spry/Runtime.hpp"
#include "../Libraries/Spry/UI/Icon.hpp"
#include "../Libraries/Spry/UI/Timer.hpp"
#include "../Libraries/Spry/WCharBuffer.hpp"

#endif