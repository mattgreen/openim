#include "ContactListCtrlBehavior.hpp"

#include <cmath>

#include "Node.hpp"
#include "Spry.hpp"

using namespace Spry;

ContactListCtrlBehavior::ContactListCtrlBehavior(CTreeViewCtrlEx& tree) :
	Transaction(NoTransaction),
	_tree(tree)
{
}

void ContactListCtrlBehavior::FormatIdleTime(int idleTime, wchar_t* buffer, int len)
{
	Assert(len >= 32);
	Ignore(len);

	wchar_t* pos = buffer;
	if(idleTime >= 1440)
	{
		int days = static_cast<int>(floor((double)idleTime / (double)1440));
		idleTime -= days * 1440;

		pos += wsprintf(pos, L"%dd ", days);
	}

	if(idleTime >= 60)
	{
		int hours = static_cast<int>(floor((double)idleTime / (double)60));
		idleTime -= hours * 60;

		pos += wsprintf(pos, L"%dh ", hours);
	}

	if(idleTime > 0)
		wsprintf(pos, L"%dm ", idleTime);
}

LRESULT ContactListCtrlBehavior::OnBeginLabelEdit(WPARAM, NMHDR* nmhdr, BOOL& handled)
{
	handled = TRUE;

	NMTVDISPINFO* dispInfo = reinterpret_cast<NMTVDISPINFO*>(nmhdr);
	switch(Transaction)
	{
	//case Transaction::AddContact:
	case AddGroupTransaction:
		return FALSE;
		break;

	/*case Transaction::RenameContact:
	case Transaction::RenameGroup:
		{
			if(dispInfo->item.pszText)
			{
				CTreeItem item(dispInfo->item.hItem, &m_tree);
				Node<void>* node = reinterpret_cast<Node<void>*>(item.GetData());

				if(node)
				{
					//TODO: polymorphic behavior
					String label;
					if(m_transaction == Transaction::RenameContact)
						label = GetLabel(static_cast<Contact*>(node->Data));
					else
						label = GetLabel(static_cast<Group*>(node->Data));

					CEdit editControl = m_tree.GetEditControl();
					editControl.SetWindowText(label.c_str());

					return FALSE;
				}
			}
		}*/
	}

	return TRUE;
}

LRESULT ContactListCtrlBehavior::OnCustomDraw(int, NMHDR* hdr, BOOL& handled)
{
	handled = TRUE;

	NMTVCUSTOMDRAW* drawInfo = reinterpret_cast<NMTVCUSTOMDRAW*>(hdr);
	Node* node = 0;

	switch(drawInfo->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT:
		return CDRF_NOTIFYITEMDRAW;
		break;

	case CDDS_ITEMPREPAINT:
		node = reinterpret_cast<Node*>(drawInfo->nmcd.lItemlParam);
		
		// If no item data is present, then don't bother with future notifications.
		if(node == 0)
			return CDRF_DODEFAULT;

		// If this is a contact node, and it is not selected...
		if(node->Type == ContactNodeType)
		{
			int idleTime = node->Contact->GetIdleTime();

			// If the contact is offline, or is idle longer than a certain threshold, color the item gray.
			if(! node->Contact->IsConnected() || idleTime > 15)
			{
				// ...except if it is currently selected. The text can be hard to see in these cases.
				if(_tree.GetSelectedItem() != node->Item)
					drawInfo->clrText = GetSysColor(COLOR_GRAYTEXT);
			}

			// Only need further paint notification if the user is idle.
			if(idleTime > 0)
				return CDRF_NOTIFYPOSTPAINT;
		}
		break;

	case CDDS_ITEMPOSTPAINT:
		node = reinterpret_cast<Node*>(drawInfo->nmcd.lItemlParam);
		Assert(node != 0);

		if(node == 0)
			return CDRF_DODEFAULT;

		if(node->Type == ContactNodeType)
		{
			int idleTime = node->Contact->GetIdleTime();

			// Color the idle time gray if it is past the user's threshold.
			COLORREF color = GetSysColor(COLOR_WINDOWTEXT);
			if(idleTime > 15)
				color = GetSysColor(COLOR_GRAYTEXT);

			// Convert the idle time into a string.
			WCharBuffer<32> text;
			FormatIdleTime(idleTime, text, text.Capacity);

			const unsigned int DrawTextFlags = DT_SINGLELINE | DT_VCENTER | DT_RIGHT;

			// Calculate where the left edge of the text will end up. We need to figure out
			// if it ends up clipping the tree item text.
			RECT textBounds = drawInfo->nmcd.rc;
			DrawText(drawInfo->nmcd.hdc, text, -1, &textBounds, DrawTextFlags | DT_CALCRECT);
			int leftEdge = drawInfo->nmcd.rc.right - textBounds.right;

			RECT treeItemBounds;
			_tree.GetItemRect(reinterpret_cast<HTREEITEM>(drawInfo->nmcd.dwItemSpec), &treeItemBounds, TRUE);
			if(treeItemBounds.right >= leftEdge)
				return CDRF_DODEFAULT;

			// Clear the area where the text is to be drawn.
			int width = textBounds.right - textBounds.left;
			textBounds.right = drawInfo->nmcd.rc.right;
			textBounds.left = textBounds.right - width;
			FillRect(drawInfo->nmcd.hdc, &textBounds, reinterpret_cast<HBRUSH>(COLOR_WINDOW+1));

			// Draw the text.
			COLORREF fore = SetTextColor(drawInfo->nmcd.hdc, color);
			COLORREF back = SetBkColor(drawInfo->nmcd.hdc, GetSysColor(COLOR_WINDOW));

			DrawText(drawInfo->nmcd.hdc, text, -1, &drawInfo->nmcd.rc, DrawTextFlags);

			SetBkColor(drawInfo->nmcd.hdc, back);
			SetTextColor(drawInfo->nmcd.hdc, fore);

			return CDRF_SKIPDEFAULT;
		}

		break;
	}

	return CDRF_DODEFAULT;
}

