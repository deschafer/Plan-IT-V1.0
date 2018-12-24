// Month.cpp : implementation file
//

#include "stdafx.h"
#include "..\PlannerApp.h"
#include "Month.h"
#include "Day.h"

IMPLEMENT_SERIAL(CMonth, CObject, 1000);

CMonth::CMonth(int NumberDays, int BegDate, int MonthType, int Year)
{
	// Local vars
	int DayOfWeek = 0;

	// Setting appropriate variables
	m_NumberDays = NumberDays;
	m_BegDay = BegDate;
	m_MonthType = MonthType;
	m_Year = Year;
	DayOfWeek = BegDate;

	// Creating list to contain the days
	list_Days = new CLLNode;

	// This is creating the initial list of m_NumberDays day objects,
	// so the loop in within this function.
	// For modifying this list, the loop will be present in the CDay class methods.
	for (int i = 0; i < m_NumberDays; i++, DayOfWeek++)
	{

		if (DayOfWeek == 7)
			DayOfWeek = 0;
		// Adding each day element with the correct inital dimensions
		list_Days->EnQueue(new CDay(i + 1, DayOfWeek, m_MonthType, m_Year));
	}

}


bool CMonth::IsSixRowDisplay()
{
	// If a 31-day month, and first day is a sat or Fri, then 6 rows are needed
	if (m_NumberDays == 31 && (m_BegDay == 6 || m_BegDay == 5)) return 1;
	// If a 30-day month, and first day is fri, then 6 rows are needed
	else if (m_NumberDays == 30 && m_BegDay == 6) return 1;
	// Otherwise, the default number of 5 rows will be just fine
	else return 0;

}

// CMonth member functions

//
//
//
//
int CMonth::LastWeek()
{
	int week = 0;
	int begDay = m_BegDay;


	for (int i = 0; i < m_NumberDays; i++)
	{
		// IF the first week
		if (i == 0)
		{
			for (int day = m_BegDay; day < 7; day++)
			{
				i++;
			}
			week++;
		}
		else
		{

			i += 7;
			week++;
		}

	}

	return week;
}

//
// Serialize()
// Responsible for saving all of the components
// of this object
//
void CMonth::Serialize(CArchive &ar) 
{

	CObject::Serialize(ar);

	// Writing
	if (ar.IsStoring())
	{
		ar << m_BegDay
			<< m_MonthType
			<< m_Year
			<< m_NumberDays
			<< list_Days;
	}
	// Storing
	else
	{
		list_Days = new CLLNode;

		ar >> m_BegDay
			>> m_MonthType
			>> m_Year
			>> m_NumberDays
			>> list_Days;
	}

}
