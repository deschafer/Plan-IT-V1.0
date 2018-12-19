#pragma once


static const UINT VERSION_NUMBER = 1000;

class CPlannerEvent : public CObject
{
	DECLARE_SERIAL(CPlannerEvent);
private:
	CString m_Description;	// Description of this event
	CString* m_Place;		// Place for this event
	int m_StartDate;		// Starting date for this event
	int m_StartDateMinutes; // Starting date minutes for this event
	int m_EndDate;			// Ending date for this event
	int m_EndDateMinutes;   // ending date minutes for this event
	bool m_TaskCompleted;	// Whether this task is completed
	bool m_AllDayEvent;		// Whether this is an all day event

public:

	CString* GetDescription() { return &m_Description; }
	int GetStartTime() { return m_StartDate; }
	int GetEndTime() { return m_EndDate; }
	int GedEndTimeMinutes() { return m_EndDateMinutes; }
	int GetStartTimeMinutes() { return m_StartDateMinutes; }
	CString* GetPlace() { return m_Place; }
	CString GetTimeStandardForm();
	bool IsAllDayEvent() { return m_AllDayEvent; }
	bool IsCompleted() { return m_TaskCompleted; }
	void MarkCompleted() { m_TaskCompleted = true; }

	virtual void Serialize(CArchive& ar) override;

	CPlannerEvent(CString Description, CString* Place, int StartDate, int EndDate);
	CPlannerEvent(CString Description, CString* Place, int StartDate, int EndDate, int StartDateMinutes, int EndDateMinutes);
	CPlannerEvent(CString Description, CString* Place, int StartDate, int EndDate, 
		int StartDateMinutes, int EndDateMinutes, bool AllDayEvent);

	CPlannerEvent() {};
	~CPlannerEvent() {};
};

