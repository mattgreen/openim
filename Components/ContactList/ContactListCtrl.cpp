//
// Copyright (C) OpenIM developers
//  
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software 
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

#include "ContactListCtrl.hpp"

#include <algorithm>
#include <cmath>

#include "Main.hpp"
#include "Spry.hpp"

const UINT ContextMenuFlags = TPM_RIGHTBUTTON | TPM_LEFTALIGN;

ContactListCtrl::ContactListCtrl() :
	_dragImageList(),
	_resourceManager(ComponentManager::Get<IResourceManager>()),
	_accountContext(LoadMenu(Module, MAKEINTRESOURCE(IDR_ACCOUNTCONTEXT))),
	_groupContext(LoadMenu(Module, MAKEINTRESOURCE(IDR_GROUPCONTEXT))),
	_contactContext(LoadMenu(Module, MAKEINTRESOURCE(IDR_CONTACTCONTEXT))),
	_dragging(),
	_showOfflineContacts()
{
	Assert(_resourceManager != 0);

	LOGFONT iconFont;
	SystemParametersInfo(SPI_GETICONTITLELOGFONT, sizeof(iconFont), &iconFont, 0);
	_treeFont.CreateFontIndirect(&iconFont);
}

ContactListCtrl::~ContactListCtrl()
{
	for(AccountTable::iterator i = _accounts.begin(); i != _accounts.end(); ++i)
		delete i->second;
	_accounts.clear();

	for(GroupTable::iterator i = _groups.begin(); i != _groups.end(); ++i)
		delete i->second;
	_groups.clear();

	for(ContactTable::iterator i = _contacts.begin(); i != _contacts.end(); ++i)
		delete i->second;
	_contacts.clear();
}

// Given only a Node, insert an item into the tree.
void ContactListCtrl::CreateItem(Node* node, void* parent, bool expanded)
{
	Assert(node != 0);

	if(! node->Item.IsNull())
		return;

	CTreeItem parentItem;
	CTreeItem firstSibling = _tree.GetRootItem();

	// Retrieve the parent node, if specified. If someone is silly enough
	// to add a child to a node associated with multiple tree items, then
	// just take the first one we find. 
	if(parent != 0)
	{
		GroupTable::iterator i = _groups.find(reinterpret_cast<Group*>(parent));
		if(i != _groups.end())
		{
			Assert(! i->second->Item.IsNull());
			
			parentItem = i->second->Item;
			firstSibling = parentItem.GetChild();
		}
		else
		{
			AccountTable::iterator j = _accounts.find(reinterpret_cast<Account*>(parent));
			if(j != _accounts.end())
			{
				Assert(! j->second->Item.IsNull());

				parentItem = j->second->Item;
				firstSibling = parentItem.GetChild();
			}
			else
			{
				Fail("Insert subitem: parent node not found!");
				return;
			}
		}
	}

	// Determine the item's position relative to its new siblings.
	HTREEITEM previousItem = TVI_FIRST;
	for(CTreeItem i = firstSibling; ! i.IsNull(); i = i.GetNextSibling())
	{
		Node* siblingNode = reinterpret_cast<Node*>(i.GetData());
		Assert(siblingNode != 0);

		if(siblingNode->Position < node->Position)
			previousItem = siblingNode->Item;
		else
			break;
	}

	// Insert the item with as little information as possible - it will be updated later shortly.
	node->Item = _tree.InsertItem(TVIF_PARAM | TVIF_STATE, 
			L"", 
			0, 0, 
			expanded ? TVIS_EXPANDED : 0, TVIS_EXPANDED, 
			reinterpret_cast<LONG_PTR>(node), 
			parentItem.IsNull() ? TVI_ROOT : parentItem, 
			previousItem);
	Assert(! node->Item.IsNull());

	// Update the item's info.
	switch(node->Type)
	{
	case ContactNodeType:
		OnContactUpdate(node->Contact);
		break;

	case GroupNodeType:
		OnGroupUpdate(node->Group);
		break;

	case AccountNodeType:
		OnAccountUpdate(node->Account);
		break;
	}
}

