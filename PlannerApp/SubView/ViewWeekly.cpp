
#include "stdafx.h"
#include "ViewBase.h"

#include "..\Dialog\DialogAddEvent.h"


//
// CViewWeekly()
// Constructor for this view object
//
CViewWeekly::CViewWeekly(unsigned *Rows, unsigned *Columns, int *Height,
	int *Width, int* TopBarSize, CPlannerObject* Object)
	: CViewBase(Rows, Columns, Height, Width, TopBarSize, Object), m_RowCounter(0), m_WeekStartDay(0), m_DayCounter(0), 
	m_Change(1), m_Offset(0), m_LastWeek(0), m_FirstWeek(0), m_SelectedTime(0), m_Reminder(1), m_Initialized(0), 
	m_FirstMonthFirstWeek(0), m_LastMonthLastWeek(0), m_ContextMenuSelectedDay(nullptr)
{
	// Setting all of the current displayed days to null
	for (int i = 0; i < 7; i++)
	{
		m_Week[i] = nullptr;
	}
	
	// Sets the number of rows for this view
	*m_Rows = 10;

}


//
// InitilizeWndVariables()
// Initializes view-specefic information such as the width and height variables
// that change when the window is resized.
//
void CViewWeekly::InitilizeWndVariables(CPlannerView* View)
{

	// Gets the current year
	m_Year = m_Planner->ReturnCurrentYear();
	// Gets the current month
	m_CurrentMonth = m_Year->CurrentMonth();

	// Resets row size
	*m_Rows = 10;

	// Sets the appropriate dimensions for each row or column based
	// on the total number of rows or columns set
	m_HeightPortion = (*m_Height - *m_TopBarSize) / *m_Rows;
	m_WidthPortion = *m_Width / *m_Columns;
	m_AllDayEventSectionSize = m_HeightPortion;


	// Set the indicator variable back to false
	m_SizeChanged = false;

	// Sets the array of month strings
	if (!m_MonthsComplete)
	{
		m_MonthStrings[0] = "January";
		m_MonthStrings[1] = "February";
		m_MonthStrings[2] = "March";
		m_MonthStrings[3] = "April";
		m_MonthStrings[4] = "May";
		m_MonthStrings[5] = "June";
		m_MonthStrings[6] = "July";
		m_MonthStrings[7] = "August";
		m_MonthStrings[8] = "September";
		m_MonthStrings[9] = "October";
		m_MonthStrings[10] = "November";
		m_MonthStrings[11] = "December";
		m_MonthsComplete = 1;
	}
	// Sets the array of day strings
	if (!m_DaysComplete)
	{
		m_DayStrings[0] = "Sunday";
		m_DayStrings[1] = "Monday";
		m_DayStrings[2] = "Tuesday";
		m_DayStrings[3] = "Wednesday";
		m_DayStrings[4] = "Thursday";
		m_DayStrings[5] = "Friday";
		m_DayStrings[6] = "Saturday";
		m_DaysComplete = 1;
	}

	// Set this object as intialized
	m_Initialized = 1;
}


//
// DrawLayout()
// Draws the week style layout for this view
//
void CViewWeekly::DrawLayout(CDC* pDC, CPlannerView* View)
{

	// Initialize variables if not already
	if (m_Width == 0 || m_Height == 0 || m_SizeChanged)
	{
		m_CurrentView = View;
		InitilizeWndVariables(View);
		m_CurrentMonth->ResetCells(m_WidthPortion, m_HeightPortion);
	}

	// Sets the default color for the background
	FillBackground(pDC, View);
	

	// Drawing the month section of the UI
	DrawMonthSection(pDC, View);

	// Drawing the cells for this view
	DrawCells(View, pDC);

	// Drawing the new grid
	DrawWeeklyGrid(pDC);

	// Drawing the top bar/ days of the week section of the UI
	DrawTopBarSection(pDC, View);

	//if(m_Reminder) DrawReminder(pDC);

	// If scrolling was present brefore, reset the indicator
	if (m_Scroll) m_Scroll = 0;
}

