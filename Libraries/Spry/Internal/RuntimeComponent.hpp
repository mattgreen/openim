#ifndef SPRY_INTERNAL_RUNTIMECOMPONENT_HPP
#define SPRY_INTERNAL_RUNTIMECOMPONENT_HPP

namespace Spry
{
namespace Internal
{
	// Abstract base of Spry runtime support components.
	//
	// Certain portions of Spry rely on resources such as invisible windows
	// or threads in order to operate properly. However, it is wasteful
	// to hold onto these resources if they aren't needed. Therefore, support
	// components are created as late as possible and stored in the single
	// Runtime structure instance.
	class RuntimeComponent
	{
	public:
		explicit RuntimeComponent(unsigned int id) :
			ID(id)
		{
		}

		virtual ~RuntimeComponent()
		{
		}

		const unsigned int ID;
	
	private:
		RuntimeComponent& operator=(const RuntimeComponent&);
	};

	// Factory function for RuntimeComponent-derived objects.
	template<typename T>
	inline RuntimeComponent* Create()
	{
		return new T();
	}
}
}


#endif