#pragma once
#include <memory>


#include "Day.h"
#include "LLNode.h"

// Month types
enum class MonthType {JAN = 31, FEB = 28, MAR = 31, APR = 30, MAY = 31, JUN = 30, JUL = 31, AUG = 31, SEP = 30, OCT = 31, NOV = 30, DEC = 31};

// CMonth command target
class CMonth : public CObject
{
	DECLARE_SERIAL(CMonth);
protected:
	
	int m_BegDay;
	int m_MonthType;		// Holds the type of this month (0 = JAN, ..., 11 = DEC)
	int m_Year;
	CLLNode *list_Days;			// List containing day objects

public:

	int m_NumberDays;		// Holds the number of days for this month
	
	CMonth() {};
	CMonth(int NumberDays, int BegDate, int MonthType, int Year);
	virtual ~CMonth() {};

	CLLNode* GetList() { return list_Days; }
	int ReturnMonthType() { return m_MonthType; }
	bool IsSixRowDisplay();
	int ReturnBegDate() { return m_BegDay; }
	int LastWeek();
	CDay* ReturnDayWithDate(int Date) { return list_Days->ReturnDayWithDate(Date); }
	virtual void Serialize(CArchive &ar) override;
};



