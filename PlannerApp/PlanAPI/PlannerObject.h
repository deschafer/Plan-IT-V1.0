#pragma once
#include "LLNode_Year.h"
// CPlannerObject command target

class CPlannerView;

// This is the main object file, 
// it will contain all the information that will be saved from a user
class CPlannerObject : public CObject
{
	DECLARE_SERIAL(CPlannerObject);
protected:
	
	CString m_PlannerName;	// Name for this planner object
	int m_BegYear;		// First year of this planner's year range
	int m_EndYear;		// Last year of this planner's year range
	int m_YearCount;	// The range of years for this planner
	CYear *m_CurrentYear;	// The current year of this planner object
	CLLNode_Year* YearList;	// The list structure containing the years of this structure

public:
	// Default CTOR should not be used
	CPlannerObject() {};
	CPlannerObject(int begYear, int endYear, CString Name = _T(""));
	virtual ~CPlannerObject() {};
	
	// Methods

	int SetDayObject(CDay* Day);
	int ReturnBegYear() { return m_BegYear; }
	const CString ReturnPlannerName() const { return m_PlannerName; }
	void IncrementCurrentYear(int x);
	void DecrementCurrentYear(int x);
	bool IsToday(CDay *Day);
	bool IsStartYear(int x) 
	{ 
	if (x == m_BegYear) return 1;
	else return 0;
	}
	bool IsLastYear(int x)
	{
		if (x == m_EndYear - 1) return 1;
		else return 0;
	}
	int MoveCurrMonth(int x);
	int MoveCurrYear(int x);
	CYear* ReturnCurrentYear() { return m_CurrentYear; }
	CMonth* ReturnCurrentMonth() { return m_CurrentYear->CurrentMonth(); }
	CMonth* ReturnPrevMonth();
	CMonth* ReturnNextMonth();
	CYear* ReturnYearThatMatches(int YearDate) { return YearList->ReturnYearThatMatches(YearDate); }

	virtual void Serialize(CArchive &ar) override;
};


