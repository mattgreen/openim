#ifndef CONTACTLIST_CONTACTLISTCTRLACTIONS_HPP
#define CONTACTLIST_CONTACTLISTCTRLACTIONS_HPP

#include "../../OpenIM/WTL.hpp"

#include "resource.h"

class ContactListCtrl;

class ContactListCtrlActions :
	public CMessageMap
{
public:
	ContactListCtrl* Control;

public:
	BEGIN_MSG_MAP(ContactListCtrlActions)
		
	END_MSG_MAP()

private:
	

};

#endif