void ContactListCtrl::FormatIdleTime(int idleTime, wchar_t* buffer, int len)
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

Account* ContactListCtrl::GetAccountFor(const CTreeItem& item)
{
	for(CTreeItem i = item; i.IsNull() == FALSE; i = i.GetParent())
	{
		Node* node = reinterpret_cast<Node*>(i.GetData());
		if(node->Type == AccountNodeType)
			return node->Account;
	}

	return 0;
}

// Performs hit testing on the tree control in response to a click, returning the item at the specified
// coordinates. Coordinates are relative to the screen, not the tree client area.
CTreeItem ContactListCtrl::GetItemAt(const CPoint& pt)
{
	Assert(pt.x != -1 && pt.y != -1);

	CPoint treePt = pt;
	_tree.ScreenToClient(&treePt);

	// Do we want to allow users to click on the space directly to the right of a label?
	// Windows Explorer thinks so.
	UINT flags = 0;
	CTreeItem selectedItem = _tree.HitTest(treePt, &flags);
	if(selectedItem.IsNull() || (((flags & TVHT_ONITEMLABEL) == 0) && (flags & TVHT_ONITEMICON) == 0))
		return CTreeItem();		

	return selectedItem;
}

Node* ContactListCtrl::GetSelectedNode()
{
	CTreeItem item = _tree.GetSelectedItem();
	if(item.IsNull())
		return 0;

	return reinterpret_cast<Node*>(item.GetData());
}

bool ContactListCtrl::GetShowOfflineContacts()
{
	return _showOfflineContacts;
}

void ContactListCtrl::OnAccountConnect(Account* account, int)
{
	OnAccountUpdate(account);
}

void ContactListCtrl::OnAccountCreate(Account* account, int position)
{
	Assert(account != 0);
	Assert(position >= 0);
	Assert(_accounts.find(account) == _accounts.end());

	Node* node = new Node(account, position);
	_accounts.insert(std::make_pair(account, node));	

	CreateItem(node, 0, true);
}

void ContactListCtrl::OnAccountDestroy(Account* account)
{
	Assert(account != 0);

	AccountTable::iterator i = _accounts.find(account);
	if(i == _accounts.end())
		return;

	if(! i->second->Item.IsNull())
		i->second->Item.Delete();

	delete i->second;
	_accounts.erase(account);
}

void ContactListCtrl::OnAccountDisconnect(Account* account)
{
	OnAccountUpdate(account);
}

void ContactListCtrl::OnAccountUpdate(Account* account)
{
	Assert(account != 0);

	AccountTable::iterator i = _accounts.find(account);
	if(i == _accounts.end())
		return;
	
	int newImage = _resourceManager->GetImageIndex(account);
	
	i->second->Item.SetImage(newImage, newImage);

	if(account->GetLabel() != 0)
		i->second->Item.SetText(account->GetLabel());
	else
		i->second->Item.SetText(account->GetUserID());
}

LRESULT ContactListCtrl::OnBeginDrag(NMHDR* hdr)
{
	NMTREEVIEW* msg = reinterpret_cast<NMTREEVIEW*>(hdr);
	_dragging = true;
	
	_dragImageList = _tree.CreateDragImage(msg->itemNew.hItem);
	_dragImageList.BeginDrag(0, msg->ptDrag.x, msg->ptDrag.y);
	_dragImageList.DragEnter(_tree, );
	_tree.SetCapture();

	return 0;
}

LRESULT ContactListCtrl::OnBeginLabelEdit(NMHDR* nmhdr)
{
	NMTVDISPINFO* dispInfo = reinterpret_cast<NMTVDISPINFO*>(nmhdr);
	switch(_transaction)
	{
	case AddContactTransaction:
	case AddGroupTransaction:
		return FALSE;
		break;

	case RenameContactTransaction:
	case RenameGroupTransaction:
		{
			CTreeItem item(dispInfo->item.hItem, &_tree);
			Assert(! item.IsNull());
			if(item.IsNull())
				return TRUE;

			Node* node = reinterpret_cast<Node*>(item.GetData());
			Assert(node != 0);
			if(node == 0)
				return TRUE;
			
			if(dispInfo->item.pszText == 0)
				return TRUE;

			_tree.GetEditControl().SetWindowText(dispInfo->item.pszText);

			return FALSE;
		}
	}

	return TRUE;
}



