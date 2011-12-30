#ifndef AIM_CONTACTLISTIMPL_HPP
#define AIM_CONTACTLISTIMPL_HPP

#include <vector>

#include "../../Utilities/Types.hpp"

class AIMAccount;
class AIMContact;
class AIMGroup;

typedef std::vector<AIMGroup*>	AIMGroups;

class ContactListImpl
{
public:
	~ContactListImpl();

	void		Clear(bool notify = true);
	AIMContact*	GetContact(uint16 iid, uint16 gid);
	AIMContact*	GetContactByID(const wchar_t* id);
	int			GetGroupCount() const;
	AIMGroup*	GetGroup(uint16 iid);
	AIMGroups&	GetList();
	uint16		GetUniqueBuddyID();
	uint16		GetUniqueGroupID();
	
	void		OnAccountDisconnect(AIMAccount* acct);

	void		OnContactAdd(AIMAccount* acct, const wchar_t* name, const wchar_t* alias, uint16 iid, uint16 gid, int position);
	void		OnContactMove(AIMAccount* acct, const wchar_t* name, uint16 iid, uint16 gid, int position);
	void		OnContactRemove(AIMAccount* acct, const wchar_t* name, uint16 iid, uint16 gid, int position);

	void		OnGroupAdd(AIMAccount* acct, const wchar_t* name, uint16 iid, uint16 gid, int position);
	void		OnGroupRemove(AIMAccount* acct, const wchar_t* name, uint16 iid, uint16 gid, int position);
	void		OnGroupUpdate(AIMAccount* acct, const wchar_t* name, uint16 iid, uint16 gid, int position);

private:
	AIMGroups	_groups;
};

#endif