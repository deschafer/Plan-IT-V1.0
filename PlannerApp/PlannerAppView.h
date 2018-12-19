
// PlannerAppView.h : interface of the CPlannerAppView class
//

#pragma once

#include "PlannerAppDoc.h"
#include "PlanAPI\Month.h"
#include "PlanAPI\Year.h"
#include "PlanAPI\PlannerObject.h"
#include "SubView\ViewBase.h"
#include "Dialog\DialogAddEvent.h"

class CPlannerView : public CView
{
protected: // create from serialization only
	CPlannerView();
	DECLARE_DYNCREATE(CPlannerView)

// Attributes
public:
	CPlannerDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CPlannerView();
	int ReturnRows() { return m_Rows; }						// Returns the number of rows
	int ReturnColumns() { return m_Columns; }				// Returns number of columns
	int ReturnWidthPortion() { return m_WidthPortion; }		// Returns the width of each cell
	int ReturnHeightPortion() { return m_HeightPortion; }	// Returns the height of each cell
	int ReturnTopSize() { return m_TopBarSize; }			// Returns the size of the top portion of the client area
	int SendData(CPlannerEvent* Event);
	int SetDayObject(CDay* Day);							// Sets the day object for this planner
	void WindowDestroyed();
	void UpdateView() { m_CurrentView->ResetSelectedObject(); }
	void CreateNewEventDialog();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	int m_Width;			// To store the total width of this client area
	int m_Height;			// To store the total height of this client area
	int m_HeightPortion;	// To store the section size for each row
	int m_WidthPortion;		// To Store the section size for each column
	int m_TopBarSize;		// The size of the top bar in this view
	int m_WeeklyHeightPortion;	// The size for each row in the weekly view
	unsigned m_Rows;		// Stores the count of the number of rows
	unsigned m_Columns;		// Stores the count of the number of columns

	CPoint* m_PreviousPoint;	// Previous point where user clicked

	int m_DialogCount;
	int m_DialogMax;
	CDialogEx* m_OpenDialog;


	CPlannerObject *m_Planner;

	CViewBase* m_CurrentView;
	CViewMonthly m_MonthlyView;
	CViewWeekly m_WeeklyView;
	CViewDaily m_DailyView;

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in PlannerAppView.cpp
inline CPlannerAppDoc* CPlannerAppView::GetDocument() const
   { return reinterpret_cast<CPlannerAppDoc*>(m_pDocument); }
#endif

