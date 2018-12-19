// Day.cpp : implementation file
//

#include "stdafx.h"
#include "..\PlannerApp.h"
#include "Day.h"

IMPLEMENT_SERIAL(CDay, CObject, 1000);

// 
// CDay()
// Std. ctor for this object
//
CDay::CDay(const int DayNumber, const int DayOfWeek, CPoint TopLeft, CPoint BottomRight, int Month, int Year)
{
	// Setting variables
	m_DayNumber = DayNumber;
	m_DayOfWeek = DayOfWeek;
	m_Year = Year;
	m_Month = Month;

	// Initialized array of LL's for events
	for (int i = 0; i < 24; i++)
	{
		EventListArray[i] = new CLLNode_Event();
	}

	m_AllDayEventList = new CLLNode_Event();
}

// CDay member functions

//
// GetEventsForTime()
// Gets the event list for a given time period
// -- WILL BE REMOVED WITH NEXT VERSION
//
CLLNode_Event* CDay::GetEventsForTime(int Time)
{

	return EventListArray[Time];

}

//
// GetEventForTime()
// Returns the ith number in the linked list associated with the
// time number passed in as the first argument.
//
CPlannerEvent* CDay::GetEventForTime(int time, int EventNumber)
{
	return EventListArray[time]->GetEventX(EventNumber);

}

//
// GetAllDayEvents()
// Gets the separate list of all day events
// -- WILL BE REMOVED WITH NEXT VERSION
//
CLLNode_Event* CDay::GetAllDayEvents()
{
	return m_AllDayEventList;

}

//
// GetAllDayEvent()
// Gets the ith number event in the all day event linked list
//
CPlannerEvent* CDay::GetAllDayEvent(int EventNumber)
{
	return m_AllDayEventList->GetEventX(EventNumber);
}

//
// AddEvent()
// Adds a new event to its appropriate list based
// on the data within the object
//
void CDay::AddEvent(CPlannerEvent * Event)
{
	// If an allday event, add to that specefic list
	if (Event->IsAllDayEvent())
	{
		m_AllDayEventList->EnQueue(Event);
	}
	// If not an all day event
	else
	{
		if (Event->GetStartTime() > 0 || Event->GetStartTime() < 23)
		{
			EventListArray[Event->GetStartTime()]->EnQueue(Event);
		}
		// Error
		else
		{
			AfxMessageBox(_T("Unable to add event -- time not within bounds."));
		}
	}
}

//
// IsEvent()
// Returns true if there is at least one event
// within this day object
//
bool CDay::IsEvent()
{

	for (int i = 0; i < 24; i++)
	{

		if (EventListArray[i]->GetHead() != nullptr) return 1;

	}

	if (GetAllDayEvents()->GetHead() != nullptr) return 1;

	return 0;
}

//
// GetNumberOfEvents()
// Returns number of events for a given time
//
int CDay::GetNumberOfEvents(int time)
{
	if (time >= 0 && time < 23)
	{
		return EventListArray[time]->GetNumberOfEvents();
	}
	return -1;
}

//
// GetNumberOfAllDayEvents()
// Returns the number of events for an allday event
//
int CDay::GetNumberOfAllDayEvents()
{
	return m_AllDayEventList->GetNumberOfEvents();
}

//
// DeleteXthElement()
// Deletes the Xth element from either the allday events
// or from events for specefied time
//
int CDay::DeleteXthElement(bool AllDay, int X, int time)
{
	// Delete Xth element from the allday list
	if (AllDay)
	{
		m_AllDayEventList->DeleteXthElement(X);
	}
	else
	{
		if(time < 24 && time >= 0) EventListArray[time]->DeleteXthElement(X);
	}

	return 0;
}

//
// DeleteEventList()
// Deletes either the allday event list or the list specefied
// the param time
//
int CDay::DeleteEventList(bool AllDay, int time)
{
	// Delete Xth element from the allday list
	if (AllDay)
	{
		m_AllDayEventList->DeleteList();
	}
	else
	{
		if (time < 24 && time >= 0) EventListArray[time]->DeleteList();
	}

	return 0;
}

//
// MarkEventAsCompleted()
// Marks the specefied event as completed -- first param
// indicates whether an all day event is to be marked.
//
void CDay::MarkEventAsCompleted(bool AllDay, int X, int time)
{

	// Delete Xth element from the allday list
	if (AllDay)
	{
		m_AllDayEventList->MarkXAsCompleted(X);
	}
	else
	{
		if (time < 24 && time >= 0) EventListArray[time]->MarkXAsCompleted(X);
	}


}

//
// MarkNextEventAsCompleted()
// MArks the first uncompleted event as completed in the 
// specefied event list.
//
void CDay::MarkNextEventAsCompleted(bool AllDay, int time)
{
	// Delete Xth element from the allday list
	if (AllDay)
	{
		m_AllDayEventList->MarkNextCompleted();
	}
	else
	{
		if (time < 24 && time >= 0) EventListArray[time]->MarkNextCompleted();
	}


}

//
// ClearEntireDay()
// Clears all the events lists for this day
//
int CDay::ClearEntireDay()
{
	m_AllDayEventList->DeleteList();

	for (int i = 0; i < 24; i++) EventListArray[i]->DeleteList();
	
	return 1;
}

//
// Serialize()
// Responsible for saving all of the components
// of this object
//
void CDay::Serialize(CArchive& ar)
{

	CObject::Serialize(ar);

	// Writing
	if (ar.IsStoring())
	{
		ar << m_Cell
			<< m_DayNumber
			<< m_DayOfWeek
			<< m_Year
			<< m_Month
			<< m_AllDayEventList;
			
		for (int i = 0; i < 24; i++)
		{
			ar << EventListArray[i];
		}

	}
	// Storing
	else
	{
		m_AllDayEventList = new CLLNode_Event;

		ar >> m_Cell
			>> m_DayNumber
			>> m_DayOfWeek
			>> m_Year
			>> m_Month
			>> m_AllDayEventList;
			
		for (int i = 0; i < 24; i++)
		{
			EventListArray[i] = new CLLNode_Event;

			ar >> EventListArray[i];
		}
	}
}
