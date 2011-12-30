#ifndef OPENIM_COMPONENTS_RESOURCEMANAGER_RESOURCEBASE_HPP
#define OPENIM_COMPONENTS_RESOURCEMANAGER_RESOURCEBASE_HPP

#include "../../String.hpp"

class ResourceBase
{
public:
	ResourceBase(const wchar_t* name) :
		Name(name)
	{
	}

	virtual ~ResourceBase()
	{
	}

public:
	bool operator<(const ResourceBase* r) const
	{
		return Name < r.Name;
	}

public:
	virtual void*	GetValue() = 0;
	
public:
	String			Name;
};

#endif