#include "stdafx.h"
#include "LLNode_Event.h"

IMPLEMENT_SERIAL(CLLNode_Event, CObject, 1000);

// Adds a new node to the end of the list
void CLLNode_Event::EnQueue(CPlannerEvent *EventObject)
{
	// Local Vars
	Node* p_Aux;

	// Creating new node
	p_Aux = new Node;
	p_Aux->Object = EventObject;
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
// GetEventX()
// Returns the Xth item in this linked list,
// will return nullptr if no event is at that location.
//
CPlannerEvent* CLLNode_Event::GetEventX(int X)
{
	// Local Variables
	Node* p_Aux;

	p_Aux = GetHead();

	// Traversing the linked list X times
	for (int i = 0; p_Aux != nullptr && i < X; p_Aux = p_Aux->next, i++)
	{

	}

	// If not found in the list/list is empty
	if (p_Aux == nullptr)
	{
		//AfxMessageBox(_T("Xth item is not in this LL, returning nullptr"));
		return nullptr;
	}
	// otherwise return the corresponding event
	else return p_Aux->Object;
	
}

//
// GetNumberOfEvents()
// Returns the number of how many objects are stored
// in this list.
//
int CLLNode_Event::GetNumberOfEvents()
{
	int Count = 0;
	Node* p_Aux = GetHead();

	// Gets the count of the list
	for (; p_Aux != nullptr; p_Aux = p_Aux->next, Count++);

	return Count;
}

//
// DeleteXthElement()
// Deletes the Xth element in the list,
// the counting starts at 1.
//
void CLLNode_Event::DeleteXthElement(int x)
{
	// Local Vars
	Node *p_Traverse = p_Head;
	Node* Prev = nullptr;

	for (int i = 0; i < x && (p_Traverse != nullptr); i++)
	{
		Prev = p_Traverse;
		p_Traverse = p_Traverse->next;
	}

	if (p_Traverse == nullptr)
	{

	}
	else
	{
		// Breaking the links
		if (Prev == nullptr) p_Head = p_Head->next;
		else Prev->next = p_Traverse->next;
		delete p_Traverse;
	}
}

//
// DeleteList()
// Delets the entire linked list
//
void CLLNode_Event::DeleteList()
{
	// Local Vars
	Node *p_Traverse = p_Head;
	Node* Prev = nullptr;

	while (p_Traverse != nullptr)
	{

		Prev = p_Traverse;
		p_Traverse = p_Traverse->next;

		delete Prev;
	}

	p_Head = nullptr;
}

//
// MarkXAsCompleted() 
// Marks specefied event as completed
//
void CLLNode_Event::MarkXAsCompleted(int x)
{
	// Local Vars
	Node *p_Traverse = p_Head;

	for (int i = 0; i < x && (p_Traverse != nullptr); i++)
	{
		p_Traverse = p_Traverse->next;
	}

	if (p_Traverse != nullptr)
	{
		p_Traverse->Object->MarkCompleted();
	}
}


//
// IsXCompleted()
// Returns true if the specefied event is completed
//
bool CLLNode_Event::IsXCompleted(int x)
{
	// Local Vars
	Node *p_Traverse = p_Head;

	for (int i = 0; i < x && (p_Traverse != nullptr); i++)
	{
		p_Traverse = p_Traverse->next;
	}

	if (p_Traverse != nullptr)
	{
		return p_Traverse->Object->IsCompleted();
	}
	else return false;
}

//
//
// Marks the first uncompleted event as completed
//
void CLLNode_Event::MarkNextCompleted()
{
	// Local Vars
	Node *p_Traverse = p_Head;

	for (int i = 0; (p_Traverse != nullptr) && p_Traverse->Object->IsCompleted(); i++)
	{
		p_Traverse = p_Traverse->next;
	}

	if (p_Traverse != nullptr)
	{
		p_Traverse->Object->MarkCompleted();
	}

}

//
// Serialize()
// Responsible for saving all of the components
// of this object
//
void CLLNode_Event::Serialize(CArchive &ar)
{
	Node* Curr = p_Head;

	CObject::Serialize(ar);

	// Writing
	if (ar.IsStoring())
	{
		m_Size = GetNumberOfEvents();
		ar << m_Size;
		for (int i = 0; (Curr != nullptr); Curr = Curr->next)
		{
			ar << Curr->Object;
		}

	}
	// Storing
	else
	{
		ar >> m_Size;
		for (int i = 0; i < m_Size; i++)
		{
			
			CPlannerEvent *Event = new CPlannerEvent;
			ar >> Event;
			EnQueue(Event);
		}
	}
}

//
// SwapWithNext()
// Swaps the event pointed to with the next event
//
void CLLNode_Event::SwapWithNext(CPlannerEvent* Event)
{
	Node* p_Traverse = nullptr;
	Node* p_Prev = nullptr;
	p_Traverse = GetHead();

	for (int i = 0; (p_Traverse) != nullptr && p_Traverse->Object != Event; )
	{
		p_Prev = p_Traverse;
		p_Traverse = p_Traverse->next;
	}

	if (p_Traverse == nullptr);
	else
	{
		Node* NextNode = p_Traverse->next;
		if (NextNode != nullptr)
		{
			p_Traverse->next = NextNode->next;
			NextNode->next = p_Traverse;
			if (p_Prev != nullptr)
			{
				p_Prev->next = NextNode;
			}
			else
			{
				p_Head = NextNode;
			}
		}
	}
}

//
// SwapWithPrevious()
// Swaps the event pointed to with the previous event
//
void CLLNode_Event::SwapWithPrevious(CPlannerEvent* Event)
{
	Node* p_Traverse = nullptr;
	Node* p_Prev = nullptr;
	Node* p_PrevPrev = nullptr;
	p_Traverse = GetHead()->next;
	p_Prev = GetHead();

	for (int i = 0; (p_Traverse) != nullptr && p_Traverse->Object != Event; )
	{
		p_PrevPrev = p_Prev;
		p_Prev = p_Prev->next;
		p_Traverse = p_Traverse->next;
	}

	// case where the element to be swapped is the second element
	if (GetHead()->next->Object == Event)
	{
		Node* node = p_Head;
		p_Head = p_Head->next;
		Node* Rem = p_Head->next;
		p_Head->next = node;
		node->next = Rem;
		return;
	}

	if (p_Traverse == nullptr);
	else
	{
		p_Prev->next = p_Traverse->next;
		p_Traverse->next = p_Prev;

		if (p_PrevPrev != nullptr)
		{
			p_PrevPrev->next = p_Traverse;
		}
		else
		{
			p_Head = p_Prev;
			p_Prev->next = p_Traverse;
		}
	}
}