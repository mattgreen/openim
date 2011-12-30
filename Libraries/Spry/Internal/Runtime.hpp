#ifndef SPRY_INTERNAL_RUNTIME_HPP
#define SPRY_INTERNAL_RUNTIME_HPP

#include <cstdlib>

#include "../Assert.hpp"
#include "../OSHeaders.hpp"

#include "RuntimeComponent.hpp"

#pragma warning(push)
#pragma warning(disable:4244)
#pragma warning(disable:4312)

// Environment variable where the TLS slot will be stored.
#define SPRY_RUNTIME_ID "Spry::Runtime 1.0"

// Maximum number of loaded runtime components.
#define SPRY_RUNTIME_MAX_COMPONENTS 32

namespace Spry
{
namespace Internal
{
	// Allows creation of components via ID.
	typedef RuntimeComponent* (* ComponentFactory)();

	struct ObjectFactory
	{
		int					ID;
		ComponentFactory	Creator;
	};

	// Spry runtime support structure.
	// 
	struct Runtime
	{
	public:
		Runtime(HMODULE owner, int slot) :
			Owner(owner),
			NumComponents(),
			NumFactories(),
			Slot(slot)
		{
		}

		~Runtime()
		{
			for(int i = NumComponents - 1; i >= 0; i--)
				delete Components[i];
			NumComponents = 0;
		}

		template<typename T>
		static void AddFactory()
		{
			ObjectFactory factory;
			factory.Creator = Create<T>;
			factory.ID = T::ClassID;
			
			if(Runtime* runtime = GetInstance())
			{
				for(int i = 0; i < runtime->NumFactories; i++)
				{
					if(runtime->Factories[i].ID == T::ClassID)
						return;
				}

				if(runtime->NumFactories < SPRY_RUNTIME_MAX_COMPONENTS)
					runtime->Factories[runtime->NumFactories++] = factory;
			}
		}

		// Retrieve the single, process-wide Runtime structure.
		static Runtime* GetInstance()
		{
			static Runtime* instance = 0;

			if(instance == 0)
			{
				char str[2] = { 0, };

				GetEnvironmentVariableA(SPRY_RUNTIME_ID, str, sizeof str);
				int slot = static_cast<int>(str[0] - 60);

				instance = static_cast<Runtime*>(TlsGetValue(slot));
			}

			return instance;
		}

		template<typename T>
		static T* GetComponent()
		{
			Runtime* runtime = GetInstance();
			if(runtime == 0)
				return 0;

			// See if the component has already been instantiated first.
			for(int i = 0; i < runtime->NumComponents; i++)
			{
				// It has. Return the existing component.
				if(runtime->Components[i]->ID == T::ClassID)
					return static_cast<T*>(runtime->Components[i]);
			}

			// If it hasn't, see if any factories can make it for us.
			for(int i = 0; i < runtime->NumFactories; i++)
			{
				// Found one. Construct the component, add it to the loaded component
				// list, and return the new component.
				if(runtime->Factories[i].ID == T::ClassID)
				{
					Assert(runtime->Factories[i].Creator != 0);
					if(runtime->NumComponents < SPRY_RUNTIME_MAX_COMPONENTS)
					{
						RuntimeComponent* component = runtime->Factories[i].Creator();
						Assert(component != 0);
						
						runtime->Components[runtime->NumComponents++] = component;
						
						return static_cast<T*>(component);
					}
				}
			}
		

			return 0;
		}

		

		template<typename T>
		static bool HasComponent()
		{
			Runtime* runtime = GetInstance();
			if(runtime == 0)
				return false;

			for(int i = 0; i < runtime->NumComponents; i++)
			{
				if(runtime->Components[i]->ID == T::ClassID)
					return true;
			}

			return false;
		}

		static void Host(HMODULE module)
		{
			// Ensure some other module hasn't beaten us to the punch.
			if(GetInstance() != 0)	
				return;

			// Install the struct into a TLS slot. 
			int tlsSlot = TlsAlloc();
			TlsSetValue(tlsSlot, new Runtime(module, tlsSlot));

			// Store the TLS slot index in an environment variable, formatted
			// as a string.
			char str[2] = { 0, };
			str[0] = static_cast<char>(tlsSlot + 60);
			SetEnvironmentVariableA(SPRY_RUNTIME_ID, str);

			OutputDebugString(TEXT(SPRY_RUNTIME_ID) TEXT(" loaded.\n"));
		}

		static void Shutdown()
		{
			if(Runtime* runtime = GetInstance())
			{
				TlsFree(runtime->Slot);

				delete runtime;
				SetEnvironmentVariableA(SPRY_RUNTIME_ID, 0);

				OutputDebugString(TEXT(SPRY_RUNTIME_ID) TEXT(" unloaded.\n"));
			}
		}	

		RuntimeComponent*	Components[SPRY_RUNTIME_MAX_COMPONENTS];
		int					NumComponents;
		
		ObjectFactory		Factories[SPRY_RUNTIME_MAX_COMPONENTS];
		int					NumFactories;

		HMODULE				Owner;
		int					Slot;
		char				ScratchBuffer[32768];
	};
}
}

#undef SPRY_RUNTIME_ID
#undef SPRY_RUNTIME_MAX_COMPONENTS

#pragma warning(pop)

#endif