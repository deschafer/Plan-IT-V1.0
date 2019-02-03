#include "stdafx.h"
#include "ViewBase.h"

#include "..\Dialog\DialogAddEvent.h"

// CTOR for a CViewDaily subview object
CViewDaily::CViewDaily(int *Width, int *Height, int * TopBarPortion, CPlannerObject* Object) :
	CViewBase(0, 0, Height, Width, TopBarPortion, Object), m_PreviousMonth(0), m_TimeOffset(0),
	m_CenterSectionWidth(35), m_AllDayOffset(0), m_LeftFontActualSize(28), m_SelectedTime(-1),
	m_AllDayEventAddedPrevious(0), m_DisplayedStrings(0), m_MaxNumberOfDisplayedStrings(0), m_CurrDay(nullptr),
	m_LeftSideStringSpace(28), m_SetExternal(0), m_ContextSelectedTime(0), m_DraggedEventInitialPosition(0),
	m_AddEventButtonSelected(false), m_DraggingEvent(false)
{
	m_Rows = new unsigned;
	m_Columns =  new unsigned;

	*m_Rows = 8;
	*m_Columns = 2;
}

//
// InitilizeWndVariables()
// Override that adds some initialization procedures for 
// variables unique to this view
//
void CViewDaily::InitilizeWndVariables(CPlannerView* View)
{
	// Calling original function
	CViewBase::InitilizeWndVariables(View);

	// Setting the number of rows
	*m_Rows = 8;

	m_HeightPortion = (*m_Height - *m_TopBarSize) / *m_Rows;
	// Setting the current view
	m_CurrentView = View;
	// Setting the maximum number of displayed strings for the left section
	m_MaxNumberOfDisplayedStrings = (*m_Height - *m_TopBarSize) / m_LeftFontActualSize;
}



//
// DrawLayout()
// Draws the entire user layout for this Daily view
//
void CViewDaily::DrawLayout(CDC* pDC, CPlannerView* View)
{

	// Initialize variables if not already
	if (m_Width == 0 || m_Height == 0 || m_SizeChanged)
	{
		InitilizeWndVariables(View);
	}

	// Get the current day object
	SetDay();

	// Get the cursor position
	GetCursorPos(&m_CursorPosition);

	ScreenToClient(View->m_hWnd, &m_CursorPosition);

	// Drawing a default background
	FillBackground(pDC, View);

	// Drawing the month section
	DrawMonthSection(pDC, View);

	// Drawing the topbar section
	DrawTopBarSection(pDC, View);

	// Drawing the basicc grid layout
	DrawGrid(pDC, View);

}

// Sets the current day object
void CViewDaily::SetDay()
{

	// Getting today's time information
	time_t Time = time(NULL);
	tm* p_Time = localtime(&Time);

	if (!m_SetExternal)
	{


		// If the month has been changed, change the current day object
		if (m_PreviousMonth != m_CurrentMonth->ReturnMonthType())
		{

			// If this month, then set the current day to today's date
			if (m_CurrentMonth->ReturnMonthType() == p_Time->tm_mon &&
				m_CurrentMonth->ReturnDayWithDate(1)->GetYear() == m_Year->ReturnYearDate())
			{
				// Set the day object
				m_CurrDay = m_CurrentMonth->ReturnDayWithDate(p_Time->tm_mday);
			}

			else
			{
				// Otherwise set the date as the first of the month
				m_CurrDay = m_CurrentMonth->ReturnDayWithDate(1);
			}
		}

	}

	// Storing this month type
	m_PreviousMonth = m_CurrentMonth->ReturnMonthType();

	if (m_CurrDay == nullptr)
	{

		m_CurrDay = m_CurrentMonth->ReturnDayWithDate(p_Time->tm_mday);

	}


	//Calculate the current displayed events on the screen
	m_DisplayedStrings = m_CurrDay->GetNumberOfAllDayEvents() - m_AllDayOffset + 2;

	m_SetExternal = 0;
}

