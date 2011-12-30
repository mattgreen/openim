#ifndef SPRY_SELECT_HPP
#define SPRY_SELECT_HPP

namespace Spry
{
namespace Metaprogramming
{
	template<bool flag, typename T, typename U>
	struct Select
	{
		typedef T Result;
	};

	template<typename T, typename U>
	struct Select<false, T, U>
	{
		typedef U Result;
	};
}
}

#endif