//
// DrawReminder()
// Not complete -- used to draw reminder box using multithreading
//
void CViewWeekly::DrawReminder(CDC* pDC)
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));
	
	if (!CreateProcess(NULL,   // No module name (use command line)
		NULL,        // Command line
		NULL,           // Process handle not inheritable
		NULL,           // Thread handle not inheritable
		FALSE,          // Set handle inheritance to FALSE
		0,              // No creation flags
		NULL,           // Use parent's environment block
		NULL,           // Use parent's starting directory 
		&si,            // Pointer to STARTUPINFO structure
		&pi)           // Pointer to PROCESS_INFORMATION structure
		)
	{
		printf("CreateProcess failed (%d).\n", GetLastError());
		return;
	}


	m_Reminder = 0;

	// Close process and thread handles. 
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
}

//
// FillBackground
// Fills in the background colors for this view
//
void CViewWeekly::FillBackground(CDC* pDC, CPlannerView* View)
{
	// Local Variables
	CRect EnclosingRect(CPoint(0, 0), CPoint(*m_Width, *m_Height));
	CBrush NewBrush;

	// Creating the new brush
	NewBrush.CreateSolidBrush(RGB(180, 180, 180));

	// Filling the client area
	for (int i = 0; i < *m_Columns; i++)
	{
		if ((i % 2) == 0)
		{
			pDC->FillRect(CRect(CPoint((i)*m_WidthPortion, *m_TopBarSize), CPoint((i + 1)*m_WidthPortion, *m_Height)), new CBrush(RGB(200, 200, 200)));
		}
		else
		{
			pDC->FillRect(CRect(CPoint((i)*m_WidthPortion, *m_TopBarSize), CPoint((i + 1)*m_WidthPortion, *m_Height)), new CBrush(RGB(221, 221, 235)));
		}

	}
	
	pDC->FillRect(CRect(CPoint(0, *m_TopBarSize), CPoint(m_WidthPortion, *m_Height)), new CBrush(RGB(200, 200, 200)));

	// Filling all day event region
	pDC->FillRect(CRect(CPoint(0, *m_TopBarSize), CPoint(*m_Width, *m_TopBarSize + m_HeightPortion)), new CBrush(RGB(150, 150, 150)));
	
	// Deleting the brush object
	NewBrush.DeleteObject();
}

