#include "stdafx.h"
#include "ViewBase.h"

#include "..\Dialog\DialogAddEvent.h"

// CTOR for this view object,
// initializes all the required variables for this view
CViewMonthly::CViewMonthly(unsigned *Rows, unsigned *Columns, int *Height,
	int *Width, int* TopBarSize, CPlannerObject* Object)
	: CViewBase(Rows, Columns, Height, Width, TopBarSize, Object),
	m_ContextMenuSelectedDay(nullptr)
{}

//
// DrawLayout()
// Draws the Month style layout for this view
//
void CViewMonthly::DrawLayout(CDC* pDC, CPlannerView* View)
{

	// Initialize variables if not already
	if (m_Width == 0 || m_Height == 0 || m_SizeChanged)
	{
		InitilizeWndVariables(View);
		m_CurrentView = View;
		//m_CurrentMonth->ResetCells(m_WidthPortion, m_HeightPortion);
	}

	// Sets the default color for the background
	FillBackground(pDC, View);

	// Drawing the month section of the UI
	DrawMonthSection(pDC, View);

	// Drawing the top bar/ days of the week section of the UI
	DrawTopBarSection(pDC, View);

	// Draws the rows and columns for the main layout of the month view
	DrawGrid(pDC);

	// Draws the contents of each cell
	DrawCells(pDC);
}

//
// DrawGrid()
// Draws grid lines to correctly display the current month object
//
void CViewMonthly::DrawGrid(CDC* pDC)
{
	// Draws rows based on amount of rows and current
	// dimensions of client area
	for (int height = m_HeightPortion + *m_TopBarSize; height < *m_Height - 10; height += m_HeightPortion)
	{
		pDC->MoveTo(0, height);
		pDC->LineTo(*m_Width, height);
	}
	// Draws columns based on amount of colomns and current
	// dimensions of client area
	for (int width = m_WidthPortion; width < *m_Width - 10; width += m_WidthPortion)
	{
		pDC->MoveTo(width, *m_TopBarSize);
		pDC->LineTo(width, *m_Height);
	}
}