//
// DrawGrid()
// Draws the basic grid elemments of the UI for this daily view
//
void CViewDaily::DrawGrid(CDC* pDC, CPlannerView* View)
{
	// Local Variables
	CPen aPen(PS_SOLID, 3, RGB(30, 40, 50));
	CPen *OldPen = nullptr;

	// Selecting the new pen
	OldPen = pDC->SelectObject(&aPen);

	// Drawing main dividing line
	pDC->MoveTo(*m_Width / 2, *m_TopBarSize + 1);
	pDC->LineTo(*m_Width / 2, *m_Height);

	// Restoring the pen object
	pDC->SelectObject(OldPen);

	// Drawing the right section of this view
	DrawRightSection(pDC, View, *m_TopBarSize, *m_Height, m_WidthPortion, *m_Width);

	// Drawing the center section for this view
	DrawCenterSection(pDC, View, *m_TopBarSize, *m_Height, m_WidthPortion - m_CenterSectionWidth, m_WidthPortion);
	
	// Drawing the left section for this view
	DrawLeftSection(pDC, View, *m_TopBarSize, *m_Height, 0, m_WidthPortion - m_CenterSectionWidth);

}

//
// DrawLeftSection()
// Draws the left section to display the misc/all day
// events for this view.
//
void CViewDaily::DrawLeftSection(CDC* pDC, CPlannerView* View, int HeightMin, int HeightMax, int WidthMin, int WidthMax)
{
	// Local Variables
	CString String;
	CFont Font;
	CFont *OldFont = nullptr;
	CPlannerEvent* CurrEvent = nullptr;
	int BegIndex = 0 + m_TimeOffset;
	int TotalHeight = HeightMax - HeightMin;
	int TotalTextHeight = 0;
	int i = 0, j = 0;
	int Offset = 10;

	// Create the font
	VERIFY(Font.CreateFont(
		32,                  // nHeight
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

	// Selecting the new font
	OldFont = pDC->SelectObject(&Font);

	// Adding the offset for scrolling
	i += m_AllDayOffset;

	// Printing out the events
	for (j = 0; (CurrEvent = m_CurrDay->GetAllDayEvent(i)) != nullptr; i++, j++)
	{
		// Formatting the string to display
		if (i + 1 < 10)
		{
			if (*CurrEvent->GetPlace() == _T("")) String.Format(L"%d.     %s", i + 1, *CurrEvent->GetDescription()); 
			else String.Format(L"%d.     %s : %s", i + 1, *CurrEvent->GetDescription(), *CurrEvent->GetPlace());
		}
		else
		{
			if (*CurrEvent->GetPlace() == _T("")) String.Format(L"%d.     %s", i + 1, *CurrEvent->GetDescription());
			else String.Format(L"%d.     %s : %s", i + 1, *CurrEvent->GetDescription(), *CurrEvent->GetPlace());
		}
		
		if (CurrEvent->IsCompleted())
		{
			CString String2 = _T(" -- Completed");
			String = String + String2;
			pDC->TextOutW(WidthMin + 5, HeightMin + (j * m_LeftSideStringSpace) + 5, String);
		}
		else
		{
			// Printing out the string
			pDC->TextOutW(WidthMin + 5, HeightMin + (j * m_LeftSideStringSpace) + 5, String);
		}
	}

	// New Event button
	// Drawing the inside of the rectangle
	if (m_AddEventButtonSelected)
	{
		pDC->FillRect(CRect(CPoint(WidthMin + 2, HeightMin + (j * m_LeftSideStringSpace) + Offset),
			CPoint(pDC->GetTextExtent(_T("+ Add New Event")).cx + Offset + 5, HeightMin + ((j + 1) * m_LeftSideStringSpace) + Offset + 5)),
			&CBrush(RGB(165, 176, 206)));
	}
	else
	{
		pDC->FillRect(CRect(CPoint(WidthMin + 2, HeightMin + (j * m_LeftSideStringSpace) + Offset),
			CPoint(pDC->GetTextExtent(_T("+ Add New Event")).cx + Offset + 5, HeightMin + ((j + 1) * m_LeftSideStringSpace) + Offset + 5)),
			&CBrush(RGB(145, 156, 186)));
	}
	
	// Drawing the outline of the rectangle
	pDC->FrameRect(CRect(CPoint(WidthMin + 2, HeightMin + (j * m_LeftSideStringSpace) + Offset),
		CPoint(pDC->GetTextExtent(_T("+ Add New Event")).cx + Offset + 5, HeightMin + ((j + 1) * m_LeftSideStringSpace) + Offset + 5)),
		&CBrush(RGB(0, 0, 0)));
	// Drawing the text of the button
	pDC->TextOutW(WidthMin + 5, HeightMin + (j * m_LeftSideStringSpace) + Offset, _T("+ Add New Event"));

	m_EventButtonWidthMax = pDC->GetTextExtent(_T("+ Add New Event")).cx + Offset + 5;
	m_EventButtonWidthMin = WidthMin + 2;

	// Resetting the font
	pDC->SelectObject(OldFont);
	Font.DeleteObject();
}


//
// DrawCenterSection()
// Draws the time slot section, center section, for 
// this view. Sets the appropriate time for the
// time slots so the scrolling will work properly.
//
void CViewDaily::DrawCenterSection(CDC* pDC, CPlannerView* View, int HeightMin, int HeightMax, int WidthMin, int WidthMax)
{
	// Local Variables
	CString String;
	CPen aPen(PS_SOLID, 3, RGB(30, 40, 50));
	CPen *OldPen = nullptr;
	int HeightPortion = (HeightMax - HeightMin) / *m_Rows;
	CFont font;
	CFont* OldFont = nullptr;
	int FontCenteringWidth = 3;
	int FontCenterWidthCorrection = 0;
	CRect CenterSectionBackground(WidthMin, HeightMin + 1, WidthMax, HeightMax + 1);
	CBrush *NewBrush = new CBrush(RGB(150, 150, 150));

	// Filling the background color for this section
	pDC->FillRect(CenterSectionBackground, NewBrush);

	// Create the font
	VERIFY(font.CreateFont(
		25,                  // nHeight
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

	// Selecting the new pen and font
	OldPen = pDC->SelectObject(&aPen);
	OldFont = pDC->SelectObject(&font);

	// Setting the correct starting time index
	int BegIndex = 0 + m_TimeOffset;

	// Drawing the surrounding gridlines
	pDC->MoveTo(WidthMin, HeightMin + 1);
	pDC->LineTo(WidthMin, HeightMax);

	// Selecting the default pen
	pDC->SelectObject(OldPen);

	// Drawing the time horizontal gridlines
	for (int i = 0; i <= *m_Rows; i++)
	{
		// Drawing the horizontal lines
		pDC->MoveTo(WidthMin, HeightMin + (HeightPortion * i));
		pDC->LineTo(WidthMax, HeightMin + (HeightPortion * i));
	}

	// Drawing the time slot text
	for (int i = 0; i < *m_Rows; i++)
	{
		// Resets the correction 
		FontCenterWidthCorrection = 0;

		// Formatting the string to display
		if ((BegIndex + i) == 0) String.Format(L"%da", 12);
		else if((BegIndex + i) < 12) String.Format(L"%da", BegIndex + i);
		else if ((BegIndex + i) == 12) String.Format(L"%dp", 12);
		else String.Format(L"%dp", (BegIndex + i) % 12);

		// Adjusting the position of the text to center it properly
		if (String.GetLength() == 2)
		{
			FontCenterWidthCorrection = 4;
		}
		// Displaying the text
		pDC->TextOutW(WidthMin + FontCenteringWidth + FontCenterWidthCorrection, HeightMin + (HeightPortion * (i) + (HeightPortion / 2)) - (HeightPortion / 10), String);
	}

	// Resetting the font
	pDC->SelectObject(OldFont);
	delete NewBrush;
}


//
// DrawRightSection()
// Draws the hourly section of this view, called from DrawGrid().
// By having this in its own function, the coordinates can be simplified further,
// making it easier to maintain and read.
//
void CViewDaily::DrawRightSection(CDC* pDC, CPlannerView* View, int HeightMin, int HeightMax, int WidthMin, int WidthMax)
{
	// Local Variables
	int HeightPortion = (HeightMax - HeightMin) / *m_Rows;
	int BegIndex = 0 + m_TimeOffset;
	CPlannerEvent* CurrEvent;
	CString Str;
	CFont font;
	CFont* OldFont = nullptr;
	CRect RightSectionBackground(WidthMin + 3, HeightMin + 1, WidthMax, HeightMax);
	CBrush* NewBrush = new CBrush(RGB(221, 221, 235));
	CBrush HighLightBrush(RGB(150, 150, 160));
	CRect EnclosingRect;
	int CellHeight = 0;
	int CellWidth = 0;

	pDC->FillRect(RightSectionBackground, NewBrush);

	// Create the font
	VERIFY(font.CreateFont(
		20,                  // nHeight
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

	// Selects the new font
	OldFont = pDC->SelectObject(&font);

	// Drawing the events for each section
	for (int i = BegIndex, k = 0; i < 24; i++, k++)
	{
		// Drawing gridines
		pDC->MoveTo(WidthMin, HeightMin + (HeightPortion * (k + 1)));
		pDC->LineTo(WidthMax, HeightMin + (HeightPortion * (k + 1)));

		// Gets the rectangle for this section
		EnclosingRect.SetRect(CPoint(WidthMin, HeightMin + (HeightPortion * k) + 1), CPoint(WidthMax, HeightMin + (HeightPortion * (k + 1))));

		EnclosingRect.DeflateRect(2, 0);

		// Fills in the background
		if (i == m_SelectedTime)
		{
			pDC->FillRect(EnclosingRect, &HighLightBrush);
		}
		else
		{
			pDC->FillRect(EnclosingRect, NewBrush);
		}

		CellHeight = k + m_HeightPortion + HeightMin;
		CellWidth = WidthMin;

		for (int j = 0; (CurrEvent = m_CurrDay->GetEventForTime(i, j)) != nullptr; j++)
		{
			// Formatting the strings for this event
			if (*CurrEvent->GetPlace() != _T(""))
			{
				Str.Format(L"%s : %s @ %s", *CurrEvent->GetDescription(), *CurrEvent->GetPlace(), CurrEvent->GetTimeStandardForm());
			}
			else
			{
				Str.Format(L"%s @ %s", *CurrEvent->GetDescription(), CurrEvent->GetTimeStandardForm());
			}

			// Placing in correct position
			if (((18 * (j)) + 5) >= m_HeightPortion)
			{
			}
			// If the event in the first out of view
			else if ((18 * (j + 2)) >= m_HeightPortion)
			{

					pDC->TextOutW(WidthMin + 5, HeightMin + (HeightPortion * (i - BegIndex) + (18 * j)) - 3, _T("..."));
			}
			// If the event is within view
			else
			{
				if (CurrEvent->IsCompleted()) pDC->TextOutW(WidthMin + 5, HeightMin + (HeightPortion * (i - BegIndex) + (18 * j) + 5), _T("Completed"));
				else
					pDC->TextOutW(WidthMin + 5, HeightMin + (HeightPortion * (i - BegIndex) + (18 * j) + 5), Str);
			}

		}
	}

	// Freeing memory and selecting old objects
	pDC->SelectObject(OldFont);
	delete NewBrush;
}

//
// DrawTopBarSection()
// Draws the unique top bar for this daily view
//
void CViewDaily::DrawTopBarSection(CDC* pDC, CPlannerView* View)
{

	// Local Variables
	int FontSize = 25;
	CFont font;
	CRect EnclosingRect(CPoint(0, 37), CPoint(*m_Width, *m_TopBarSize));
	CBrush NewBrush;
	CString String;

	// Set the background mode as transparent so text will
	// appear accordingly
	pDC->SetBkMode(TRANSPARENT);

	// Drawing the top bar section
	pDC->MoveTo(0, *m_TopBarSize);
	pDC->LineTo(*m_Width, *m_TopBarSize);

	// Create the font
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

								   // Save the default font, and set the new font
	CFont* def_font = pDC->SelectObject(&font);

	// Create the new brush
	NewBrush.CreateSolidBrush(RGB(140, 120, 120));

	// Set the color of the text 
	pDC->SetTextColor(RGB(50, 50, 50));

	// Color in the rectangle for this section of the topbar
	pDC->FillRect(EnclosingRect, &NewBrush);

	// Formatting the String for the first section
	String.Format(L"Tasks for %d/%d", m_CurrentMonth->ReturnMonthType() + 1, m_CurrDay->GetNumber());
	pDC->TextOutW(m_WidthPortion * 0 + (m_WidthPortion / 100), 37, String);

	// Reformatting the String for the second section
	String.Format(L"Hourly Events");
	pDC->TextOutW(m_WidthPortion * 1 + (m_WidthPortion / 100), 37, String);

	// Selecting the old object
	pDC->SelectObject(def_font);

	// Deleting objects
	font.DeleteObject();
	NewBrush.DeleteObject();

}


//
//
//
//
int CViewDaily::OnNextMonth()
{

	if (!(m_CurrentMonth->m_NumberDays < m_CurrDay->GetNumber() + 1))
	{

		m_CurrDay = m_CurrentMonth->ReturnDayWithDate(m_CurrDay->GetNumber() + 1);

	}

	return 0;
}

//
//
//
//
int CViewDaily::OnPrevMonth()
{
	// If the previous day is within this month's range
	if (!(0 >= m_CurrDay->GetNumber() - 1))
	{
		m_CurrDay = m_CurrentMonth->ReturnDayWithDate(m_CurrDay->GetNumber() - 1);
	}
	else
	{
		// Otherwise, get the previous month and...
		m_Planner->MoveCurrMonth(-1);
		// Set the new day pointer

		m_Year = m_Planner->ReturnCurrentYear();

		//m_CurrDay = m_Planner->ReturnCurrentMonth()->ReturnDayWithDate(m_Planner->ReturnCurrentMonth()->m_NumberDays);
	}
	m_SizeChanged = 1;
	return 0;
}

//
// OnNextYear()
// Message handler for the next year toolbar button --
// Moves to the next month in this planner object
//
int CViewDaily::OnNextYear()
{
	// Moving the current month
	m_Planner->MoveCurrMonth(1);
	
	// Getting the new current year if there was a change
	m_Year = m_Planner->ReturnCurrentYear();

	m_SizeChanged = 1;

	return 1;
}


//
// OnPrevYear()
// Message handler for the prev year button -- 
// Moves to the previous month in this planner object
//
int CViewDaily::OnPrevYear()
{
	// Moving the current month
	m_Planner->MoveCurrMonth(-1);

	// Getting the new current year if there was a change
	m_Year = m_Planner->ReturnCurrentYear();

	m_SizeChanged = 1;

	return 1;
}

//
// MoveRow()
// Handler for the mouse scroll wheel, will move the
// current displayed time slots in this view to 
// implement scrolling.
//
int CViewDaily::MoveRow(int Movement)
{
	// Get the cursor position
	GetCursorPos(&m_CursorPosition);
	// Converting screen coordinates to client coordinates
	ScreenToClient(m_CurrentView->m_hWnd, &m_CursorPosition);

	// Moving rows for the time slots/ right side
	// of this view, so if the cursor is on that side,
	// then scroll that side
	if (m_CursorPosition.x > (m_WidthPortion - m_CenterSectionWidth))
	{
		// Moving up one row
		if (Movement > 30)
		{
			if (m_TimeOffset > 0) m_TimeOffset--;
		}
		// Moving down one row
		else
		{
			if (m_TimeOffset + *m_Rows < 24) m_TimeOffset++;
		}
		return 0;
	}
	// Otherwise move the left side
	else
	{
		// Moving up one row
		if (Movement > 0)
		{
			if (m_AllDayOffset > 0)
			{
				m_AllDayOffset--;
			}	
		}
		// Moving down one row
		else
		{
			// Verifying that the amount of displayed strings is larger than the total of displayed strings
			// that can be displayed. This allows prevents scrolling past the last event
			if ((m_DisplayedStrings) > m_MaxNumberOfDisplayedStrings)
			{
				m_AllDayOffset++;
			}
		}
		return 0;
	}
}

//
// SetObject()
// Finds the current day object under the selected point
// for this view.
//
bool CViewDaily::SetObject(CPoint Point, CDay **DayObject)
{
	m_DraggingEvent = false;

	// If the point is in the left side
	if (Point.y > *m_TopBarSize &&
		Point.x < (m_WidthPortion - m_CenterSectionWidth))
	{
		// Add event button
		if ((Point.y >= ((m_CurrDay->GetNumberOfAllDayEvents() - m_AllDayOffset) * m_LeftSideStringSpace) + *m_TopBarSize + 10 &&
			Point.y <= (((m_CurrDay->GetNumberOfAllDayEvents() - m_AllDayOffset) + 1) * m_LeftSideStringSpace) + *m_TopBarSize + 15) &&
			Point.x >= m_EventButtonWidthMin && Point.x <= m_EventButtonWidthMax || m_AllDayEventAddedPrevious)
		{
			// Setting the day object ptr
			*DayObject = m_CurrDay;

			// Indicates that time is not applicable
			m_SelectedTime = -1;

			// Resets indicator
			m_AllDayEventAddedPrevious = 0;

			if ((m_DisplayedStrings) > m_MaxNumberOfDisplayedStrings) m_AllDayOffset++;

			// Indicates a day was found
			return true;
		}
		CPlannerEvent* Event;
		//if ((Event = m_CurrDay->GetAllDayEvent((Point.y - *m_TopBarSize) / m_LeftFontActualSize)) != nullptr) Event->MarkCompleted();
		m_SizeChanged = 1;
	}
	// If the point is on the right side
	else if (Point.y > *m_TopBarSize &&
		Point.x >= (m_WidthPortion - m_CenterSectionWidth))
	{
		// Getting the current selected time by calculating the position
		// of the cursor
		m_SelectedTime = (Point.y - *m_TopBarSize) /
			(((*m_Height - *m_TopBarSize) / *m_Rows)) +
			m_TimeOffset;

		// Setting the day object ptr
		*DayObject = m_CurrDay;
		
		// Indicates a day was found
		return true;

	}
	// If the point is above the topbar area
	else
	{
	}

	// No day was found
	return false;
}

//
// SetDialogData()
// Sets the time of the dialog to current selected time
//
void CViewDaily::SetDialogData(CDialogAddEvent *aDlg) 
{
	if (m_SelectedTime < 0);
	else
	{
		aDlg->SetBegTime(m_SelectedTime, 0);
		if (m_SelectedTime != 23) aDlg->SetEndTime(m_SelectedTime + 1, 0);
		else aDlg->SetEndTime(m_SelectedTime, 0);
	}
}

//
// HandleKeyboardMsg()
// Handles the various keyboard messages and takes the appropriate
// action for this view
//
int CViewDaily::HandleKeyboardMsg(UINT nChar)
{
	// Handles the space button specefically for this view
	if (nChar == VK_SPACE)
	{
		// Sets indicator to show that the previous event added as an all day event,
		// this is required because of the select button to create a new event for
		// the left section.
		m_AllDayEventAddedPrevious = true;
		m_SizeChanged = 1;
		return 5;
	}
	else
	{
		return CViewBase::HandleKeyboardMsg(nChar);
	}

}

//
// HandleContextMenu()
// Handles and returns the approp. context menu
//
CMenu* CViewDaily::HandleContextMenu(CWnd* pWnd, CPoint Point)
{
	// Local Variables
	CMenu Menu;

	// Get the cursor position
	GetCursorPos(&m_CursorPosition);
	// Converting screen coordinates to client coordinates
	ScreenToClient(m_CurrentView->m_hWnd, &m_CursorPosition);

	// If the point is in the left side
	if (m_CursorPosition.y > *m_TopBarSize &&
		m_CursorPosition.x < (m_WidthPortion - m_CenterSectionWidth))
	{
		// If the cursor is over an element
		if (m_CurrDay->GetAllDayEvent((m_CursorPosition.y - *m_TopBarSize - 10) / m_LeftFontActualSize ))
		{

			// Load the desired context menu
			Menu.LoadMenu(ID_MENU_EVENT);
			// Loading the appropriate submenu for this section
			CMenu* pContext = Menu.GetSubMenu(2);
			// Verify the menu is there
			ASSERT(pContext != nullptr);
			// Make the menu visible
			pContext->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,
				Point.x, Point.y, pWnd);
		}
		else
		{
			// Load the desired context menu
			Menu.LoadMenu(ID_MENU_EVENT);
			// Loading the appropriate submenu for this section
			CMenu* pContext = Menu.GetSubMenu(0);
			// Verify the menu is there
			ASSERT(pContext != nullptr);
			// Make the menu visible
			pContext->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,
				Point.x, Point.y, pWnd);
		}
		m_ContextSelectedTime = 1;
		// Return the new menu
		return &Menu;
	}
	// If the point is on the right side
	else if (m_CursorPosition.y > *m_TopBarSize &&
		m_CursorPosition.x >= (m_WidthPortion - m_CenterSectionWidth))
	{
		// Load the desired context menu
		Menu.LoadMenu(ID_MENU_EVENT);
		// Loading the appropriate submenu for this section
		CMenu* pContext = Menu.GetSubMenu(1);
		// Verify the menu is there
		ASSERT(pContext != nullptr);
		// Make the menu visible
		pContext->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,
			m_CursorPosition.x, m_CursorPosition.y, pWnd);

		m_ContextSelectedTime = 0;

		// Return the new menu
		return &Menu;
	}
	// If the point is above the topbar area
	else	
	{
	}

	// Return no menu
	return nullptr;
}

//
// OnDeleteEvent()
// Delete the event under the cursor for the all day events
//
void CViewDaily::OnClearDay()
{

	m_CurrDay->ClearEntireDay();
}

//
// OnDeleteEvent()
// Delete the event under the cursor for the all day events
//
void CViewDaily::OnDeleteEvent()
{

	CString String;
	String.Format(L"%d", (m_CursorPosition.y - *m_TopBarSize - 5) / m_LeftFontActualSize + m_AllDayOffset);
	//AfxMessageBox(String);

	// Deleting the appropriate event
	m_CurrDay->DeleteXthElement(true, (m_CursorPosition.y - *m_TopBarSize - 5) / m_LeftFontActualSize + m_AllDayOffset);

}

//
// OnMarkNextAsCompleted()
// Delete the event under the cursor for the all day events
//
void CViewDaily::OnDayMarkAsCompleted()
{

	CString String;
	String.Format(L"%d", (m_CursorPosition.y - *m_TopBarSize - 5) / m_LeftFontActualSize + m_AllDayOffset);
	//AfxMessageBox(String);

	// Marking appropriate event as completed
	m_CurrDay->MarkEventAsCompleted(true, (m_CursorPosition.y - *m_TopBarSize - 5) / m_LeftFontActualSize + m_AllDayOffset);
}

//
// OnRemoveFirstEvent()
// Removes the first event in a specefied list of events
//
void CViewDaily::OnRemoveFirstEvent()
{

	CString String;
	String.Format(L"%d", m_TimeOffset + (m_CursorPosition.y - *m_TopBarSize) / m_HeightPortion);
	//AfxMessageBox(String);

	// Marking appropriate event as completed
	m_CurrDay->DeleteXthElement(false, 0, m_TimeOffset + (m_CursorPosition.y - *m_TopBarSize) / m_HeightPortion);

}

//
// OnClearHour()
// Deletes the list of events at the time under the cursor
//
void CViewDaily::OnClearHour()
{
	if (m_ContextSelectedTime == 1)
	{
		m_CurrDay->DeleteEventList(true);

	}
	else
	{
		// Deleting the hour under the cursor
		m_CurrDay->DeleteEventList(false, m_TimeOffset + (m_CursorPosition.y - *m_TopBarSize) / m_HeightPortion);
	}
}

//
// OnMarkNextAsCompleted()
// Marks the first uncompleted event as completed for
// the specefied time slot under the cursor
//
void CViewDaily::OnMarkNextAsCompleted()
{
	// Deleting the hour under the cursor
	m_CurrDay->MarkNextEventAsCompleted(false, m_TimeOffset + (m_CursorPosition.y - *m_TopBarSize) / m_HeightPortion);

}

//
// HandleMouseDrag()
// Sets current dragged event following the current
// mouse cursor position
//
int CViewDaily::HandleMouseDrag(CPoint Point)
{
	CPlannerEvent* Event = nullptr;

	// If the point is in the left side
	if (Point.y > *m_TopBarSize &&
		Point.x < (m_WidthPortion - m_CenterSectionWidth))
	{
		// If the cursor is over an element
		if (Event = (m_CurrDay->GetAllDayEvent(m_AllDayOffset + (Point.y - *m_TopBarSize - 10) / m_LeftFontActualSize)))
		{
			// Sets the event that has been selected
			m_DraggingEvent = true;
			m_DraggedEvent = Event;
			m_DraggedEventInitialPosition = m_AllDayOffset + (Point.y - *m_TopBarSize - 10) / m_LeftFontActualSize;
			return 1;
		}

	}

	return 0;
}

//
// HandleMouseMove()
// Handles mouse movement for the dragging motion
// Records mouse movement, and takes appropriate action
// in the LL to represent the changes
//
void CViewDaily::HandleMouseMove(CPoint Point)
{
	if (m_AddEventButtonSelected == false && m_DraggedEvent != nullptr && m_DraggingEvent)
	{
		if (((Point.y - *m_TopBarSize - 10) / m_LeftFontActualSize + m_AllDayOffset) < m_DraggedEventInitialPosition)
		{
			m_CurrDay->SwapEvent(m_DraggedEvent, true, true);
			m_DraggedEventInitialPosition--;
			m_CurrentView->InvalidateRect(nullptr);
		}

		else if ((((Point.y - *m_TopBarSize - 10) / m_LeftFontActualSize + m_AllDayOffset) > m_DraggedEventInitialPosition))
		{
			m_CurrDay->SwapEvent(m_DraggedEvent, true, false);
			m_DraggedEventInitialPosition++;
			m_CurrentView->InvalidateRect(nullptr);
		}
	}

	// button selection add event button
	if ((Point.y >= ((m_CurrDay->GetNumberOfAllDayEvents() - m_AllDayOffset) * m_LeftSideStringSpace) + *m_TopBarSize + 10 &&
		Point.y <= (((m_CurrDay->GetNumberOfAllDayEvents() - m_AllDayOffset) + 1) * m_LeftSideStringSpace) + *m_TopBarSize + 15) &&
		Point.x >= m_EventButtonWidthMin && Point.x <= m_EventButtonWidthMax)
	{
		m_AddEventButtonSelected = true;
		m_CurrentView->InvalidateRect(nullptr);
	}
	else
	{

		m_AddEventButtonSelected = false;
		m_CurrentView->InvalidateRect(nullptr);

	}

}