//
// DrawTopBarSection()
// Draws the top bar section for this view, an important part of drawing
// the UI
//
void CViewWeekly::DrawTopBarSection(CDC* pDC, CPlannerView* View)
{

	// Local Variables
	int FontSize = 24;
	CSize Size = 0;
	CFont font;
	CFont fontAux;
	CRect EnclosingRect(CPoint(0, 37), CPoint(*m_Width, *m_TopBarSize));
	CBrush NewBrush;
	CString Str;

	// Set the background mode as transparent so text will
	// appear accordingly
	pDC->SetBkMode(TRANSPARENT);

	// Drawing the top bar section
	pDC->MoveTo(0, *m_TopBarSize);
	pDC->LineTo(*m_Width, *m_TopBarSize);

	// Create the font objects
	VERIFY(font.CreateFont(
		FontSize,                  // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("Calibri")));           // lpszFacename

	// Aux font for other styles
	VERIFY(fontAux.CreateFont(
		FontSize - 2,              // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("Calibri")));           // lpszFacename
	

	// Save the default font, and set the new font
	CFont* def_font = pDC->SelectObject(&font);

	// Create the new brush
	NewBrush.CreateSolidBrush(RGB(140, 120, 120));

	// Set the color of the text 
	pDC->SetTextColor(RGB(50, 50, 50));

	// Color in the rectangle for this section of the topbar
	pDC->FillRect(EnclosingRect, &NewBrush);

	// Drawing each day of the week's text
	for (int i = 0; i < 7; i++)
	{
		// For all days that are empty (without events)
		if (m_Week[i] != nullptr && m_Week[i]->IsEvent());
		else
			pDC->TextOutW(m_WidthPortion * i + (m_WidthPortion / 50), 37, m_DayStrings[i]);
	}

	// Selecting the aux font to correctly display the dates
	pDC->SelectObject(&fontAux);

	// Printing out the date for each day in a different font style
	for (int i = 0; i < 7; i++)
	{
		if (m_Week[i] != nullptr)
		{
			Str.Format(L"%d / %d", m_Week[i]->GetMonth() + 1, m_Week[i]->GetNumber());

			Size = pDC->GetTextExtent(Str);

			pDC->TextOutW(i * m_WidthPortion + (m_WidthPortion - Size.cx - 10), *m_TopBarSize - 20, Str);
		}
	}

	// Deleting the auxilery font
	fontAux.DeleteObject();

	// Creating a new font
	VERIFY(fontAux.CreateFont(
		FontSize,              // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_BOLD,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("Calibri")));           // lpszFacename

	// Selecting this new font
	pDC->SelectObject(&fontAux);

	// Now printing bold weekday names to indicate that an event takes place
	// on that day
	for (int i = 0; i < 7; i++)
	{
		// If there is an event on the day of the week
		if (m_Week[i] != nullptr && m_Week[i]->IsEvent())
			pDC->TextOutW(m_WidthPortion * i + (m_WidthPortion / 50), 37, m_DayStrings[i]);
	}

	// Selecting the old object
	pDC->SelectObject(def_font);

	// Deleting objects
	font.DeleteObject();
	fontAux.DeleteObject();
	NewBrush.DeleteObject();
}


//
// DrawWeeklyGrid()
// Draws the line for the weekly grid based off of the updated height
// and width variables
//
void CViewWeekly::DrawWeeklyGrid(CDC* pDC)
{
	// Local Variables
	CString String;
	int i = 0 + m_Offset;
	CPen aPen;
	CPen* OldPen;
	CFont aFont;
	CFont* OldFont;

	// Creating the font object
	VERIFY(aFont.CreateFont(
		20,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("Calibri")));           // lpszFacename

	OldFont = pDC->SelectObject(&aFont);

	// Draws rows based on amount of rows and current
	// dimensions of client area
	for (int height = m_HeightPortion + *m_TopBarSize; height < *m_Height; height += m_HeightPortion, i++)
	{

		// Formatting the current time
		if(i == 0) String.Format(L"%da", 12);
		else if (i < 12) String.Format(L"%da", i);
		else if (i == 12) String.Format(L"%dp", 12);
		else String.Format(L"%dp", i - 12);
		
		// Moving to correct position to print out time
		// Drawing a thicker line for this separation between all day section and time section
		if (height == m_HeightPortion + *m_TopBarSize)
		{

			CPen* OldFont = pDC->SelectObject(new CPen(PS_SOLID, 3, RGB(82, 95, 120)));
			pDC->MoveTo(0, height);
			pDC->LineTo(*m_Width, height);
			pDC->SelectObject(OldFont);
		}
		pDC->MoveTo(0, height);
		pDC->LineTo(*m_Width, height);
		pDC->TextOutW(0, height - m_HeightPortion + m_AllDayEventSectionSize, String);
	}

	// Drawing the text for the all day event section
	pDC->TextOutW(0, *m_TopBarSize, _T("All Day"));

	// Creating new pen and selecting this object
	aPen.CreatePen(PS_SOLID, 3, RGB(82, 95, 120));
	OldPen = pDC->SelectObject(&aPen);

	// Draws columns based on amount of colomns and current
	// dimensions of client area
	for (int width = m_WidthPortion; width < *m_Width - 10; width += m_WidthPortion)
	{

		pDC->MoveTo(width, *m_TopBarSize + 1);
		pDC->LineTo(width, *m_Height);
	}

	// Restoring the original objects
	pDC->SelectObject(OldPen);
	pDC->SelectObject(OldFont);
}

//
// DrawCells()
// Draws/displays all the current displayed days forming a week from
// the current month, prev month, or next month objects
//
void CViewWeekly::DrawCells(CPlannerView* View, CDC* pDC)
{
	// Local Vars
	CString String;

	// Flushes the current displayed days
	ResetWeek();

	// If this is the last week
	if (m_LastWeek)
	{
		// Reset the day offset arithmetic variable
		m_DayCounter = 0;

		// Counts the days 
		for (int i = m_CurrentMonth->ReturnBegDate(), n = 1; i < 7 && (m_DayCounter < m_CurrentMonth->m_NumberDays); i++, n++)
		{
			if (m_Change) m_DayCounter++;
		}

		// Setting the starting day of the week
		while ((m_WeekStartDay * 7 + m_DayCounter) > m_CurrentMonth->m_NumberDays)
		{
			m_WeekStartDay--;
		}

		// Resetting indicator variable
		m_LastWeek = 0;
	}

	// If this is the first week
	if (m_FirstWeek)
	{
		// Reset the day offset arithmetic variable
		m_DayCounter = 0;

		// Counts the days
		for (int i = m_CurrentMonth->ReturnBegDate(), n = 1; i < 7 && (m_DayCounter < m_CurrentMonth->m_NumberDays); i++, n++)
		{
			if (m_Change) m_DayCounter++;
		}

		// Reset the indicator variable
		m_FirstWeek = 0;
	}

	// If the starting day of the week is Sunday
	if (m_WeekStartDay == 0)
	{
		// Reset the day offset arithmetic variable
		m_DayCounter = 0;

		// Sets the current displayed objects if any for the current month are present
		for (int i = m_CurrentMonth->ReturnBegDate(), n = 1; i < 7 && (m_DayCounter < m_CurrentMonth->m_NumberDays); i++, n++)
		{
			m_Week[m_CurrentMonth->GetList()->ReturnDayWithDate(n)->GetDayOfWeek()] = m_CurrentMonth->GetList()->ReturnDayWithDate(n);
			// Draws this column in the view
			DrawColumn(View, pDC, n, m_CurrentMonth->GetList()->ReturnDayWithDate(n)->GetDayOfWeek(), m_CurrentMonth);
				if(m_Change) m_DayCounter++;	
		}

		// Now getting the previous month to display those days if applicable
		CMonth* prev = m_Planner->ReturnPrevMonth();
		
		// If there is a previous month in this planner object
		if (prev != nullptr)
		{
			// Drawing the days for this previous month
			for (int i = m_CurrentMonth->ReturnBegDate() - 1, n = prev->m_NumberDays; i >= 0; i--, n--)
			{
				m_Week[i] = prev->ReturnDayWithDate(n);
				DrawColumn(View, pDC, n, i, prev);
			}
		}
		else
		{
			m_FirstMonthFirstWeek = 1;

			// Otherwise, setting the days as null since there is not previous month
			for (int i = m_CurrentMonth->ReturnBegDate() - 1; i >= 0; i--)
			{
				m_Week[i] = nullptr;
			}
		}
	}
	else
	{
		// Set the starting week day
		int m_WeekStart = GetDate(m_DayCounter + 1 + (7 * m_WeekStartDay), m_CurrentMonth->ReturnMonthType(), m_Year->ReturnYearDate());
		int Place = 0;

		// Drawing the displayed objects
		for (int i = 0, n = m_DayCounter; i < 7 && (m_DayCounter + (7 * (m_WeekStartDay - 1)) + i < m_CurrentMonth->m_NumberDays); i++, n++)
		{
			m_Week[i] = m_CurrentMonth->ReturnDayWithDate((m_DayCounter + (7 * (m_WeekStartDay - 1)) + i + 1));
			DrawColumn(View, pDC, (m_DayCounter + (7 * (m_WeekStartDay - 1)) + i + 1), i, m_CurrentMonth);

			Place++;
		}
		// Drawing the days for the next month object
		if (m_WeekStartDay * 7 + m_DayCounter + 1 > m_CurrentMonth->m_NumberDays)
		{
			// Getting the next object
			CMonth * Next = m_Planner->ReturnNextMonth();

			if (Next != nullptr)
			{
				// Displaying the months for the next object
				for (int i = Place, n = 0; i < 7; i++, n++)
				{
					m_Week[i] = Next->ReturnDayWithDate(n + 1);
					DrawColumn(View, pDC, n + 1, i, Next);
				}
			}
			else
			{
				// Setting this as a possible last week in this planner object
				m_LastMonthLastWeek = 1;
				// Setting the days as null
				for (int i = Place, n = 0; i < 7; i++, n++)
				{
					m_Week[i] = nullptr;
				}
			}
		}
	}
}

//
// DrawColumn()
// Draws each column individually
// Start day is betwen 0 - 6 for each day of the week
// The start date is the date of each day
void CViewWeekly::DrawColumn(CPlannerView* View, CDC* pDC, int Date, int Day, CMonth* Month)
{
	// Local Variables
	CString Str;
	int CellWidth = Day * m_WidthPortion;
	int j = 0;
	CSize Size;
	int MonthType = Month->ReturnMonthType();
	CDay* Today;
	CFont aFont;
	CFont* defFont;
	CPlannerEvent* Event;

	// Creating the font object
	VERIFY(aFont.CreateFont(
		20,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("Calibri")));           // lpszFacename

	// Saving the previous font object 
	defFont = pDC->SelectObject(&aFont);

	// Getting the day object for this column
	Today = Month->ReturnDayWithDate(Date);

	// So that the background color shows correctly
	pDC->SetBkMode(TRANSPARENT);

	// If today is the selected day and the selected time is for 
	// an allday event, highlight the all day section
	if (Today == m_SelectedDay && m_SelectedTime < 0)
	{
		pDC->FillRect(m_SelectedRect, &CBrush(RGB(100, 100, 100)));
	}

	// Drawing the all day events for this column
	for (int i = 0; (Event = Today->GetAllDayEvent(i)) != nullptr; i++)
	{
		// Formatting the strings for the allday events
		if (*Event->GetPlace() != _T(""))
		{
			Str.Format(L"%s : %s", *Event->GetDescription(), *Event->GetPlace());
		}
		else
		{
			Str.Format(L"%s", *Event->GetDescription());
		}

		// Accounting for the space to display times in the left side of the view
		if (Day == 0)
		{
			// Placing in correct position
			// If the event is out of view
			if (*m_TopBarSize + 15 + 15 * (i + 1) >= *m_TopBarSize + m_HeightPortion)
			{
			}
			// If the event in the first out of view
			else if (*m_TopBarSize + 15 + 15 * (i + 2) >= *m_TopBarSize + m_HeightPortion)
			{
				pDC->TextOutW(CellWidth + 5, *m_TopBarSize + 15 + 15 * i, _T(". . ."));
			}
			// If the event is within view
			else
			{
				pDC->TextOutW(CellWidth + 5, *m_TopBarSize + 15 + 15 * i, Str);
			}
		}
		// Normal operation
		else
		{
			// Placing in correct position
			// If the event is out of view
			if (*m_TopBarSize + 15 * (i + 1) >= *m_TopBarSize + m_HeightPortion)
			{
			}
			// If the event in the first out of view
			else if (*m_TopBarSize + 15 * (i + 2) >= *m_TopBarSize + m_HeightPortion)
			{
				pDC->TextOutW(CellWidth + 5, *m_TopBarSize + 15 * i, _T(". . ."));
			}
			// If the event is within view
			else
			{
				pDC->TextOutW(CellWidth + 5, *m_TopBarSize + 15 * i, Str);
			}
		}
	}

	// Drawing the cells for this weekly view
	for (int CellHeight = *m_TopBarSize + m_AllDayEventSectionSize, n = 0 + m_Offset; CellHeight < *m_Height, (n) < 24; CellHeight += m_HeightPortion, n++)
	{
		// If the time slot for this column in the selected time,
		// highlight this slot
		if (Today == m_SelectedDay && n == m_SelectedTime)
		{
			// Get the rectangle of this slot
			CRect Selected(CPoint(CellWidth, CellHeight),
				CPoint(CellWidth + m_WidthPortion, CellHeight + m_HeightPortion));
			// Fill in this rectangle with higlight color
			pDC->FillRect(Selected, &CBrush(RGB(100, 100, 100)));
		}

		// Drawing the normal events for today
		for (int j = 0; (Event = Today->GetEventForTime(n, j)) != nullptr; j++)
		{
			// Formatting the strings for this event
			if (*Event->GetPlace() != _T(""))
			{
				Str.Format(L"%s : %s @ %s", *Event->GetDescription(), *Event->GetPlace(), Event->GetTimeStandardForm());
			}
			else
			{
				Str.Format(L"%s @ %s", *Event->GetDescription(), Event->GetTimeStandardForm());
			}
			// Accounting for the space to display times in the left side of the view
			if (Day == 0)
			{
				// Placing in correct position
				// If the event is out of view
				if (CellHeight + 15 + 15 * (j + 1) >= CellHeight + m_HeightPortion)
				{
				}
				// If the event in the first out of view
				else if (CellHeight + 15 + 15 * (j + 2) >= CellHeight + m_HeightPortion)
				{
					pDC->TextOutW(CellWidth + 5, CellHeight + 15 + 15 * j, _T(". . ."));
				}
				// If the event is within view
				else
				{
					pDC->TextOutW(CellWidth + 5, CellHeight + 15 + 15 * j, Str);
				}
			}
			// Normal operation
			else
			{
				// Placing in correct position
				// If the event is out of view
				if (CellHeight + 15 * (j + 1) >= CellHeight + m_HeightPortion)
				{
				}
				// If the event in the first out of view
				else if (CellHeight + 15 * (j + 2) >= CellHeight + m_HeightPortion)
				{
					pDC->TextOutW(CellWidth + 5, CellHeight + 15 * j, _T(". . ."));
				}
				// If the event is within view
				else
				{
					pDC->TextOutW(CellWidth + 5, CellHeight + 15 * j, Str);
				}
			}
		}
	}

	// Resetting the font
	pDC->SelectObject(defFont);
	aFont.DeleteObject();
}

//
// MoveRow()
// Handler for the mousewheel, calculates and limits the offset
// of the time to correctly implements the psuedo scrolling.
//
int CViewWeekly::MoveRow(int Movement)
{
	// Moving up one row
	if (Movement > 0)
	{
		if(m_RowCounter > 0) m_RowCounter--;
		if (m_Offset > 0) m_Offset--;
	}
	// Moving down one row
	else
	{
		if(m_RowCounter < 15) m_RowCounter++;
		if (m_Offset < 15) m_Offset++;
	}
	return 0;
}

//
// OnNextMonth()
// Handler for the array toolbar button, month type
// for this view this moves between weeks of the planner
//
int CViewWeekly::OnNextMonth() 
{
	CMonth* PreviousMonth = nullptr;

	// If this is the very last week in this planner object, then do not move to the next week,
	// as there is not valid week to complete this operation
	if (m_CurrentMonth->ReturnMonthType() == 11 && 
		m_Planner->IsLastYear(m_Year->ReturnYearDate()) && 
		m_LastMonthLastWeek);
	else
	{
		PreviousMonth = m_CurrentMonth;
		m_WeekStartDay++;

		if (m_WeekStartDay * 7 >= m_CurrentMonth->m_NumberDays)
		{
			OnNextYear();
			if (m_CurrentMonth->ReturnBegDate() != 0 && PreviousMonth->ReturnDayWithDate(PreviousMonth->m_NumberDays)->GetDayOfWeek() != 6)
			{
				m_WeekStartDay = 0;
				m_FirstWeek = 0;
			}
			else
			{
				m_WeekStartDay = 0;
				m_FirstWeek = 0;
			}
		}

		// Set change as true
		m_Change = 1;
	}
	// Flush leftover indicator variables
	m_FirstMonthFirstWeek = 0;
	m_LastMonthLastWeek = 0;

	return 0;
}

//
// OnPrevMonth
// Handler for the array toolbar button, month type
// for this view this moves between weeks of the planner
//
int CViewWeekly::OnPrevMonth()
{

	// If this is the very first week in this planner object, then do not move to the previous week,
	// as there is not valid week to complete this operation
	if (m_CurrentMonth->ReturnMonthType() == 0 && 
		m_Planner->IsStartYear(m_Year->ReturnYearDate()) && 
		m_FirstMonthFirstWeek);
	else
	{
		m_WeekStartDay--;
		if (m_WeekStartDay < 0)
		{
			OnPrevYear();
			m_LastWeek = 1;
			m_WeekStartDay = m_CurrentMonth->LastWeek();
		}

		// Sets change as true
		m_Change = 1;
	}
	// Flush leftover indicator variables
	m_FirstMonthFirstWeek = 0;
	m_LastMonthLastWeek = 0;

	return 0;
}

//
// OnNextYear()
// Handler for the next year toolbar button
// moves each month for this view
//
int CViewWeekly::OnNextYear()
{
	// Local Vars
	int error = m_Planner->MoveCurrMonth(1);
	// Get the new year, may not always be different
	m_Year = m_Planner->ReturnCurrentYear();
	// Reset varibles and set client area as needing to be redrawn
	m_SizeChanged = 1;
	m_DayCounter = 0;
	m_WeekStartDay = 0;
	m_Change = 1;

	return 1;
}

//
// OnPrevYear
// Handler for the next year toolbar button
// moves each month for this view
//
int CViewWeekly::OnPrevYear()
{
	// Local Vars
	int error = m_Planner->MoveCurrMonth(-1);
	// Get the new year, may not always be different
	m_Year = m_Planner->ReturnCurrentYear();
	// Reset varibles and set client area as needing to be redrawn
	m_SizeChanged = 1;
	m_DayCounter = 0;
	m_WeekStartDay = 0;
	m_Change = 1;

	return 1;
}

//
// GetDate()
// Returns the date for the corresponding day of the year
//
int CViewWeekly::GetDate(int day, int month, int year)
{
	static int t[] = { 0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4 };
	year -= month < 3;
	return (year + year / 4 - year / 100 + year / 400 + t[month - 1] + day) % 7;
}

//
// SetObject()
// Sets the selected object in the view
//
bool CViewWeekly::SetObject(CPoint Point, CDay** DayObject)
{
	// Local Variables
	CRect EnclosingRect;
	CString string;
	int Row = 0;
	int column = 0;

	// Finding the appropriate object to select
	for (int i = 0; i < 7; i++)
	{
		if (Point.x > i * m_WidthPortion
			&& Point.x < (i + 1) * m_WidthPortion &&
			Point.y > *m_TopBarSize && Point.y < m_AllDayEventSectionSize + *m_TopBarSize && m_Week[i] != nullptr)
		{
			// Setting the selected object
			m_SelectedDay = m_Week[i];
			*DayObject = m_Week[i];

			// Calculating the column
			column = ((Point.x) / m_WidthPortion);

			// Setting the selected time for this Day
			m_SelectedTime = -1;
			// Setting the selected Rectangle for this day
			m_SelectedRect = new CRect(CPoint(column * m_WidthPortion, *m_TopBarSize), CPoint((column + 1) * m_WidthPortion, *m_TopBarSize + m_AllDayEventSectionSize));

			return 1;
		}

		// Checking the coordinates
		else if (Point.x > i * m_WidthPortion 
			&& Point.x < (i + 1) * m_WidthPortion && 
			Point.y > *m_TopBarSize - m_AllDayEventSectionSize && m_Week[i] != nullptr)
		{
			// Setting the selected object
			m_SelectedDay = m_Week[i];
			*DayObject = m_Week[i];

			// Calculating for offset to correctly implement scrolling
			Row = ((Point.y - *m_TopBarSize - m_AllDayEventSectionSize) / m_HeightPortion) + m_Offset;

			// Calculating the column
			column = ((Point.x) / m_WidthPortion);
			
			// Setting the selected time for this Day
			m_SelectedTime = Row;
			// Setting the selected Rectangle for this day
			m_SelectedRect = new CRect(CPoint(column * m_WidthPortion, (Row) * m_HeightPortion), CPoint((column+1) * m_WidthPortion, (Row+3) * m_HeightPortion));

			return 1;
		}
	}
	return 0;
}

//
// ResetWeek()
// Resets the m_Week array for this planner object
//
void CViewWeekly::ResetWeek()
{
	for (int i = 0; i < 7; i++)
	{
		m_Week[i] = nullptr;
	}
}

//
// SetDialogData()
// For this view, this function sets the time for the dialog to 
// the time of the selected time for the selected day of the month
//
void CViewWeekly::SetDialogData(CDialogAddEvent *aDlg)
{
	// Sends the relevant data to the dialog object

	// IF the time is invalid, so this is an allday event
	if (m_SelectedTime < 0);
	else
	{
		aDlg->SetBegTime(m_SelectedTime, 0);
		if (m_SelectedTime != 23) aDlg->SetEndTime(m_SelectedTime + 1, 0);
		else aDlg->SetEndTime(m_SelectedTime, 0);
	}
}

//
// HandleContextMenu()
// Handles and returns the approp. context menu
//
CMenu* CViewWeekly::HandleContextMenu(CWnd* pWnd, CPoint Point)
{
	// Local Vars
	CMenu Menu;

	// Get the cursor position
	GetCursorPos(&m_CursorPosition);
	// Converting screen coordinates to client coordinates
	ScreenToClient(m_CurrentView->m_hWnd, &m_CursorPosition);


	// Finding the appropriate object to select
	for (int i = 0; i < 7; i++)
	{
		if (m_CursorPosition.x > i * m_WidthPortion
			&& m_CursorPosition.x < (i + 1) * m_WidthPortion &&
			m_CursorPosition.y > *m_TopBarSize && m_CursorPosition.y < m_AllDayEventSectionSize + *m_TopBarSize && m_Week[i] != nullptr)
		{
			// Setting the selected object
			m_ContextMenuSelectedDay = m_Week[i];

			// Calculating for offset to correctly implement scrolling
			m_ContextMenuSelectedTime = -1;

			// Load the desired context menu
			Menu.LoadMenu(ID_MENU_EVENT);
			// Loading the appropriate submenu for this section
			CMenu* pContext = Menu.GetSubMenu(3);
			// Verify the menu is there
			ASSERT(pContext != nullptr);
			// Make the menu visible
			pContext->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,
				Point.x, Point.y, pWnd);

			return &Menu;
		}

		// Checking the coordinates
		else if (m_CursorPosition.x > i * m_WidthPortion
			&& m_CursorPosition.x < (i + 1) * m_WidthPortion &&
			m_CursorPosition.y > *m_TopBarSize - m_AllDayEventSectionSize && m_Week[i] != nullptr)
		{
			// Setting the selected object
			m_ContextMenuSelectedDay = m_Week[i];

			// Calculating for offset to correctly implement scrolling
			m_ContextMenuSelectedTime = ((m_CursorPosition.y - *m_TopBarSize - m_AllDayEventSectionSize) / m_HeightPortion) + m_Offset;

			// Load the desired context menu
			Menu.LoadMenu(ID_MENU_EVENT);
			// Loading the appropriate submenu for this section
			CMenu* pContext = Menu.GetSubMenu(4);
			// Verify the menu is there
			ASSERT(pContext != nullptr);
			// Make the menu visible
			pContext->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,
				Point.x, Point.y, pWnd);

			return &Menu;

		}
	}

	return nullptr;
}

//
// OnClearHour()
// Clears the hour where the cursor is currently located.
//
void CViewWeekly::OnClearHour()
{
	if (m_ContextMenuSelectedDay != nullptr)
	{
		// All Day event
		if (m_ContextMenuSelectedTime == -1)
		{
			m_ContextMenuSelectedDay->DeleteEventList(true);
		}
		else
		{
			m_ContextMenuSelectedDay->DeleteEventList(false, m_ContextMenuSelectedTime);
		}
	}
}

//
// OnClearDay()
// Clears the day where the cursor is currently located.
//
void CViewWeekly::OnClearDay()
{

	if (m_ContextMenuSelectedDay != nullptr)
	{
		m_ContextMenuSelectedDay->ClearEntireDay();
	}

}

//
// OnMarkNextAsCompleted()
// Marks the first uncompleted event as completed
//
void CViewWeekly::OnMarkNextAsCompleted()
{

	if (m_ContextMenuSelectedDay != nullptr)
	{
		// All Day event
		if (m_ContextMenuSelectedTime == -1)
		{
			m_ContextMenuSelectedDay->MarkNextEventAsCompleted(true);
		}
		else
		{
			m_ContextMenuSelectedDay->MarkNextEventAsCompleted(false, m_ContextMenuSelectedTime);
		}
	}

}


//
// OnMarkNextAsCompleted()
// Marks the first uncompleted event as completed
//
void CViewWeekly::OnRemoveFirstEvent()
{

	if (m_ContextMenuSelectedDay != nullptr)
	{
		// All Day event
		if (m_ContextMenuSelectedTime == -1)
		{
			m_ContextMenuSelectedDay->DeleteXthElement(true, 0);
		}
		else
		{
			m_ContextMenuSelectedDay->DeleteXthElement(false, 0, m_ContextMenuSelectedTime);
		}
	}

}

//
// OnGoToDay()
// Returns the currently selected day from HandleContextMenu()
//
CDay* CViewWeekly::OnGoToDay()
{
	
	return m_ContextMenuSelectedDay;
}
