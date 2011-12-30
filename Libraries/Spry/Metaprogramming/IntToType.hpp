#ifndef SPRY_INTTOTYPE_HPP
#define SPRY_INTTOTYPE_HPP

namespace Spry
{
namespace Metaprogramming
{
	template<int v>
	struct IntToType
	{
		enum { Value = v };
	};
}
}

#endif