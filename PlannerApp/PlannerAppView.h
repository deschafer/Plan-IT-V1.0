
// PlannerAppView.h : interface of the CPlannerAppView class
//

#pragma once

#include "PlannerAppDoc.h"
#include "PlanAPI\Month.h"
#include "PlanAPI\Year.h"
#include "PlanAPI\PlannerObject.h"
#include "SubView\ViewBase.h"
#include "Dialog\DialogAddEvent.h"
#include "Dialog\DialogNewPlanner.h"

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
	int SetPlannerObject(CPlannerDoc* Doc);
	void SetActiveSubview(SubView SubViewType);
	bool CreateNewPlanner();
	bool OpenPreviousPlanner(CString &NewPathname);
	bool OpenFile(CString AbsPathname);
	void SetCurrentPathname(CString NewPathName);
	CPlannerObject* GetPlanner() { return m_Planner; }

	CDC* m_pDC;
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
	int m_HasSaved;			// Indicates if the current file has been saved
	int m_DraggedEvent;		// Indicates if we now need to handle mouse movement

	CString m_CurrentPathname;	// Stores the current pathname of the open file

	CPoint* m_PreviousPoint;	// Previous point where user clicked

	int m_DialogCount;
	int m_DialogMax;
	CDialogEx* m_OpenDialog;

	CPlannerObject *m_Planner;

	CViewBase* m_CurrentView;
	CViewMonthly m_MonthlyView;
	CViewWeekly m_WeeklyView;
	CViewDaily m_DailyView;
	CViewDefault m_DefaultView;

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnNextMonth();
	afx_msg void OnPrevMonth();
	afx_msg void OnYearPlus();
	afx_msg void OnYearMinus();
	afx_msg void OnViewMonthly();
	afx_msg void OnViewWeekly();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnViewDaily();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnClearDay();
	afx_msg void OnDayDeleteEvent();
	afx_msg void OnDayMarkAsCompleted();
	afx_msg void OnClearHour();
	afx_msg void OnMarkNextAsCompleted();
	afx_msg void OnRemoveFirstEvent();
	afx_msg void OnGoToDay();
	afx_msg void OnFileOpenPlanner();
	afx_msg void OnSavePlanner();
	afx_msg void OnSavePlannerAs();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnViewStartPage();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);


	


};

#ifndef _DEBUG  // debug version in PlannerAppView.cpp
inline CPlannerDoc* CPlannerView::GetDocument() const
{
	return reinterpret_cast<CPlannerDoc*>(m_pDocument);
}
#endif

