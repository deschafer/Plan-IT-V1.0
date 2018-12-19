#include "stdafx.h"
#include "ViewBase.h"


CViewBase::CViewBase(unsigned *Rows, unsigned *Columns, int *Height, 
	int *Width, int *TopBarSize, CPlannerObject* Object) : m_SizeChanged(1), m_MonthsComplete(0),
	m_DaysComplete(0), m_Scroll(0)
{
	m_Columns = Columns;
	m_Rows = Rows;
	m_Height = Height;
	m_Width = Width;
	m_TopBarSize = TopBarSize;
	m_Planner = Object;
}

//
// InitilizeWndVariables()
// Gets the client window dimensions and sets the appropriate vars in the class
//
void CViewBase::InitilizeWndVariables(CPlannerView* View)
{
	// Local Variables
	CRect rect;

	// Gets the current year
	m_Year = m_Planner->ReturnCurrentYear();

	// Gets the current month
	m_CurrentMonth = m_Year->CurrentMonth();

	// Check if this month is valid for a 6 row display
	if (m_CurrentMonth->IsSixRowDisplay())
	{
		*m_Rows = 6;
	}
	else
	{
		*m_Rows = 5;
	}

	// Sets the appropriate dimensions for each row or column based
	// on the total number of rows or columns set
	m_HeightPortion = (*m_Height - *m_TopBarSize) / *m_Rows;
	m_WidthPortion = *m_Width / *m_Columns;

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
}

//
// FillBackground()
// Draws Fills the background with the default color
//
void CViewBase::FillBackground(CDC* pDC, CPlannerView* View)
{
	// Local Variables
	CRect EnclosingRect(CPoint(0, 0), CPoint(*m_Width, *m_Height));
	CBrush NewBrush;

	// Creating the new brush
	NewBrush.CreateSolidBrush(RGB(180, 180, 180));

	// Filling the client area
	pDC->FillRect(EnclosingRect, &NewBrush);

	// Deleting the brush object
	NewBrush.DeleteObject();
}

//
// DrawTopBarSection()
// Draws the lower section of the client area topbar.
//
void CViewBase::DrawTopBarSection(CDC* pDC, CPlannerView* View)
{
	// Local Variables
	int FontSize = 25;
	CFont font;
	CRect EnclosingRect(CPoint(0, 37), CPoint(*m_Width, *m_TopBarSize));
	CBrush NewBrush;

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

	// Drawing each day of the week's text
	for (int i = 0; i < 7; i++)
	{
		pDC->TextOutW(m_WidthPortion * i + (m_WidthPortion / 50), 37, m_DayStrings[i]);
	}

	// Selecting the old object
	pDC->SelectObject(def_font);

	// Deleting objects
	font.DeleteObject();
	NewBrush.DeleteObject();
}


void CViewBase::DrawMonthSection(CDC* pDC, CPlannerView* View)
{
	// Local variables
	int FontSize = 38;
	CFont font;
	CString YearDate;
	CRect EnclosingRect(CPoint(0, 0), CPoint(*m_Width, 37));
	CBrush NewBrush;

	// Set the background mode as transparent so text will
	// appear accordingly
	pDC->SetBkMode(TRANSPARENT);

	// Formatting the year date into this string
	YearDate.Format(L"%d", m_Year->ReturnYearDate());

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

	// Save the default font, and set the new font
	NewBrush.CreateSolidBrush(RGB(82, 95, 120));

	// Color in the rectangle for this section of the topbar
	pDC->FillRect(EnclosingRect, &NewBrush);

	// Set the color of the text 
	pDC->SetTextColor(RGB(50, 50, 50));

	// Draw the current month text
	pDC->TextOutW(5, 0, m_MonthStrings[m_CurrentMonth->ReturnMonthType()]);
	// Draw the current year date text
	pDC->TextOutW(pDC->GetTextExtent(m_MonthStrings[m_CurrentMonth->ReturnMonthType()]).cx + 25, 0, YearDate);
	// Select default font
	pDC->SelectObject(def_font);

	// Delete the font object.
	font.DeleteObject();
	NewBrush.DeleteObject();

	// Draws the last line for this section
	pDC->MoveTo(0, 35);
	pDC->LineTo(*m_Width, 35);
}

//
// HandleKeyboardMsg()
// Handles various keyboard messages for this individual view
// Return 5 -- indicates that the main view should create a new dialog.
//
int CViewBase::HandleKeyboardMsg(UINT nChar)
{

	if (nChar == VK_SPACE)
	{
		
		return 5;
	}

}