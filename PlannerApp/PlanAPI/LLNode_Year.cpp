#include "stdafx.h"
#include "LLNode_Year.h"

IMPLEMENT_SERIAL(CLLNode_Year, CObject, 1000);

// Adds a new node to the end of the list
void CLLNode_Year::EnQueue(CYear *YearObject)
{
	// Local Vars
	Node* p_Aux;

	// Creating new node
	p_Aux = new Node;
	p_Aux->Object = YearObject;
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


CYear* CLLNode_Year::ReturnYearThatMatches(int YearDate)
{


	Node *p_Traverse = p_Head;
	while (p_Traverse != nullptr)
	{
		

		if (YearDate == p_Traverse->Object->ReturnYearDate())
		{
			return p_Traverse->Object;
			break;
		}

		p_Traverse = p_Traverse->next;
	}

		
	
}

CYear* CLLNode_Year::Increment(CYear* CurrYearPtr)
{
	// Local Variables
	Node *p_Traverse = p_Head;

	while (p_Traverse->next != nullptr)
	{


		if (CurrYearPtr->ReturnYearDate() == p_Traverse->Object->ReturnYearDate())
		{
			return p_Traverse->next->Object;
			break;
		}

		p_Traverse = p_Traverse->next;
	}

	return CurrYearPtr;

}

CYear* CLLNode_Year::Decrement(CYear* CurrYearPtr)
{
	// Local Variables
	Node *p_Traverse = p_Head;
	Node *prev = nullptr;

	while (p_Traverse->next != nullptr)
	{
		prev = p_Traverse;

		if (CurrYearPtr->ReturnYearDate() == p_Traverse->next->Object->ReturnYearDate())
		{
			return prev->Object;
			break;
		}

		p_Traverse = p_Traverse->next;
	}

	return CurrYearPtr;

}

//
//
//
//
int CLLNode_Year::GetNumberOfYears()
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
void CLLNode_Year::Serialize(CArchive& ar)
{
	Node* Curr = p_Head;

	CObject::Serialize(ar);

	// Writing
	if (ar.IsStoring())
	{
		m_Size = GetNumberOfYears();

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
			Curr->Object = new CYear;

			ar >> Curr->Object;
			if (i + 1 < m_Size)
			{
				Curr->next = new Node;
				Curr = Curr->next;
			}
		}

	}
}