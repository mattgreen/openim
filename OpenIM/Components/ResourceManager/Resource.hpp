#ifndef OPENIM_COMPONENTS_RESOURCEMANAGER_RESOURCE_HPP
#define OPENIM_COMPONENTS_RESOURCEMANAGER_RESOURCE_HPP

template<typename T>
class Resource :
	public ResourceBase
{
	Resource(const wchar_t* name, const T& value) :
		ResourceBase(name),
		Value(value)
	{
	}
	
	

	T Value;
};

#endif