//
// DrawCells()
// Draws the contents of each of the day objects for this planner. 
// Also may get the previous and next months to display days from those
// events if necessary.
//
void CViewMonthly::DrawCells(CDC* pDC)
{
	// Local Variables
	CDay* CurrMonthDay;
	CPlannerEvent *CurrEvent;
	CRect EnclosingRect;
	CString string;
	CFont font;
	CBrush NewBrush;
	bool Enter = 0;
	int BeginningDay = -1;
	int CurrDayOfWeek = 0;
	int Height = 0;

	// Creating the font object
	VERIFY(font.CreateFont(
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

	// Printing each cell
	// First selecting new font
	CFont* def_font = pDC->SelectObject(&font);

	// Then creating new brush and selecting background mode for styling
	NewBrush.CreateSolidBrush(RGB(221, 221, 235));
	pDC->SetBkMode(TRANSPARENT);

	// gets beg day of week (MON-SUN)
	BeginningDay = m_CurrentMonth->ReturnBegDate();

	// Traversing the linked list structure of each month, 
	// and drawing the contents of each cell.
	CurrDayOfWeek = BeginningDay;

	for (int i = 0; i < m_CurrentMonth->m_NumberDays; i++, CurrDayOfWeek++)
	{
		// Getting the new day object and dimensions
		CurrMonthDay = m_CurrentMonth->ReturnDayWithDate(i + 1);
		// check if nullptr
		if (CurrMonthDay == nullptr) break;

		CString String;
		String.Format(L"%d", CurrDayOfWeek);

		// Moving to the next row of cells
		if (((CurrDayOfWeek % *m_Columns) == 0) && CurrDayOfWeek > 0)
		{
			CurrDayOfWeek = 0;
			Height += 1;
		}

		


		// Modifying the dimensions of this object
		EnclosingRect = CRect(CPoint(CurrDayOfWeek * m_WidthPortion, (Height * m_HeightPortion) + *m_TopBarSize),
			CPoint((CurrDayOfWeek + 1) * m_WidthPortion, ((Height + 1) * m_HeightPortion) + *m_TopBarSize));

		String.Format(L"%d, %d", (Height * m_HeightPortion) + *m_TopBarSize,((Height + 1) * m_HeightPortion) + *m_TopBarSize);
		//AfxMessageBox(String);

		// Resetting indicator for all day event display
		Enter = 0;

		// Sets CString object format
		string.Format(L"%u.", CurrMonthDay->GetNumber());

		// Writes the number of this day to the corner of this cell
		// Deflate with respect to pen width
		EnclosingRect.DeflateRect(1, 1);

		// Fill in this rectangle with NewBrush's color
		// This highlights the selected object
		if (CurrMonthDay == m_SelectedDay)pDC->FillRect(EnclosingRect, new CBrush(RGB(150, 150, 160)));
		else pDC->FillRect(EnclosingRect, &NewBrush);

		// If this month is today, then draw something in the cell to indicate today
		if (m_Planner->IsToday(CurrMonthDay))
		{
			CRect *NewEnclosingRect = new CRect(EnclosingRect.TopLeft(),
				CPoint(EnclosingRect.BottomRight().x + 1, EnclosingRect.TopLeft().y + 5));
			pDC->FillRect(NewEnclosingRect, new CBrush(RGB(82, 95, 120)));

		}

		// Reset the rectangles size
		EnclosingRect.InflateRect(1, 1);
		// Finally drawing the text
		pDC->TextOutW(EnclosingRect.TopLeft().x + 5, EnclosingRect.TopLeft().y + 5, string);

		// Drawing the events
		for (int k = 0, n = 1; k < 24; k++)
		{
			// First printing out the all day events
			if (Enter == false)
			{
				// Getting each event
				for (int g = 0; (CurrEvent = CurrMonthDay->GetAllDayEvent(g)) != nullptr; g++)
				{
					// Printing it in the correct position
					if (EnclosingRect.TopLeft().y + 10 + (15 * (n + 1)) + 5 * (n + 1) >= EnclosingRect.BottomRight().y);
					else if ((EnclosingRect.TopLeft().y + 10 + (15 * (n + 2)) + 5 * (n + 2)) >= EnclosingRect.BottomRight().y)
					{
						pDC->TextOutW(EnclosingRect.TopLeft().x + (5), EnclosingRect.TopLeft().y + 5 + (15 * n) + 5 * n, _T(". . ."));
					}
					else
					{
						pDC->TextOutW(EnclosingRect.TopLeft().x + (5), EnclosingRect.TopLeft().y + 10 + (15 * n) + 5 * n, FormatEvent(CurrEvent));
					}
					// Incrementing for each added event
					n++;
				}
				// Set this section as displayed
				Enter = true;
			}
			// Displaying each non null event in its sorted position
			for (int j = 0; (CurrEvent = CurrMonthDay->GetEventForTime(k, j)) != nullptr; j++)
			{
				if (EnclosingRect.TopLeft().y + 10 + (15 * (n + 1)) + 5 * (n + 1) >= EnclosingRect.BottomRight().y);
				else if ((EnclosingRect.TopLeft().y + 10 + (15 * (n + 2)) + 5 * (n + 2)) >= EnclosingRect.BottomRight().y)
				{
					pDC->TextOutW(EnclosingRect.TopLeft().x + (5), EnclosingRect.TopLeft().y + 5 + (15 * n) + 5 * n, _T(". . ."));
				}
				else
				{
					pDC->TextOutW(EnclosingRect.TopLeft().x + (5), EnclosingRect.TopLeft().y + 10 + (15 * n) + 5 * n, FormatEvent(CurrEvent));
				}
				n++;
			}
		}
	}

	// Getting the beginning day of the week for this month object
	int Beginning = m_CurrentMonth->ReturnBegDate();

	// If the beginning day is not a Sunday, then we will have to get the 
	// previous month to display the missing days
	if (Beginning != 0)
	{
		// If this is the first month of this entire planner, then no previous month is present, so do not 
		// display or attempt to recieve previous month
		if (m_CurrentMonth->ReturnMonthType() == 0 && m_Planner->IsStartYear(m_Year->ReturnYearDate()));
		else
		{
			// Getting the previous month
			CMonth* Prev = m_Planner->ReturnPrevMonth();
			CDay* CurrDay;

			// Printing the days of the previous month
			for (int i = 0; i < Beginning; i++)
			{
				// Reset indicator variable for all day events
				Enter = 0;
				// Format the number for these days
				string.Format(L"%d / %d", Prev->ReturnMonthType() + 1, Prev->ReturnDayWithDate(Prev->m_NumberDays - (Beginning - 1 - i))->GetNumber());
				// Getting the currday pointer
				CurrDay = Prev->ReturnDayWithDate(Prev->m_NumberDays - (Beginning - 1 - i));
				// Getting the new ecnlsoing rectangle dimensions for these cells
				EnclosingRect = CRect(CPoint(m_WidthPortion * i, *m_TopBarSize),
					CPoint(m_WidthPortion * (i + 1), *m_TopBarSize + m_HeightPortion));
				// Printing out the day's date in top left corner of this cell
				pDC->TextOutW(m_WidthPortion * i + 5, *m_TopBarSize + 5, string);

				// Drawing the events
				for (int k = 0, n = 1; k < 24; k++)
				{
					// First printing out the all day events
					if (Enter == false)
					{
						// Getting each event
						for (int g = 0; (CurrEvent = CurrDay->GetAllDayEvent(g)) != nullptr; g++)
						{
							// Printing it in the correct position
							if (EnclosingRect.TopLeft().y + 10 + (15 * (n + 1)) + 5 * (n + 1) >= EnclosingRect.BottomRight().y);
							else if ((EnclosingRect.TopLeft().y + 10 + (15 * (n + 2)) + 5 * (n + 2)) >= EnclosingRect.BottomRight().y)
							{
								pDC->TextOutW(EnclosingRect.TopLeft().x + (5), EnclosingRect.TopLeft().y + 5 + (15 * n) + 5 * n, _T(". . ."));
							}
							else
							{
								pDC->TextOutW(EnclosingRect.TopLeft().x + (5), EnclosingRect.TopLeft().y + 10 + (15 * n) + 5 * n, FormatEvent(CurrEvent));
							}
							n++;
						}
						// Set this section as displayed
						Enter = true;
					}

					for (int j = 0; (CurrEvent = CurrDay->GetEventForTime(k, j)) != nullptr; j++)
					{
						// If there is too many events for the space provided
						if (EnclosingRect.TopLeft().y + 10 + (15 * (n + 1)) + 5 * (n + 1) >= EnclosingRect.BottomRight().y)
						{
						}
						else if ((EnclosingRect.TopLeft().y + 10 + (15 * (n + 2)) + 5 * (n + 2)) >= EnclosingRect.BottomRight().y)
						{
							pDC->TextOutW(EnclosingRect.TopLeft().x + (5), EnclosingRect.TopLeft().y + 5 + (15 * n) + 5 * n, _T(". . ."));
						}
						else
						{
							pDC->TextOutW(EnclosingRect.TopLeft().x + (5), EnclosingRect.TopLeft().y + 10 + (15 * n) + 5 * n, FormatEvent(CurrEvent));
						}
						n++;
					}
				}
			}
		}
	}

	// Get the ending day of the week for this month
	int Ending = m_CurrentMonth->ReturnDayWithDate(m_CurrentMonth->m_NumberDays)->GetDayOfWeek();

	// If that ending is not a Saturday, the we will have to get the 
	// next month object to display the missing days in this view
	if (Ending < 6)
	{
		// Checking if the month is the last month in this planner object
		if (m_CurrentMonth->ReturnMonthType() == 11 && m_Planner->IsLastYear(m_Year->ReturnYearDate()));
		else
		{
			// Setting all day indicator as false
			Enter = 0;
			// Getting the next month
			CMonth* Next = m_Planner->ReturnNextMonth();
			CDay * CurrDay;

			// Displaying the days for the next month
			for (int i = Ending + 1, l = 1; i < 7; i++, l++)
			{
				// Getting the curr day object
				CurrDay = Next->ReturnDayWithDate(l);
				// Formatting date for each day
				string.Format(L"%d / %d", Next->ReturnMonthType() + 1, Next->ReturnDayWithDate(l)->GetNumber());
				// Creating the new enclosing rect structure for each cell
				EnclosingRect = CRect(CPoint(m_WidthPortion * i, m_HeightPortion * (*m_Rows - 1) + *m_TopBarSize),
					CPoint(m_WidthPortion * (i + 1), m_HeightPortion * (*m_Rows) + *m_TopBarSize));
				// Printing out the day's date in top left corner
				pDC->TextOutW(m_WidthPortion * i + 5, m_HeightPortion * (*m_Rows - 1) + *m_TopBarSize + 5, string);

				// Drawing the events
				for (int k = 0, n = 1; k < 24; k++)
				{
					// First printing out the all day events
					if (Enter == false)
					{
						// Getting each event
						for (int g = 0; (CurrEvent = CurrDay->GetAllDayEvent(g)) != nullptr; g++)
						{
							// Printing it in the correct position
							if (EnclosingRect.TopLeft().y + 10 + (15 * (n + 1)) + 5 * (n + 1) >= EnclosingRect.BottomRight().y);
							else if ((EnclosingRect.TopLeft().y + 10 + (15 * (n + 2)) + 5 * (n + 2)) >= EnclosingRect.BottomRight().y)
							{
								pDC->TextOutW(EnclosingRect.TopLeft().x + (5), EnclosingRect.TopLeft().y + 5 + (15 * n) + 5 * n, _T(". . ."));
							}
							else
							{
								pDC->TextOutW(EnclosingRect.TopLeft().x + (5), EnclosingRect.TopLeft().y + 10 + (15 * n) + 5 * n, FormatEvent(CurrEvent));
							}
							n++;
						}
						// Set this section as displayed
						Enter = true;
					}

					for (int j = 0; (CurrEvent = CurrDay->GetEventForTime(k, j)) != nullptr; j++)
					{
						// Placing in correct position
						// If the event is out of view
						if (EnclosingRect.TopLeft().y + 10 + (15 * (n + 1)) + 5 * (n + 1) >= EnclosingRect.BottomRight().y)
						{
						}
						// If the event in the first out of view
						else if ((EnclosingRect.TopLeft().y + 10 + (15 * (n + 2)) + 5 * (n + 2)) >= EnclosingRect.BottomRight().y)
						{
							pDC->TextOutW(EnclosingRect.TopLeft().x + (5), EnclosingRect.TopLeft().y + 5 + (15 * n) + 5 * n, _T(". . ."));
						}
						// If the event is within view
						else
						{
							pDC->TextOutW(EnclosingRect.TopLeft().x + (5), EnclosingRect.TopLeft().y + 10 + (15 * n) + 5 * n, FormatEvent(CurrEvent));
						}
						n++;
					}
				}
			}
		}
	}

	// Resetting the font
	pDC->SelectObject(def_font);
	// Deleting the objects
	font.DeleteObject();
	NewBrush.DeleteObject();
}

//
// FormatEvent()
// Takes a planner event and formats its appropriate string format for
// display in the monthly view
//
CString CViewMonthly::FormatEvent(CPlannerEvent *Event)
{
	CString string; // To store and return formatted string

	// If the place string is empty, we will not display it
	if (*Event->GetPlace() != _T(""))
	{
		// If this event is an all day event
		if (Event->IsAllDayEvent())
		{
			string.Format(L"%s : %s", *Event->GetDescription(), *Event->GetPlace());
		}
		// Otherwise, time format
		else
		{
			string.Format(L"%s : %s @ %s", *Event->GetDescription(), *Event->GetPlace(), Event->GetTimeStandardForm());
		}
	}
	else
	{
		// Check if all day
		if (Event->IsAllDayEvent())
		{
			string.Format(L"%s", *Event->GetDescription());
		}
		// Otherwise, time format
		else
		{
			string.Format(L"%s @ %s", *Event->GetDescription(), Event->GetTimeStandardForm());
		}
	}

	if (Event->IsCompleted()) string = string + _T(" -- Completed");

	// Return the new string
	return string;
}

//
// OnNextMonth()
// Handler for next month toolbar button, moves the m_CurrentMonth pointer
// to the new current month
//
int CViewMonthly::OnNextMonth()
{
	// Set area as needing to be redrawn
	m_SizeChanged = 1;

	// Move the current month
	int error = m_Planner->MoveCurrMonth(1);

	// Get the new year, may not always be different
	m_Year = m_Planner->ReturnCurrentYear();

	return error;
}

//
// OnPrevMonth()
// Handler for previous month toolbar button, moves the m_CurrentMonth pointer
// to the new current month
//
int CViewMonthly::OnPrevMonth()
{
	// Set area as needing to be redrawn
	m_SizeChanged = 1;

	// Move the current month
	int error = m_Planner->MoveCurrMonth(-1);

	// Get the new year, may not always be different
	m_Year = m_Planner->ReturnCurrentYear();

	return 0;
}

//
// OnNextYear()
// Handler for the next year toolbar button, moves the m_Year pointer to
// the new current year
//
int CViewMonthly::OnNextYear()
{
	// Set area as needing to be redrawn
	m_SizeChanged = 1;

	// Move the current year in this planner
	int error = m_Planner->MoveCurrYear(1);

	// Get te new year object
	m_Year = m_Planner->ReturnCurrentYear();

	return 1;
}

//
// OnPrevYear()
// Handler for the prev year toolbar button, moves the m_Year pointer to
// the new current year
//
int CViewMonthly::OnPrevYear()
{
	// Set area as needing to be redrawn
	m_SizeChanged = 1;

	// Move the current year in this planner
	int error = m_Planner->MoveCurrYear(-1);

	// Get te new year object
	m_Year = m_Planner->ReturnCurrentYear();

	return 1;
}

//
// MoveRow()
// Handler for the mouse wheel, allows thhe user to move between
// months with ease with scrolling
//
int CViewMonthly::MoveRow(int Movement)
{
	// Local vars
	int error = 0;

	// If the movement was positive, move to next
	if (Movement > 0)
	{
		error = OnNextMonth();
	}
	// Otherwise it was negative, so move to prev.
	else
	{
		error = OnPrevMonth();
	}
	return error;
}

//
// SetObject()
// Sets the appropriate object in the view to the object that was
// clicked on with the mose cursor.
//
bool CViewMonthly::SetObject(CPoint Point, CDay **DayObject)
{
	// Local Variables
	CLLNode* Curr;
	CRect EnclosingRect;
	CString string;
	int CurrDayOfWeek = m_CurrentMonth->ReturnBegDate();
	int Height = 0;

	// Traversing the head of the list of days within the month object
	Curr = m_CurrentMonth->GetList();
	Curr->m_AuxPtr = Curr->GetHead();

	if (Curr->m_AuxPtr == nullptr) return false;


	// Traversing the linked list structure of each month, 
	// and drawing the contents of each cell.
	for (int i = 0; i < m_CurrentMonth->m_NumberDays; i++, CurrDayOfWeek++)
	{
		// Moving to the next row of cells
		if (((CurrDayOfWeek % *m_Columns) == 0) && CurrDayOfWeek > 0)
		{
			CurrDayOfWeek = 0;
			Height += 1;
		}

		// Modifying the dimensions of this object
		EnclosingRect = CRect(CPoint(CurrDayOfWeek * m_WidthPortion, (Height * m_HeightPortion) + *m_TopBarSize),
			CPoint((CurrDayOfWeek + 1) * m_WidthPortion, ((Height + 1) * m_HeightPortion) + *m_TopBarSize));

		// Checks the coordinates for the mouse click
		if (Point.x >= EnclosingRect.TopLeft().x &&
			Point.y >= EnclosingRect.TopLeft().y &&
			Point.x <= EnclosingRect.BottomRight().x &&
			Point.y <= EnclosingRect.BottomRight().y)
		{
			// Selects the appropriate cell in the view
			*DayObject = Curr->m_AuxPtr->Object;
			m_SelectedDay = Curr->m_AuxPtr->Object;
			m_SelectedRect = EnclosingRect;

			m_PreviousSelectedDay = m_SelectedDay;

			return 1;
		}

		// Moves to next node
		Curr->m_AuxPtr = Curr->m_AuxPtr->next;
		// Check if nullptr
		if (Curr->m_AuxPtr == nullptr) break;

	}

	int Beginning = m_CurrentMonth->ReturnBegDate();

	// This is set up properly to add events for the previous month,
	// But it is not a feature of this planner at the moment
	if (Beginning != 0)
	{
		CMonth* Prev = m_Planner->ReturnPrevMonth();

		for (int i = 0; i < Beginning; i++)
		{
		}

	}

	int Ending = m_CurrentMonth->ReturnDayWithDate(m_CurrentMonth->m_NumberDays)->GetDayOfWeek();

	// This is set up properly to add events for the next month,
	// But it is not a feature of this planner at the moment
	if (Ending < 6)
	{
		CMonth* Next = m_Planner->ReturnNextMonth();

		for (int i = Ending + 1, n = 1; i < 7; i++, n++)
		{
		}

	}
	return 0;
}

//
// HandleKeyboardMsg()
// Handles various keyboard messages for this individual view
// Return 5 -- indicates that the main view should create a new dialog.
//
int CViewMonthly::HandleKeyboardMsg(UINT nChar)
{

	// Sets the selected day
	m_SelectedDay = m_PreviousSelectedDay;

	return CViewBase::HandleKeyboardMsg(nChar);
}

//
// ResetSelectedObject()
// Sets the selected object to null to flush the 
// highlighted object in the view
//
bool CViewMonthly::ResetSelectedObject()
{
	m_SelectedDay = nullptr;
	return 1;
}

//
// HandleContextMenu()
// Handles and returns the approp. context menu
//
CMenu* CViewMonthly::HandleContextMenu(CWnd* pWnd, CPoint Point)
{
	// Local Variables
	CMenu Menu;
	CRect EnclosingRect;
	CDay* CurrDay = nullptr;
	int CurrDayOfWeek = m_CurrentMonth->ReturnBegDate();
	int Height = 0;

	// Get the cursor position
	GetCursorPos(&m_CursorPosition);
	// Converting screen coordinates to client coordinates
	ScreenToClient(m_CurrentView->m_hWnd, &m_CursorPosition);

	// Traversing the linked list structure of each month, 
	// and drawing the contents of each cell.
	for (int i = 0; i < m_CurrentMonth->m_NumberDays && (CurrDay = m_CurrentMonth->ReturnDayWithDate(i + 1)) != nullptr ; i++, CurrDayOfWeek++)
	{

		// Moving to the next row of cells
		if (((CurrDayOfWeek % *m_Columns) == 0) && CurrDayOfWeek > 0)
		{
			CurrDayOfWeek = 0;
			Height += 1;
		}

		// Modifying the dimensions of this object
		EnclosingRect = CRect(CPoint(CurrDayOfWeek * m_WidthPortion, (Height * m_HeightPortion) + *m_TopBarSize),
			CPoint((CurrDayOfWeek + 1) * m_WidthPortion, ((Height + 1) * m_HeightPortion) + *m_TopBarSize));

		// Checks the coordinates for the mouse click
		if (m_CursorPosition.x >= EnclosingRect.TopLeft().x &&
			m_CursorPosition.y >= EnclosingRect.TopLeft().y &&
			m_CursorPosition.x <= EnclosingRect.BottomRight().x &&
			m_CursorPosition.y <= EnclosingRect.BottomRight().y)
		{
			// Selects the appropriate cell in the view
			m_ContextMenuSelectedDay = CurrDay;

			CString String;
			String.Format(L"%d/%d", CurrDay->GetMonth(), CurrDay->GetNumber());
			//AfxMessageBox(String);

			// Load the desired context menu
			Menu.LoadMenu(ID_MENU_EVENT);
			// Loading the appropriate submenu for this section
			CMenu* pContext = Menu.GetSubMenu(5);
			// Verify the menu is there
			ASSERT(pContext != nullptr);
			// Make the menu visible
			pContext->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,
				Point.x, Point.y, pWnd);

			return &Menu;
		}

	}


	// Return no menu
	return nullptr;
}

//
// OnClearDay()
// Clears all of the events for the day selected in 
// HandleContextMenu()
//
void CViewMonthly::OnClearDay()
{

	if (m_ContextMenuSelectedDay != nullptr)
		m_ContextMenuSelectedDay->ClearEntireDay();
}

//
// OnGoToDay()
// Returns the appropriate selected day from
// HandleContextMenu()
//
CDay* CViewMonthly::OnGoToDay()
{
	return m_ContextMenuSelectedDay;
}