#ifndef OPENIM_COMPONENTS_MOCKOBJECTS_MOCKACCOUNT_HPP
#define OPENIM_COMPONENTS_MOCKOBJECTS_MOCKACCOUNT_HPP

#include "../../Account.hpp"
#include "../../String.hpp"

#include "../../Events/GroupEvents.hpp"

#include "MockGroup.hpp"

class MockAccount :
	public Account
{
public:
	/// Begin connecting to the account using the previously supplied credentials.
	virtual void Connect()
	{
	}

	/// Begin disconnecting from a connected account.
	virtual void Disconnect() {}

	virtual const wchar_t* GetProtocolName() { return L"MockAccount"; }
	virtual const wchar_t* GetUserID() { return _user.c_str(); }

	virtual bool IsConnected() { return false; }

	virtual void SetPassword(const wchar_t*) {}
	virtual bool SetUserID(const wchar_t* userid) { _user = userid; return true; }

private:
	String _user;
};


#endif