
#include "stdafx.h"
#include "ViewBase.h"

#include "..\Dialog\DialogAddEvent.h"

// CTOR for a CViewDaily subview object
CViewDefault::CViewDefault(int *Width, int *Height, int * TopBarPortion) :
	CViewBase(0, 0, Height, Width, TopBarPortion, nullptr), m_ContinueHover(0),
	m_CreateHover(0), m_OpenHover(0)
{
	m_Rows = new unsigned;
	m_Columns = new unsigned;

	*m_Rows = 8;
	*m_Columns = 2;
}

//
//
//
//
void CViewDefault::DrawLayout(CDC* pDC, CPlannerView* View)
{

	// Initialize variables if not already
	if (m_Width == 0 || m_Height == 0 || m_SizeChanged)
	{
		InitilizeWndVariables(View);
		
	}

	m_CurrentView = View;

	// Get the cursor position
	GetCursorPos(&m_CursorPosition);
	ScreenToClient(View->m_hWnd, &m_CursorPosition);

	// Drawing a default background
	FillBackground(pDC, View);

	// Drawing basic welcome box
	DrawMainBox(pDC, 0, *m_TopBarSize, *m_Width, *m_Height);

}

//
// DrawMainBox()
//
//
void CViewDefault::DrawMainBox(CDC* pDC, int WidthMin, int HeightMin, int WidthMax, int HeightMax)
{
	// Local Vars
	CRect EnclosingRect;
	CBrush NewBrush(RGB(100, 110, 130));
	CBrush SecondBrush(RGB(0, 0, 0));
	CString String;
	CFont aFont;
	CFont *OldFont;

	// Creating the font object
	VERIFY(aFont.CreateFont(
		30,                        // nHeight
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
	OldFont = pDC->SelectObject(&aFont);
	pDC->SetBkMode(TRANSPARENT);

	EnclosingRect.SetRect(CPoint((WidthMax - WidthMin) / 2 - 220,  (HeightMax - HeightMin) / 2  - 280), 
		CPoint((WidthMax - WidthMin) / 2 + 220, (HeightMax - HeightMin) / 2 + 250));
	
	// Drawing outline of box
	pDC->FillRect(&EnclosingRect, &NewBrush);
	pDC->FrameRect(&EnclosingRect, &SecondBrush);

	// Welcome text
	String.Format(L"Welcome to Plan-IT!");
	pDC->TextOutW((WidthMax - WidthMin) / 2 - pDC->GetTextExtent(String).cx / 2, (HeightMax - HeightMin) / 2 - 210, String);

	String.Format(L"Select An Option Below To Get Started.");
	pDC->TextOutW((WidthMax - WidthMin) / 2 - pDC->GetTextExtent(String).cx / 2, (HeightMax - HeightMin) / 2 - 170, String);

	// Selection boxes
	
	NewBrush.DeleteObject();
	NewBrush.CreateSolidBrush(RGB(180, 190, 210));

	// Exit box
	EnclosingRect.SetRect(CPoint((WidthMax - WidthMin) / 2 - 150, (HeightMax - HeightMin) / 2 - 100),
		CPoint((WidthMax - WidthMin) / 2 + 150, (HeightMax - HeightMin) / 2 - 60));
	if (m_ContinueHover) pDC->FillRect(&EnclosingRect, &CBrush(RGB(120, 120, 120)));
	else pDC->FillRect(&EnclosingRect, &NewBrush);
	pDC->FrameRect(&EnclosingRect, &SecondBrush);
	String.Format(L"Continue");
	pDC->TextOutW((WidthMax - WidthMin) / 2 - pDC->GetTextExtent(String).cx / 2, (HeightMax - HeightMin) / 2 - 95, String);

	// Create new planner button box
	EnclosingRect.SetRect(CPoint((WidthMax - WidthMin) / 2 - 150, (HeightMax - HeightMin) / 2 - 20),
		CPoint((WidthMax - WidthMin) / 2 + 150, (HeightMax - HeightMin) / 2 + 20));
	if (m_CreateHover) pDC->FillRect(&EnclosingRect, &CBrush(RGB(120, 120, 120)));
	else pDC->FillRect(&EnclosingRect, &NewBrush);
	pDC->FrameRect(&EnclosingRect, &SecondBrush);
	String.Format(L"Create A New Planner");
	pDC->TextOutW((WidthMax - WidthMin) / 2 - pDC->GetTextExtent(String).cx / 2, (HeightMax - HeightMin) / 2 - 15, String);

	// Open existing planner box
	EnclosingRect.SetRect(CPoint((WidthMax - WidthMin) / 2 - 150, (HeightMax - HeightMin) / 2 + 60),
		CPoint((WidthMax - WidthMin) / 2 + 150, (HeightMax - HeightMin) / 2 + 100));
	if (m_OpenHover) pDC->FillRect(&EnclosingRect, &CBrush(RGB(120, 120, 120)));
	else pDC->FillRect(&EnclosingRect, &NewBrush);
	pDC->FrameRect(&EnclosingRect, &SecondBrush);
	String.Format(L"Open An Existing Planner");
	pDC->TextOutW((WidthMax - WidthMin) / 2 - pDC->GetTextExtent(String).cx / 2, (HeightMax - HeightMin) / 2 + 65, String);

	aFont.DeleteObject();

	// Creating the font object
	VERIFY(aFont.CreateFont(
		12,                        // nHeight
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

	pDC->SelectObject(&aFont);

	//String.Format(L"Welcome to Plan-IT!");
	//pDC->TextOutW((WidthMax - WidthMin) / 2 - pDC->GetTextExtent(String).cx / 2, (HeightMax - HeightMin) / 2 - 210, String);

	// Version Info.
	pDC->TextOutW((WidthMax - WidthMin) / 2 - pDC->GetTextExtent(_T("Plan-IT Planner Application V1.0.0 -- Decemeber 2018")).cx / 2, 
		(HeightMax - HeightMin) / 2 + 145, _T("Plan-IT Planner Application V1.0.0 -- Decemeber 2018"));
	pDC->TextOutW((WidthMax - WidthMin) / 2 - pDC->GetTextExtent(_T("Created by Damon E Schafer -- Inquiries desBus@gmail.com")).cx / 2, 
		(HeightMax - HeightMin) / 2 + 155, _T("Created by Damon E Schafer -- Inquiries desBus@gmail.com"));


	// Resetting selected objects
	pDC->SelectObject(OldFont);
	aFont.DeleteObject();
	NewBrush.DeleteObject();
	SecondBrush.DeleteObject();

}


//
// SetObject()
// Always return false, as we do not want to create a new event dialog.
// Use this to process button click only
//
bool CViewDefault::SetObject(CPoint Point, CDay** DayObject)
{
	CString NewPathname;
	CRect EnclosingRect;

	EnclosingRect.SetRect(CPoint((*m_Width) / 2 - 150, (*m_Height - *m_TopBarSize) / 2 - 100),
		CPoint((*m_Width) / 2 + 150, (*m_Height - *m_TopBarSize) / 2 - 60));

	if (Point.x >= EnclosingRect.TopLeft().x &&
		Point.x <= EnclosingRect.BottomRight().x &&
		Point.y >= EnclosingRect.TopLeft().y &&
		Point.y <= EnclosingRect.BottomRight().y)
	{
		if (m_CurrentView->OpenPreviousPlanner(NewPathname) && m_CurrentView->OpenFile(NewPathname))
		{
		}
		else
		{
			AfxMessageBox(_T("No Previous Planner To Open"));
		}
	}

	EnclosingRect.SetRect(CPoint((*m_Width) / 2 - 150, (*m_Height - *m_TopBarSize) / 2 - 20),
		CPoint((*m_Width) / 2 + 150, (*m_Height - *m_TopBarSize) / 2 + 20));

	if (Point.x >= EnclosingRect.TopLeft().x &&
		Point.x <= EnclosingRect.BottomRight().x &&
		Point.y >= EnclosingRect.TopLeft().y &&
		Point.y <= EnclosingRect.BottomRight().y)
	{
		

		if (m_CurrentView->CreateNewPlanner())
		{
			m_CurrentView->SetActiveSubview(SubView::Monthly);
			m_CurrentView->SetCurrentPathname(NewPathname);
		}
		// Error case
		else
		{

		}
	}

	EnclosingRect.SetRect(CPoint((*m_Width) / 2 - 150, (*m_Height - *m_TopBarSize) / 2 + 60),
		CPoint((*m_Width) / 2 + 150, (*m_Height - *m_TopBarSize) / 2 + 100));

	// Processes click on open existing file
	if (Point.x >= EnclosingRect.TopLeft().x &&
		Point.x <= EnclosingRect.BottomRight().x &&
		Point.y >= EnclosingRect.TopLeft().y &&
		Point.y <= EnclosingRect.BottomRight().y)
	{
		m_CurrentView->OnFileOpenPlanner();

	}

	return false;
}

//
// HandleMouseMove()
// Handes mouse movement -- 
//
void CViewDefault::HandleMouseMove(CPoint Point)
{
	CRect EnclosingRect;

	EnclosingRect.SetRect(CPoint((*m_Width) / 2 - 150, (*m_Height - *m_TopBarSize) / 2 - 100),
		CPoint((*m_Width) / 2 + 150, (*m_Height - *m_TopBarSize) / 2 - 60));

	if (Point.x >= EnclosingRect.TopLeft().x &&
		Point.x <= EnclosingRect.BottomRight().x &&
		Point.y >= EnclosingRect.TopLeft().y &&
		Point.y <= EnclosingRect.BottomRight().y)
	{
		m_ContinueHover = 1;
		m_OpenHover = 0;
		m_CreateHover = 0;
		m_CurrentView->InvalidateRect(nullptr);
		return;
	}

	EnclosingRect.SetRect(CPoint((*m_Width) / 2 - 150, (*m_Height - *m_TopBarSize) / 2 - 20),
		CPoint((*m_Width) / 2 + 150, (*m_Height - *m_TopBarSize) / 2 + 20));

	if (Point.x >= EnclosingRect.TopLeft().x &&
		Point.x <= EnclosingRect.BottomRight().x &&
		Point.y >= EnclosingRect.TopLeft().y &&
		Point.y <= EnclosingRect.BottomRight().y)
	{
		m_CreateHover = 1;
		m_OpenHover = 0;
		m_ContinueHover = 0;
		m_CurrentView->InvalidateRect(nullptr);
		return;
	}

	EnclosingRect.SetRect(CPoint((*m_Width) / 2 - 150, (*m_Height - *m_TopBarSize) / 2 + 60),
		CPoint((*m_Width) / 2 + 150, (*m_Height - *m_TopBarSize) / 2 + 100));

	// Processes click on open existing file
	if (Point.x >= EnclosingRect.TopLeft().x &&
		Point.x <= EnclosingRect.BottomRight().x &&
		Point.y >= EnclosingRect.TopLeft().y &&
		Point.y <= EnclosingRect.BottomRight().y)
	{
		m_OpenHover = 1;
		m_CreateHover = 0;
		m_ContinueHover = 0;
		m_CurrentView->InvalidateRect(nullptr);
		return;
	}

	if (m_OpenHover || m_ContinueHover || m_CreateHover) m_CurrentView->InvalidateRect(nullptr);

	m_OpenHover = 0;
	m_ContinueHover = 0;
	m_CreateHover = 0;
}
