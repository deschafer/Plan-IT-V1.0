// PlannerObject.cpp : implementation file
//

#include "stdafx.h"
#include "..\PlannerApp.h"
#include "PlannerObject.h"

IMPLEMENT_SERIAL(CPlannerObject, CObject, 1000);

// CPlannerObject

CPlannerObject::CPlannerObject(int begYear, int endYear, CPlannerView* view) : m_CurrentYear(nullptr)
{
	// Local vars
	int YearDate = 0;

	// Setting class variables
	m_BegYear = begYear;
	m_EndYear = endYear;
	m_YearCount = endYear - begYear;
	YearList = new CLLNode_Year;

	// Getting the date of this year
	time_t Time = time(NULL);
	tm* p_Time = localtime(&Time);

	YearDate = p_Time->tm_year + 1900;
	
	// Now creating the linked list of objects
	for (int i = 0; i < m_YearCount; i++)
	{
		// Adding each day element with the correct inital dimensions
		// View has to be passed in for the cells for each day object
		// This has to dynamically create a CYear object each time
		CYear * object = new CYear(begYear + i, view);

		// Adding this new object to the end of the list
		YearList->EnQueue(object);

		// If the year added is the current year of todays date, then
		// set it as the current year
		if (begYear + i == YearDate) m_CurrentYear = object;
		
	}

	if (m_CurrentYear == nullptr) m_CurrentYear = YearList->ReturnTopNode();
}


// CPlannerObject member functions

void CPlannerObject::IncrementCurrentYear(int x)
{

	for (int i = 0; i < x; i++)
	{
		m_CurrentYear = YearList->Increment(m_CurrentYear);
	}
}




void CPlannerObject::DecrementCurrentYear(int x)
{
	
	for (int i = 0; i < x; i++)
	{
		m_CurrentYear = YearList->Decrement(m_CurrentYear);
	}

}


CMonth* CPlannerObject::ReturnPrevMonth()
{
	int Month = 0;

	Month = m_CurrentYear->CurrentMonth()->ReturnMonthType();

	if (IsStartYear(m_CurrentYear->ReturnYearDate()) && m_CurrentYear->CurrentMonth()->ReturnMonthType() == 0)
		return nullptr;

	if (Month == 0)
	{

		DecrementCurrentYear(1);
		CMonth* Prev; 
		m_CurrentYear->SetCurrentMonth(11);
		Prev = m_CurrentYear->CurrentMonth();
		IncrementCurrentYear(1);
		m_CurrentYear->SetCurrentMonth(Month);
		return Prev;
	}
	else
	{
		m_CurrentYear->MoveMonth(-1);

		CMonth* PrevMonth = m_CurrentYear->CurrentMonth();

		m_CurrentYear->MoveMonth(1);

		return PrevMonth;
	}

}

CMonth* CPlannerObject::ReturnNextMonth()
{
	CMonth* NextMonth;
	int Month = 0;

	Month = m_CurrentYear->CurrentMonth()->ReturnMonthType();

	if (IsLastYear(m_CurrentYear->ReturnYearDate()) && m_CurrentYear->CurrentMonth()->ReturnMonthType() == 11)
		return nullptr;

	if (!m_CurrentYear->MoveMonth(1))
	{
		IncrementCurrentYear(1);
		m_CurrentYear->SetCurrentMonth(0);
		NextMonth = m_CurrentYear->CurrentMonth();
		DecrementCurrentYear(1);
		m_CurrentYear->SetCurrentMonth(11);
	}
	else
	{
		NextMonth = m_CurrentYear->CurrentMonth();
		m_CurrentYear->MoveMonth(-1);
	}
	

	return NextMonth;
}

int CPlannerObject::SetDayObject(CDay* Day)
{

	CYear* CurrYear = YearList->ReturnYearThatMatches(Day->GetYear());

	CDay* CurrDay = CurrYear->MonthNum(Day->GetMonth())->ReturnDayWithDate(Day->GetNumber());

	CurrDay = Day;

	return 0;
}

bool CPlannerObject::IsToday(CDay *Day)
{
	// Getting today's time information
	time_t Time = time(NULL);
	tm* p_Time = localtime(&Time);

	if (Day->GetMonth() == p_Time->tm_mon &&
		Day->GetYear() == p_Time->tm_year + 1900 &&
		Day->GetNumber() == p_Time->tm_mday)
	{
		return 1;
	}

	else
		return 0;

}

//
//
//
//
int CPlannerObject::MoveCurrMonth(int Movement)
{

	// Local Vars
	int error = 0;
	bool NoChange = 0;
	int PrevYearDate = -1;

	
	// Moving to the next month
	if (Movement > 0)
	{

		

		CMonth* CurrMonth = m_CurrentYear->CurrentMonth();

		if (CurrMonth->ReturnMonthType() == 11)
			error = 0;

		error = m_CurrentYear->MoveMonth(1);

		if (!error)
		{


			PrevYearDate = m_CurrentYear->ReturnYearDate();

			IncrementCurrentYear(1);
			if (PrevYearDate != m_CurrentYear->ReturnYearDate()) m_CurrentYear->SetCurrentMonth(0);
			else
			{
				//NoChange = 1;
			}
		}
	}
	// Moving to the previous month
	else
	{
		

		CMonth* CurrMonth = m_CurrentYear->CurrentMonth();

		if (CurrMonth->ReturnMonthType() == 0)
			error = 0;

		error = m_CurrentYear->MoveMonth(-1);

		if (!error)
		{

			PrevYearDate = m_CurrentYear->ReturnYearDate();

			DecrementCurrentYear(1);
			if (PrevYearDate != m_CurrentYear->ReturnYearDate()) m_CurrentYear->SetCurrentMonth(11);
			else
			{
				//NoChange = 1;
			}
		}
	}

	return NoChange;

}

int CPlannerObject::MoveCurrYear(int x)
{
	// Local Vars
	int PrevYearDate = -1;
	int Change = 0;
	int PrevMonthDate = m_CurrentYear->CurrentMonth()->ReturnMonthType();


	if (x > 0)
	{

		PrevYearDate = m_CurrentYear->ReturnYearDate();

		IncrementCurrentYear(1);
		m_CurrentYear = ReturnCurrentYear();
		m_CurrentYear->SetCurrentMonth(PrevMonthDate);

		if (PrevYearDate != m_CurrentYear->ReturnYearDate()) Change = 1;

	}
	else
	{

		PrevYearDate = m_CurrentYear->ReturnYearDate();

		DecrementCurrentYear(1);
		m_CurrentYear = ReturnCurrentYear();
		m_CurrentYear->SetCurrentMonth(PrevMonthDate);

		if (PrevYearDate != m_CurrentYear->ReturnYearDate()) Change = 1;

	}

	return Change;

}

//
// Serialize()
// Responsible for saving all of the components
// of this object
//
void CPlannerObject::Serialize(CArchive &ar)
{

	CObject::Serialize(ar);

	// Writing
	if (ar.IsStoring())
	{
		ar << m_BegYear
			<< m_EndYear
			<< m_YearCount
			<< m_CurrentYear
			<< YearList;
	}
	// Storing
	else
	{
		YearList = new CLLNode_Year;

		ar >> m_BegYear
			>> m_EndYear
			>> m_YearCount
			>> m_CurrentYear
			>> YearList;
	}

}