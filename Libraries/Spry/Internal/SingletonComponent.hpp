#ifndef SPRY_INTERNAL_SINGLETONCOMPONENT_HPP
#define SPRY_INTERNAL_SINGLETONCOMPONENT_HPP

#include "Runtime.hpp"
#include "RuntimeComponent.hpp"

namespace Spry
{
namespace Internal
{
	template<typename T, unsigned int id>
	class SingletonComponent :
		public RuntimeComponent
	{
	public:
		SingletonComponent() :
			RuntimeComponent(id)
		{
		}

		static const unsigned int ClassID = id;

		static T* GetInstance()
		{
			static T* instance = 0;

			if(instance == 0)
				instance = Runtime::GetComponent<T>();

			return instance;
		}

	private:
		SingletonComponent& operator=(const SingletonComponent&);
	};
}
}


#endif