LRESULT ContactListCtrlBehavior::OnEndLabelEdit(WPARAM, NMHDR* nmhdr, BOOL& handled)
{
	handled = TRUE;

	NMTVDISPINFO* dispInfo = reinterpret_cast<NMTVDISPINFO*>(nmhdr);
	BOOL result = FALSE;

	CTreeItem item(dispInfo->item.hItem, &_tree);
	
	AutoPtr<Node> node(reinterpret_cast<Node*>(item.GetData()));
	Assert(node != 0);

	switch(Transaction)
	{
	case AddGroupTransaction:
		if(dispInfo->item.pszText != 0)
		{
			// Get account this group is being created on.
			Account* account = 0;
			for(CTreeItem parent = item.GetParent(); item.IsNull() == FALSE; parent = parent.GetParent())
			{
				Node* node = reinterpret_cast<Node*>(parent.GetData());
				if(node->Type == AccountNodeType)
				{
					account = node->Account;
					break;
				}
			}
			
			Assert(account != 0);

			account->GetContactList()->AddGroup(dispInfo->item.pszText, node->Parent);
			result = TRUE;
		}

		item.Delete();

		break;
	
	/*case Transaction::RenameContact:
		{
			Node<Contact>* node = reinterpret_cast<Node<Contact>*>(item.GetData());
			if(dispInfo->item.pszText)
				node->Data->SetAlias(dispInfo->item.pszText);

			break;
		}
	case Transaction::RenameGroup:
		{
			Node<Group>* node = reinterpret_cast<Node<Group>*>(item.GetData());
			if(dispInfo->item.pszText)
				node->Data->SetName(dispInfo->item.pszText);

			break;
		}*/
	}

	Transaction = NoTransaction;

	return result;
}

LRESULT ContactListCtrlBehavior::OnItemExpanding(int, NMHDR* notifyMsg, BOOL& handled)
{
	NMTREEVIEW* msg = reinterpret_cast<NMTREEVIEW*>(notifyMsg);
	handled = TRUE;

	if(msg->action == TVE_COLLAPSE)
	{
		CTreeItem item(msg->itemNew.hItem, &_tree);
		Assert(! item.IsNull());

		Node* node = reinterpret_cast<Node*>(item.GetData());
		Assert(node != 0);

		if(node->Type == AccountNodeType)
			return TRUE;
	}

	return 0;
}