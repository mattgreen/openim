#include "ContactListCtrlActions.hpp"

#include "ContactListCtrl.hpp"

LRESULT ContactListCtrlActions::OnAccountContextConnect(UINT, int, HWND, BOOL& handled)
{
	Assert(Control != 0);
	handled = TRUE;

	if(Node* node = Control->GetSelectedNode())
	{
		if(node->Account->GetConnectionStatus() == Disconnected)
			node->Account->Connect();
		else
			node->Account->Disconnect();
	}

	return 0;
}

LRESULT ContactListCtrlActions::OnContactAdd(UINT, int, HWND, BOOL& handled)
{
	Assert(Control != 0);
	handled = TRUE;
	
	Node* node = Control->GetSelectedNode();
	if(node == 0 || node->Type != GroupNodeType)
		return 0;
	
	Control->StartGroupInsert
}

LRESULT ContactListCtrlActions::OnContactDelete(UINT, int, HWND, BOOL& handled)
{
	Assert(Control != 0);
	handled = true;

	Node* node = Control->GetSelectedNode();
	if(node == 0 || node->Type != ContactNodeType)
		return 0;

	Contact* contact = node->Contact;
	ContactList* list = contact->GetAccount()->GetContactList();

	list->RemoveContact(contact, node->Parent);

	return 0;
}

LRESULT ContactListCtrlActions::OnGroupContextToggle(UINT, int, HWND, BOOL& handled)
{
	Assert(Control != 0);
	handled = TRUE;

	if(Node* node = Control->GetSelectedNode())
	{
		if(node->Type != GroupNodeType)
			return 0;

		Control->GetTree().Expand(node->Item, TVE_TOGGLE);
	}

	return 0;
}

LRESULT ContactListCtrlActions::OnGroupAdd(UINT, int, HWND, BOOL& handled)
{
	Assert(Control != 0);
	handled = TRUE;

	Node* node = Control->GetSelectedNode();
	if(node == 0 || (node->Type != AccountNodeType && node->Type != GroupNodeType))
		return 0;

	Control->StartGroupInsert(node);

	return 0;
}

LRESULT ContactListCtrlActions::OnGroupDelete(UINT, int, HWND, BOOL& handled)
{
	Assert(Control != 0);
	handled = true;

	int result = MessageBox(Control->GetParent(), 
		L"Are you sure you wish to delete this group? All contacts within this group, and all subgroups will be removed as well.",
		L"Confirm Group Deletion",
		MB_YESNO | MB_ICONEXCLAMATION);

	if(result != IDYES)
		return 0;

	Node* node = Control->GetSelectedNode();
	if(node == 0 || node->Type != GroupNodeType)
		return 0;

	Group* group = node->Group;
	ContactList* list = group->GetAccount()->GetContactList();

	list->RemoveGroup(group, node->Parent);

	return 0;
}