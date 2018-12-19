#include "stdafx.h"
#include "PlannerEvent.h"

IMPLEMENT_SERIAL(CPlannerEvent, CObject, 1000)

// CPlannerEvent CTORS
CPlannerEvent::CPlannerEvent(CString Description, CString* Place, int StartDate, int EndDate) : m_TaskCompleted(0)
{
	m_Description = Description;
	m_Place = Place;
	m_StartDate = StartDate;
	m_EndDate = EndDate;

}
CPlannerEvent::CPlannerEvent(CString Description, CString* Place, int StartDate, int EndDate, int StartDateMinutes, int EndDateMinutes) : m_TaskCompleted(0)
{
	
	m_Description = Description;
	m_Place = Place;
	m_StartDate = StartDate;
	m_EndDate = EndDate;
	m_StartDateMinutes = StartDateMinutes;
	m_EndDateMinutes = EndDateMinutes;

}
CPlannerEvent::CPlannerEvent(CString Description, CString* Place, int StartDate, int EndDate, int StartDateMinutes, int EndDateMinutes, bool AllDayEvent) : m_TaskCompleted(0)
{

	m_Description = Description;
	m_Place = Place;
	m_StartDate = StartDate;
	m_EndDate = EndDate;
	m_StartDateMinutes = StartDateMinutes;
	m_EndDateMinutes = EndDateMinutes;
	m_AllDayEvent = AllDayEvent;

	if (m_AllDayEvent) m_StartDate = 0;

}

//
// GetTimeStandardForm()
// Gets the time from its integer military time format
// and returns a new CString containing the traditional
// XX:XXa or XX:XXp format.
//
CString CPlannerEvent::GetTimeStandardForm()
{
	CString NewString;
	int NewDate = 0;
	bool IfPm = 0;
	
	if (m_StartDate > 12) IfPm = 1;

	if (IfPm)
	{
		NewDate = m_StartDate - 12;

		if (m_StartDateMinutes < 10)
			NewString.Format(L"%d:0%dp.m.", NewDate, m_StartDateMinutes);
		else
			NewString.Format(L"%d:%dp.m.", NewDate, m_StartDateMinutes);
	}
	else
	{
		NewDate = m_StartDate;

		if(m_StartDate == 0 && m_StartDateMinutes < 10)
			NewString.Format(L"12:0%da.m.", m_StartDateMinutes);
		else if(m_StartDate == 0)
			NewString.Format(L"12:%da.m.", m_StartDateMinutes);
		else if (m_StartDateMinutes < 10)
			NewString.Format(L"%d:0%da.m.", NewDate, m_StartDateMinutes);
		else
			NewString.Format(L"%d:%da.m.", NewDate, m_StartDateMinutes);
	}
	


	return NewString;
}

//
// Serialize()
// Responsible for saving all of the components
// of this object
//
void CPlannerEvent::Serialize(CArchive& ar)
{

	CObject::Serialize(ar);

	// Writing
	if (ar.IsStoring())
	{
		ar << m_Description
			<< *m_Place
			<< m_StartDate
			<< m_EndDate
			<< m_EndDateMinutes
			<< m_TaskCompleted
			<< m_AllDayEvent;

	}
	// Storing
	else
	{
		m_Place = new CString;

		ar >> m_Description
			>> *m_Place
			>> m_StartDate
			>> m_EndDate
			>> m_EndDateMinutes
			>> m_TaskCompleted
			>> m_AllDayEvent;
	}
}
