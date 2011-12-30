#ifndef CONTACTLIST_CONTACTLISTCTRLBEHAVIOR_HPP
#define CONTACTLIST_CONTACTLISTCTRLBEHAVIOR_HPP

#include "../../OpenIM/WTL.hpp"

#include "Transaction.hpp"

class ContactListCtrlBehavior :
	public CMessageMap
{
public:
	explicit ContactListCtrlBehavior(CTreeViewCtrlEx& tree);

public:
	Transaction			Transaction;

public:
	BEGIN_MSG_MAP(ContactListCtrlBehavior)
		NOTIFY_CODE_HANDLER(NM_CUSTOMDRAW, OnCustomDraw)
		NOTIFY_CODE_HANDLER(TVN_ITEMEXPANDING, OnItemExpanding)
		NOTIFY_CODE_HANDLER(TVN_BEGINLABELEDIT, OnBeginLabelEdit)
		NOTIFY_CODE_HANDLER(TVN_ENDLABELEDIT, OnEndLabelEdit)
	END_MSG_MAP()

private:
	void		FormatIdleTime(int idleTime, wchar_t* buffer, int len);
	LRESULT		OnBeginLabelEdit(WPARAM, NMHDR* nmhdr, BOOL&);
	LRESULT		OnCustomDraw(int, NMHDR* hdr, BOOL& handled);
	LRESULT		OnEndLabelEdit(WPARAM, NMHDR* nmhdr, BOOL&);
	LRESULT		OnItemExpanding(int, NMHDR* item, BOOL& handled);

private:
	CTreeViewCtrlEx&	_tree;
};


#endif