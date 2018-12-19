#pragma once
//#include "LLNode.h"
#include "PlannerEvent.h"

class CLLNode_Event :public CObject
{
	DECLARE_SERIAL(CLLNode_Event);
private:
	struct Node {								// Simple Linked List struct

		CPlannerEvent *Object;
		Node *next;
	};
	Node* p_Head;								// Head pointer

	int m_Size;

public:

	Node* GetHead() { return p_Head; }			// Returns the head pointer
	void EnQueue(CPlannerEvent *PlannerObject);	// Adds a new node to the end of the list
	CPlannerEvent* GetEventX(int X);			// Returns Xth event in the list
	int GetNumberOfEvents();					// Returns the number of events
	void DeleteXthElement(int X);				// Deletes Xth element from list, starts at 1
	void DeleteList();							// Deletes the entire list
	void MarkXAsCompleted(int X);				// Marks Xth element as completed
	bool IsXCompleted(int X);					// Returns true if Xth element is completed
	void MarkNextCompleted();					// Marks the first uncompleted event as completed
	Node* p_EventAux;							// Auxillary pointer

	virtual void Serialize(CArchive &ar);

	CLLNode_Event() { p_Head = nullptr; }
	~CLLNode_Event() {}
};