void ContactListCtrl::OnContactCreate(Contact* contact, Group* group, int position)
{
	Assert(contact != 0);
	Assert(position >= 0);

	Node* node = new Node(contact, group, position);
	_contacts.insert(std::make_pair(contact, node));
	
	if(_showOfflineContacts || contact->IsConnected())
		CreateItem(node, node->Parent, false);	
}

void ContactListCtrl::OnContactDestroy(Contact* contact)
{
	RemoveItems(contact);

	for(ContactTable::iterator i = _contacts.lower_bound(contact); i != _contacts.upper_bound(contact); ++i)
		delete i->second;
	_contacts.erase(contact);
}

void ContactListCtrl::OnContactSignOff(Contact* contact)
{
	if(! _showOfflineContacts)
		RemoveItems(contact);
	else
		OnContactUpdate(contact);
}

void ContactListCtrl::OnContactSignOn(Contact* contact)
{
	Assert(contact != 0);

	if(! _showOfflineContacts)
	{
		for(ContactTable::iterator i = _contacts.lower_bound(contact); i != _contacts.upper_bound(contact); ++i)
			CreateItem(i->second, i->second->Parent, false);
	}
	else
		OnContactUpdate(contact);
}

void ContactListCtrl::OnContactUpdate(Contact* contact)
{
	Assert(contact != 0);

	int image = _resourceManager->GetImageIndex(contact);
	for(ContactTable::iterator i = _contacts.lower_bound(contact); i != _contacts.upper_bound(contact); ++i)
	{
		if(! i->second->Item.IsNull())
		{
			i->second->Item.SetImage(image, image);
			i->second->Item.SetText(contact->GetDisplayName());
		}
	}
}

void ContactListCtrl::OnContextMenu(HWND, CPoint pt)
{
	// Figure out which item was selected.
	CTreeItem item;
	if(pt.x != -1 && pt.y != -1)
	{
		item = GetItemAt(pt);
		if(item.IsNull())
			return;
	}
	else
	{
		// Shift-F10/context menu key pressed.
		CRect rect;
		item = _tree.GetSelectedItem();
		if(item.IsNull())
			return;

		item.GetRect(&rect, TRUE);

		// Offsets similar to those used by Windows Explorer.
		pt.x = rect.left + 1;
		pt.y = rect.bottom - ((rect.bottom - rect.top) / 2);

		_tree.ClientToScreen(&pt);
	}

	Assert(! item.IsNull());

	// Ensure the target is selected, or else the context menu
	// doesn't feel right.
	if(_tree.GetSelectedItem().m_hTreeItem != item.m_hTreeItem)
		item.Select();

	Node* node = reinterpret_cast<Node*>(item.GetData());
	if(node == 0)
	{
		Fail("No data found for selected item!");
		return;
	}

	switch(node->Type)
	{
	case AccountNodeType:
		ShowAccountContextMenu(node, pt);
		break;

	case ContactNodeType:
		ShowContactContextMenu(node, pt);
		break;

	case GroupNodeType:
		ShowGroupContextMenu(node, pt);
		break;
	}

}

LRESULT ContactListCtrl::OnCreate(CREATESTRUCT*)
{
	_tree.Create(*this, 
		0, 
		0, 
		WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | TVS_HASBUTTONS | TVS_NOTOOLTIPS | TVS_EDITLABELS);
	Assert(_tree != 0);
	_tree.SetFont(_treeFont);
	
	// Attach the system image list to the treeview, if present.
	if(_resourceManager != 0)
	{
		HIMAGELIST imageList = _resourceManager->GetImageList();
		if(imageList != 0)
		{
			_tree.SetImageList(imageList, TVSIL_NORMAL);
		}
	}

	return 0;
}

