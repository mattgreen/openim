#ifndef SPRY_TYPETOTYPE_HPP
#define SPRY_TYPETOTYPE_HPP

namespace Spry
{
namespace Metaprogramming
{
	template<typename T>
	struct TypeToType
	{
		typedef T OriginalType;
	};
}
}

#endif