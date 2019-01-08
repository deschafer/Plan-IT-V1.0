#pragma once
#include "PlannerEvent.h"
#include "LLNode_Event.h"
//class CLLNode_Event;

// CDay command target

class CDay : public CObject
{
	DECLARE_SERIAL(CDay);
public:
	virtual ~CDay(void) {}

	CDay(const int DayNumber, const int DayOfWeek, CPoint TopLeft, CPoint BottomRight, int Month, int Year);
	CDay(const int DayNumber, const int DayOfWeek, int Month, int Year);
	CDay(void) {};

	CLLNode_Event* GetPlannerEvent(int x) { return EventListArray[x]; }
	unsigned GetNumber() { return m_DayNumber; }
	unsigned GetDayOfWeek() { return m_DayOfWeek; }
	int GetYear() { return m_Year; }
	int GetMonth() { return m_Month; }
	void AddEvent(CPlannerEvent *Event);
	CLLNode_Event* GetEventsForTime(int Time);
	CPlannerEvent* GetEventForTime(int time, int EventNumber);
	int GetNumberOfEvents(int time);
	CLLNode_Event* GetAllDayEvents();
	CPlannerEvent* GetAllDayEvent(int EventNumber);
	int GetNumberOfAllDayEvents();
	bool IsEvent();
	int DeleteXthElement(bool AllDay, int X, int time = 0);
	int DeleteEventList(bool AllDay, int time = 0);
	int ClearEntireDay();
	void MarkEventAsCompleted(bool AllDay, int X, int time = 0);
	void MarkNextEventAsCompleted(bool AllDay, int time = 0);
	void SwapEvent(CPlannerEvent* Event, bool AllDay, bool Prev, int time = 0);

	virtual void Serialize(CArchive &ar) override;

protected:
	unsigned m_DayNumber;	// Part of this day object's date
	unsigned m_DayOfWeek;
	CLLNode_Event* EventListArray[24];
	CLLNode_Event* m_AllDayEventList;
	int m_Year;
	int m_Month;
	
};


