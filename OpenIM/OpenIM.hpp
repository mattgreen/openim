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

#ifndef OPENIM_OPENIM_HPP
#define OPENIM_OPENIM_HPP

// Build environment checks.
//
#if ! defined(_MSC_VER) || _MSC_VER < 1300
#error "Unrecognized or old compiler. OpenIM requires Visual C++ 7.1 or higher in order to build."
#endif

#ifndef _MT
#error "C++ runtime library must be thread-safe; switch to a multithreaded library."
#endif

#ifndef _CPPRTTI
#error "C++ run time type information (RTTI) must be enabled."
#endif

// Linkage specifiers
//
#ifdef OPENIM_EXPORTS
#define OPENIM_API __declspec(dllexport)
#else
#define OPENIM_API __declspec(dllimport)
#endif

#define EXPORTED __declspec(dllexport)

// Version string
#define OPENIM_VERSION_STRING	"OpenIM 0.30"
#define OPENIM_BUILD_TIMESTAMP	__TIMESTAMP__

// API version number. Incremented if breaking changes are made to the API.
#define OPENIM_API_VERSION		1

#include <cwchar>

#include "Win32Headers.hpp"

#include "../Utilities/Assert.hpp"
#include "../Utilities/LeakChecker.hpp"

#ifdef _DEBUG
__declspec(selectany) LeakChecker GlobalLeakChecker;
#endif

#include "ComponentManager.hpp"

#endif