LRESULT ContactListCtrl::OnCustomDraw(NMHDR* hdr)
{
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
			// If no data is present, then the node is temporary and thus has no associated data.
			if(node->Contact == 0)
				return CDRF_DODEFAULT;

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

LRESULT ContactListCtrl::OnEndLabelEdit(NMHDR* nmhdr)
{
	NMTVDISPINFO* dispInfo = reinterpret_cast<NMTVDISPINFO*>(nmhdr);
	BOOL result = FALSE;

	CTreeItem item(dispInfo->item.hItem, &_tree);
	
	AutoPtr<Node> node(reinterpret_cast<Node*>(item.GetData()));
	Assert(node != 0);

	switch(_transaction)
	{
	case AddContactTransaction:
		if(dispInfo->item.pszText != 0)
		{
			Account* account = GetAccountFor(item);
			Assert(account != 0);

			account->GetContactList()->AddContact(dispInfo->item.pszText, node->Parent);
			result = TRUE;
		}

		item.Delete();
		break;

	case AddGroupTransaction:
		if(dispInfo->item.pszText != 0)
		{
			Account* account = GetAccountFor(item);	
			Assert(account != 0);

			account->GetContactList()->AddGroup(dispInfo->item.pszText, node->Parent);
			result = TRUE;
		}

		item.Delete();

		break;
	
	case RenameGroupTransaction:
		if(dispInfo->item.pszText != 0)
		{
			node->Group->Rename(dispInfo->item.pszText);			
		}

		node.Release();
		break;

	case RenameContactTransaction:
		if(dispInfo->item.pszText)
		{
			node->Contact->SetAlias(dispInfo->item.pszText);
		}

		node.Release();
		break;
	}

	_transaction = NoTransaction;

	return result;
}

void ContactListCtrl::OnGroupCreate(Group* group, Group* parentGroup, int position, bool expanded)
{
	Assert(group != 0);
	Assert(position >= 0);

	Node* node = new Node(group, parentGroup, position);
	_groups.insert(std::make_pair(group, node));

	void* parent = parentGroup;
	if(parent == 0)
		parent = group->GetAccount();
	CreateItem(node, parent, expanded);
}

void ContactListCtrl::OnGroupDestroy(Group* group)
{
	Assert(group != 0);

	GroupTable::iterator i = _groups.find(group);
	if(i == _groups.end())
		return;

	if(! i->second->Item.IsNull())
		i->second->Item.Delete();

	delete i->second;
	_groups.erase(group);
}

void ContactListCtrl::OnGroupUpdate(Group* group)
{
	Assert(group != 0);
	
	GroupTable::iterator i = _groups.find(group);
	if(i == _groups.end())
		return;

	int image = _resourceManager->GetImageIndex(group);

	i->second->Item.SetImage(image, image);
	i->second->Item.SetText(group->GetName());
}

LRESULT ContactListCtrl::OnKeyDown(NMHDR* msg)
{
	NMTVKEYDOWN* keydown = reinterpret_cast<NMTVKEYDOWN*>(msg);

	Node* node = GetSelectedNode();
	if(node == 0)
		return 0;

	int result = 0;
	switch(keydown->wVKey)
	{
	case VK_F2:
		if(node->Type == GroupNodeType)
			OnMenuGroupContextRename(0, 0, 0);
		else if(node->Type == ContactNodeType)
			OnMenuContactContextRename(0, 0, 0);
		result = 1;

		break;

	case VK_DELETE:
		if(node->Type == GroupNodeType)
			OnMenuGroupContextDelete(0, 0, 0);
		else if(node->Type == ContactNodeType)
			OnMenuContactContextDelete(0, 0, 0);
		result = 1;

		break;

	case VK_RETURN:
		if(node->Type == ContactNodeType)
		{
			OnMenuContactContextMessage(0, 0, 0);
			result = 1;
		}

		break;
	}

	if(result == 1)
		SetMsgHandled(TRUE);

	return result;
}

LRESULT ContactListCtrl::OnItemExpanding(NMHDR* notifyMsg)
{
	NMTREEVIEW* msg = reinterpret_cast<NMTREEVIEW*>(notifyMsg);

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

void ContactListCtrl::OnMenuAccountContextConnect(UINT, int, HWND)
{
	if(Node* node = GetSelectedNode())
	{
		if(node->Account->GetConnectionStatus() == Disconnected)
			node->Account->Connect();
		else
			node->Account->Disconnect();
	}
}

void ContactListCtrl::OnMenuContactContextDelete(UINT, int, HWND)
{
	if(Node* node = GetSelectedNode())
	{
		if(node->Type != ContactNodeType)
			return;

		Contact* contact = node->Contact;
		ContactList* list = contact->GetAccount()->GetContactList();

		list->RemoveContact(contact, node->Parent);
	}
}

void ContactListCtrl::OnMenuContactContextMessage(UINT, int, HWND)
{
	if(Node* node = GetSelectedNode())
	{
		if(node->Type != ContactNodeType)
			return;


	}
}

void ContactListCtrl::OnMenuContactContextRename(UINT, int, HWND)
{
	if(Node* node = GetSelectedNode())
	{
		if(node->Type != ContactNodeType)
			return;

		_transaction = RenameContactTransaction;

		node->Item.EnsureVisible();
		node->Item.EditLabel();
	}
}

void ContactListCtrl::OnMenuGroupContextToggle(UINT, int, HWND)
{
	if(Node* node = GetSelectedNode())
	{
		if(node->Type != GroupNodeType)
			return;

		_tree.Expand(node->Item, TVE_TOGGLE);
	}
}

void ContactListCtrl::OnMenuGroupContextAdd(UINT, int, HWND)
{
	if(Node* node = GetSelectedNode())
	{
		if(node->Type != AccountNodeType && node->Type != GroupNodeType)
			return;

		_transaction = AddGroupTransaction;

		CTreeItem item = node->Item.InsertAfter(L"", TVI_LAST, _resourceManager->GetGroupImageIndex());
		item.SetData(reinterpret_cast<DWORD_PTR>(new Node(static_cast<Group*>(0), node->Group, 0)));
		item.EnsureVisible();
		item.EditLabel();
	}
}

void ContactListCtrl::OnMenuGroupContextAddContact(UINT, int, HWND)
{
	if(Node* node = GetSelectedNode())
	{
		if(node->Type != AccountNodeType && node->Type != GroupNodeType)
			return;
		
		Account* account = GetAccountFor(node->Item);
		Assert(account != 0);

		_transaction = AddContactTransaction;

		CTreeItem item = node->Item.InsertAfter(L"", TVI_LAST, _resourceManager->GetImageIndex(account->GetProtocolName()));
		item.SetData(reinterpret_cast<DWORD_PTR>(new Node(static_cast<Contact*>(0), node->Group, 1024)));
		item.EnsureVisible();
		item.EditLabel();
	}
}

void ContactListCtrl::OnMenuGroupContextDelete(UINT, int, HWND)
{
	if(Node* node = GetSelectedNode())
	{
		if(node->Type != GroupNodeType)
			return;

		int result = GetParent().MessageBox(
			L"Are you sure you wish to delete this group and all the items it contains?",
			L"Confirm Group Deletion",
			MB_YESNO | MB_ICONEXCLAMATION);
        
		if(result != IDYES)
			return;

		Group* group = node->Group;
		ContactList* list = group->GetAccount()->GetContactList();

		list->RemoveGroup(group, node->Parent);
	}
}

void ContactListCtrl::OnMenuGroupContextRename(UINT, int, HWND)
{
	if(Node* node = GetSelectedNode())
	{
		if(node->Type != GroupNodeType)
			return;

		_transaction = RenameGroupTransaction;
		node->Item.EnsureVisible();
		node->Item.EditLabel();
	}
}

LRESULT ContactListCtrl::OnSize(UINT, CSize size)
{
	_tree.MoveWindow(0, 0, size.cx, size.cy - 22, FALSE);
	_tree.Invalidate();

	return 0;
}

void ContactListCtrl::RemoveItems(Contact* contact)
{
	Assert(contact != 0);

	for(ContactTable::iterator i = _contacts.lower_bound(contact); i != _contacts.upper_bound(contact); ++i)
	{
		if(! i->second->Item.IsNull())
		{
			i->second->Item.Delete();
			i->second->Item = 0;
		}
	}
}

void ContactListCtrl::SetShowOfflineContacts(bool show)
{
	// Laziness is very good.
	if(show == _showOfflineContacts)
		return;

	_showOfflineContacts = show;

	// Regardless of whether the user enabled or disabled the option, 
	// we're going to be mucking with the tree quite a bit. Prevent redraws until we're done.
	// This makes us look *very* good.
	_tree.SetRedraw(FALSE);
	if(_showOfflineContacts)
	{
		for(ContactTable::iterator i = _contacts.begin(); i != _contacts.end(); i++)
		{
			if(i->second->Item.IsNull())
				CreateItem(i->second, i->second->Parent, false);
		}
	}
	else
	{
		for(ContactTable::iterator i = _contacts.begin(); i != _contacts.end(); i++)
		{
			if(! i->first->IsConnected())
				RemoveItems(i->first);
		}
	}
	_tree.SetRedraw(TRUE);
	_tree.Invalidate();
}

void ContactListCtrl::ShowAccountContextMenu(Node* node, const CPoint& pt)
{
	Assert(node != 0);
	Assert(node->Account != 0);
	
	// Fix up menu item text. If the account is connected, or in the process of connecting,
	// then we need to disconnect upon request.
	const wchar_t* text = L"Disconnect";
	if(node->Account->GetConnectionStatus() == Disconnected)
		text = L"Connect";

	_accountContext.GetSubMenu(0).ModifyMenu(ID_ACCOUNTCONTEXT_CONNECT, MF_BYCOMMAND, ID_ACCOUNTCONTEXT_CONNECT, text);

	UINT connected = node->Account->GetConnectionStatus() == Connected ? MF_ENABLED : MF_GRAYED;
	
	_accountContext.GetSubMenu(0).EnableMenuItem(ID_ACCOUNTCONTEXT_ADDGROUP, MF_BYCOMMAND | connected);
	_accountContext.GetSubMenu(0).TrackPopupMenu(ContextMenuFlags, pt.x, pt.y, *this);
}

void ContactListCtrl::ShowContactContextMenu(Node* node, const CPoint& pt)
{
	Assert(node != 0);

	_contactContext.GetSubMenu(0).SetMenuDefaultItem(ID_CONTACTCONTEXT_MESSAGE);
	_contactContext.GetSubMenu(0).TrackPopupMenu(ContextMenuFlags, pt.x, pt.y, *this);
}

void ContactListCtrl::ShowGroupContextMenu(Node* node, const CPoint& pt)
{
	Assert(node != 0);
	Assert(node->Group != 0);
	Assert(node->Group->GetAccount() != 0);

	// Enable/disable items based upon whether the account is connected.
	UINT enabled = node->Group->GetAccount()->GetConnectionStatus() == Connected ? MF_ENABLED : MF_GRAYED;
	for(int i = 0; i < _groupContext.GetSubMenu(0).GetMenuItemCount(); i++)
		_groupContext.GetSubMenu(0).EnableMenuItem(i, MF_BYPOSITION | enabled);

	// Change menu item text based on expanded/collapsed state.
	const wchar_t* text = L"&Collapse";
	if((node->Item.GetState(TVIS_EXPANDED) & TVIS_EXPANDED) == 0)
		text = L"&Expand";
	_groupContext.GetSubMenu(0).ModifyMenu(ID_GROUPCONTEXT_EXPAND,	MF_BYCOMMAND, ID_GROUPCONTEXT_EXPAND, text);
	_groupContext.GetSubMenu(0).SetMenuDefaultItem(ID_GROUPCONTEXT_EXPAND);
	
	_groupContext.GetSubMenu(0).TrackPopupMenu(ContextMenuFlags, pt.x, pt.y, *this);
}
