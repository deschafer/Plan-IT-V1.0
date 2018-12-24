#pragma once
#include "Month.h"

class CPlannerView;

// CYear command target
class CYear : public CObject
{
	DECLARE_SERIAL(CYear);
protected:

	int m_YearDate;			// The date of this year, like 2018
	bool m_LeapYear;		// Indicator variable if this is a leap year
	int m_CurrMonthCounter; // Counter for array access of months

	bool IsLeapYear();		// Returns true if this year is a leap year
	int FindBegDate(int day, int month, int year);
	CMonth *m_Months[12];	// Array of the twelve months within a year
	CMonth *m_CurrentMonth;	// Ptr to current month

public:
	// Returns the current month from today's date
	CMonth* CurrentMonth() { return m_Months[m_CurrMonthCounter]; };
	int ReturnYearDate() { return m_YearDate; }
	CMonth* MonthX(int x) { return m_Months[x]; }
	CMonth* MonthNum(int x) { return m_Months[x]; }
	bool MoveMonth(int x);
	void SetCurrentMonth(int x);
	virtual void Serialize(CArchive& ar) override;

	// CTOR for a CYear object
	CYear(int YearDate);

	CYear() {};
	virtual ~CYear() {};
};


