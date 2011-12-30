#ifndef OPENIM_COMPONENTS_MOCKOBJECTS_MOCKACCOUNTFACTORY_HPP
#define OPENIM_COMPONENTS_MOCKOBJECTS_MOCKACCOUNTFACTORY_HPP

#include "../../AccountFactoryImpl.hpp"

#include "MockAccount.hpp"

class MockAccountFactory :
	public AccountFactoryImpl<MockAccount>
{
public:
	MockAccountFactory() :
		AccountFactoryImpl<MockAccount>(L"MockAccount")
	{
	}
};

#endif