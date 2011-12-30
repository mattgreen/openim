#ifndef SPRY_ASSERT_HPP
#define SPRY_ASSERT_HPP

#include <cassert>

// Debug build error checking, no-op in release
#define Assert(x) assert(x)
#define Ignore(x) (x)
#define Fail(x) Assert((x) && 0)

template<typename Exception>
void Enforce(bool condition)
{
	if(! condition)
		throw Exception();
}

#endif