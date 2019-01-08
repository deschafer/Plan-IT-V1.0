// Year.cpp : implementation file
//

#include <chrono>
#include <ctime>

#include "stdafx.h"
#include "..\PlannerApp.h"
#include "Year.h"
#include "..\PlannerAppView.h"

IMPLEMENT_SERIAL(CYear, CObject, 1000)


CYear::CYear(int YearDate) : m_CurrMonthCounter(0)
{
	// Local variables
	int Correction = 0;
	int MonthToday = 0;
	// Getting today's time
	time_t _Time = time(NULL);
	tm * TimeOut = localtime(&_Time);
	MonthToday = TimeOut->tm_mon;
	// Setting member vars
	m_YearDate = YearDate;
	// Determines if this object is a leap year
	(IsLeapYear()) ? m_LeapYear = 1, Correction = 1 : m_LeapYear = 0;

	// Initializing the months
	m_Months[0] = new CMonth((int)MonthType::JAN,  FindBegDate(1, 1, YearDate), 0, YearDate);
	m_Months[1] = new CMonth((int)MonthType::FEB + Correction,  FindBegDate(1, 2, YearDate), 1, YearDate);
	m_Months[2] = new CMonth((int)MonthType::MAR, FindBegDate(1, 3, YearDate), 2, YearDate);
	m_Months[3] = new CMonth((int)MonthType::APR, FindBegDate(1, 4, YearDate), 3, YearDate);
	m_Months[4] = new CMonth((int)MonthType::MAY, FindBegDate(1, 5, YearDate), 4, YearDate);
	m_Months[5] = new CMonth((int)MonthType::JUN, FindBegDate(1, 6, YearDate), 5, YearDate);
	m_Months[6] = new CMonth((int)MonthType::JUL, FindBegDate(1, 7, YearDate), 6, YearDate);
	m_Months[7] = new CMonth((int)MonthType::AUG, FindBegDate(1, 8, YearDate), 7, YearDate);
	m_Months[8] = new CMonth((int)MonthType::SEP, FindBegDate(1, 9, YearDate), 8, YearDate);
	m_Months[9] = new CMonth((int)MonthType::OCT, FindBegDate(1, 10, YearDate), 9, YearDate);
	m_Months[10] = new CMonth((int)MonthType::NOV, FindBegDate(1, 11, YearDate), 10, YearDate);
	m_Months[11] = new CMonth((int)MonthType::DEC, FindBegDate(1, 12, YearDate), 11, YearDate);

	// Set the current month and month counter
	m_CurrentMonth = m_Months[MonthToday];
	m_CurrMonthCounter = MonthToday;
}

// CYear member functions

//
// IsLeapYear()
// Returns a bool variable indicating whether this is a leap year
//
bool CYear::IsLeapYear()
{
	int YearDifference = 0;

	// Sets reference point at year 2000

	YearDifference = m_YearDate - 2000;

	return ((m_YearDate % 4) == 0) ? 1 : 0;
}


//
// FindBegDate()
// Returns the day of the week for a given date
//
int CYear::FindBegDate(int day, int month, int year)
{
	static int t[] = { 0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4 };
	year -= month < 3;
	return (year + year / 4 - year / 100 + year / 400 + t[month - 1] + day) % 7;
}

//
// MoveMonth()
// Moves the month counter and current month
// returns 0 if out of range or at end of year
// returns 1 if successful
// x should be either a pos. or neg. change
//
bool CYear::MoveMonth(int x)
{
	// Local Variables
	int temp = 0;

	// If not within range, or at the end of the year
	if ((m_CurrMonthCounter + x) < 0 || (m_CurrMonthCounter + x) > 11)
	{
		return 0;
	}

	// Adds the additional month change and resets the current month
	m_CurrMonthCounter += x;
	m_CurrentMonth = m_Months[m_CurrMonthCounter];

	// return true to indicate that it was successful
	return 1;
}

//
// SetCurrentMonth()
// Sets the current month counter
//
void CYear::SetCurrentMonth(int x)
{
	m_CurrMonthCounter = x;
}

//
// Serialize()
// Responsible for saving all of the components
// of this object
//
void CYear::Serialize(CArchive& ar)
{
	CObject::Serialize(ar);

	// Writing
	if (ar.IsStoring())
	{
		ar << m_YearDate
			<< m_LeapYear
			<< m_CurrMonthCounter;

		for (int i = 0; i < 12; i++)
		{
			ar << m_Months[i];
		}

	}
	// Storing
	else
	{
		ar >> m_YearDate
			>> m_LeapYear
			>> m_CurrMonthCounter;

		for (int i = 0; i < 12; i++)
		{
			m_Months[i] = new CMonth;
			ar >> m_Months[i];
		}
		
	}


}
