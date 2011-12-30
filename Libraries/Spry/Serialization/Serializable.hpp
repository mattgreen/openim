#ifndef SPRY_SERIALIZATION_SERIALIZABLE_HPP
#define SPRY_SERIALIZATION_SERIALIZABLE_HPP

#include "../NoVTable.hpp"

namespace Spry
{
namespace Serialization
{
	template<typename DerivedClass>
	class SPRY_NOVTABLE Serializable
	{
	public:
		template<typename Context>
		void Serialize(Context& context)
		{
			static_cast<DerivedClass*>(this)->Serialize(context);
		}
	};
}
}

#endif