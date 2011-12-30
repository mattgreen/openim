#ifndef OPENIM_COMPONENTS_MOCKOBJECTS_MOCKGROUP_HPP
#define OPENIM_COMPONENTS_MOCKOBJECTS_MOCKGROUP_HPP

#include "../../Group.hpp"

class MockGroup :
	public Group
{
public:
	MockGroup(Account* account) :
		_owner(account)
	{
	}

	virtual Account* GetAccount()
	{
		return _owner;
	}

	virtual const wchar_t* GetName()
	{
		return L"MockGroup";
	}

private:
	Account* _owner;
};

#endif