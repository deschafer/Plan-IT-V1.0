#include "stdafx.h"
#include "LLNode.h"
#include "Month.h"

IMPLEMENT_SERIAL(CLLNode, CObject, 1000);

//
// EnQueue()
// Adds a new CDay object to the end of the linked list structure
//
void CLLNode::EnQueue(CDay *DayObject) { // adds actor name to the end of the linked list

	// Local Vars
	Node* p_Aux;

	// Creating new node
	p_Aux = new Node;
	p_Aux->Object = DayObject;
	p_Aux->next = nullptr;

	if (p_Head == nullptr) { // If empty list

		p_Head = p_Aux;
		return;
	}
	// Otherwise, traverse and add to end of the list.
	Node *p_Traverse = p_Head;
	while (p_Traverse->next != nullptr)
		p_Traverse = p_Traverse->next;
	p_Traverse->next = p_Aux;
}

//
//
//
//
CDay* CLLNode::ReturnDayWithDate(int Date)
{
	Node* p_Traverse = p_Head;
	CString error;

	while (p_Traverse != NULL)
	{
		if (p_Traverse->Object->GetNumber() == Date)
			return p_Traverse->Object;
		p_Traverse = p_Traverse->next;
	}

	error.Format(L"ERROR -- Date not found in month -- ReturnDayWithDate -- CLLNode -- %d", Date);
	AfxMessageBox(error);

	return nullptr;
}

//
// ReturnNumberOfDays()
// Returns the number of days in this list
//
int CLLNode::ReturnNumberOfDays()
{
	Node* Curr = p_Head;
	int Count = 0;

	for (Count = 0; Curr != nullptr; Curr = Curr->next, Count++);

	return Count;
}

//
// Serialize()
// Responsible for saving all of the components
// of this object
//
void CLLNode::Serialize(CArchive &ar)
{
	Node* Curr = p_Head;

	CObject::Serialize(ar);

	// Writing
	if (ar.IsStoring())
	{
		m_Size = ReturnNumberOfDays();

		ar << m_Size;

		for (int i = 0; (Curr != nullptr); Curr = Curr->next)
		{
			ar << Curr->Object;
		}

	}
	// Storing
	else
	{
		Curr = new Node;
		p_Head = Curr;

		ar >> m_Size;

		for (int i = 0; i < m_Size; i++)
		{
			Curr->Object = new CDay;

			ar >> Curr->Object;
			if (i + 1 < m_Size)
			{
				Curr->next = new Node;
				Curr = Curr->next;
			}
		}

		Curr->next = nullptr;
	}
}