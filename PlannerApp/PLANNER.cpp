#include "stdafx.h"
#include "PlannerAppView.h"
#include "resource.h"

void CPlannerView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	m_CurrentView->Update();

}

void CPlannerView::OnLButtonUp(UINT nFlags, CPoint point)
{

	if (m_Planner == nullptr && m_CurrentView != &m_DefaultView) return;

	// Local variables
	CWnd* temp = nullptr;
	CString String;
	LoadLibrary(_T("riched32.dll"));
	CRect *ORect = nullptr;

	// Resets flag indicating we are handling mouse movement
	m_DraggedEvent = 0;

	// Set the previous point pointer
	m_PreviousPoint = new CPoint(point);

	// Construct the dialog box passing the 
	// ID of the dialog template resource
	if (m_DialogCount < m_DialogMax)
	{
		// Create new dialog object
		CDialogAddEvent * aDlg = new CDialogAddEvent();

		if (m_CurrentView->SetObject(point, &aDlg->m_Day))
		{
			aDlg->m_TempEventName.Format(L"Add Event: %d/%d/%d", aDlg->m_Day->GetMonth() + 1, aDlg->m_Day->GetNumber(), aDlg->m_Day->GetYear());

			ORect = m_CurrentView->GetSelectedRect();

			// Creating the dialog object
			aDlg->Create(IDD_ADD_EVENT, this);
			m_DialogCount++;		// Incrementing the open dialog count
			m_OpenDialog = aDlg;	// Setting the open dialog pointer

									// Now calculating the position of the dialog.
									// First, getting the Rect object of the dialog
			CRect Rect;
			aDlg->GetWindowRect(&Rect);
			Rect.NormalizeRect();

			// Next saving the dimensions of this object
			int dx = Rect.Width();
			int dy = Rect.Height();

			// Then moving the object to the location of the cursor
			Rect.OffsetRect(point.x, point.y);

			// Getting client area dimensions
			CRect Client;
			GetClientRect(&Client);
			CRect rect;

			// Places the rectangle so that is always visible
			if (point.x + dx > Client.Width()) Rect.OffsetRect(-dx, 0);
			if (point.y + dy > Client.Height()) Rect.OffsetRect(0, -dy);

			// Set the pointer to this view
			aDlg->View = this;

			// Lastly doing any subview-specefic operations before displaying the dialog
			m_CurrentView->SetDialogData(aDlg);

			// Move window to new location and display the window, updating the location of the change.
			aDlg->MoveWindow(Rect);
			aDlg->ShowWindow(SW_SHOW);
			InvalidateRect(nullptr);
		}

	}
	else
	{
		if (m_CurrentView->GetSelectedObject() != nullptr)
			ORect = m_CurrentView->GetSelectedRect();

		m_OpenDialog->DestroyWindow();
		InvalidateRect(nullptr);

	}
}

void CPlannerView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	

}

void CPlannerView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// Some sort of indicator variable is needed so button up knows if dragging
	// Current view drag items here

	m_DraggedEvent = m_CurrentView->HandleMouseDrag(point);


}


void CPlannerView::OnMouseMove(UINT nFlags, CPoint point)
{

	if(m_CurrentView == &m_DefaultView || m_DraggedEvent && m_Planner != nullptr 
		|| m_CurrentView == &m_DailyView)
		m_CurrentView->HandleMouseMove(point);

	CView::OnMouseMove(nFlags, point);
}


void CPlannerView::CreateNewEventDialog()
{
	// If there was a previous point, then create a new click where the previous 
	// point was.
	if (m_PreviousPoint != nullptr) OnLButtonUp(1, *m_PreviousPoint);
}

void CPlannerView::OnNextMonth()
{
	int error = 0;

	error = m_CurrentView->OnNextMonth();

	if (!error) InvalidateRect(nullptr);
}


void CPlannerView::OnPrevMonth()
{
	int error = 0;

	error = m_CurrentView->OnPrevMonth();

	if (!error) InvalidateRect(nullptr);
}


void CPlannerView::OnYearPlus()
{
	int error = 0;

	error = m_CurrentView->OnNextYear();

	if (error) InvalidateRect(nullptr);
}


void CPlannerView::OnYearMinus()
{
	int error = 0;

	error = m_CurrentView->OnPrevYear();

	if (error) InvalidateRect(nullptr);
}


void CPlannerView::OnViewMonthly()
{
	if (m_CurrentView == &m_MonthlyView) return;
	else if (m_CurrentView == &m_DefaultView) return;
	else
	{
		m_CurrentView = &m_MonthlyView;
		m_CurrentView->Reset();
		InvalidateRect(nullptr);
	}
}


void CPlannerView::OnViewWeekly()
{
	if (m_CurrentView == &m_WeeklyView) return;
	else if (m_CurrentView == &m_DefaultView) return;
	else
	{
		m_CurrentView = &m_WeeklyView;
		m_CurrentView->Reset();
		InvalidateRect(nullptr);
	}
}

void CPlannerView::OnViewDaily()
{
	if (m_CurrentView == &m_DailyView) return;
	else if (m_CurrentView == &m_DefaultView) return;
	else
	{
		m_CurrentView = &m_DailyView;
		m_CurrentView->Reset();
		InvalidateRect(nullptr);
	}
}



BOOL CPlannerView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{

	if (zDelta > 0)
	{
		m_CurrentView->SetScroll();
		m_CurrentView->MoveRow(zDelta);

	}
	else
	{
		m_CurrentView->SetScroll();
		m_CurrentView->MoveRow(zDelta);

	}


	InvalidateRect(nullptr);
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}

int CPlannerView::SendData(CPlannerEvent* Event)
{

	//AfxMessageBox(*Event->GetDescription());
	return 0;
}

BOOL CPlannerView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return FALSE;
}




void CPlannerView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	int Response = 0;

	Response = m_CurrentView->HandleKeyboardMsg(nChar);

	if (Response == 5) // Indicating that a new dialog should be opened
	{
		CreateNewEventDialog();
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

//
// OnContextMenu()
// 
//
void CPlannerView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CMenu* Menu;

	Menu = m_CurrentView->HandleContextMenu(this, point);

	if (Menu != nullptr)
	{

	}
}


void CPlannerView::OnClearDay()
{

	m_CurrentView->OnClearDay();

	InvalidateRect(nullptr);

}


void CPlannerView::OnDayDeleteEvent()
{
	m_CurrentView->OnDeleteEvent();

	InvalidateRect(nullptr);
}


void CPlannerView::OnDayMarkAsCompleted()
{
	m_CurrentView->OnDayMarkAsCompleted();

	InvalidateRect(nullptr);
}


void CPlannerView::OnClearHour()
{
	m_CurrentView->OnClearHour();

	InvalidateRect(nullptr);
}


void CPlannerView::OnMarkNextAsCompleted()
{
	m_CurrentView->OnMarkNextAsCompleted();

	InvalidateRect(nullptr);
}


void CPlannerView::OnRemoveFirstEvent()
{
	m_CurrentView->OnRemoveFirstEvent();

	InvalidateRect(nullptr);
}


void CPlannerView::OnGoToDay()
{
	// Local Vars
	CDay* TargetDay;

	if ((TargetDay = m_CurrentView->OnGoToDay()) != nullptr)
	{
		// Set current view as daily view
		m_CurrentView = &m_DailyView;

		// Sets the new day
		m_DailyView.SetCurrentDay(TargetDay);

		InvalidateRect(nullptr);
	}

}


void CPlannerView::OnFileOpenPlanner()
{
	
	// Block local vars
	CString sFilePath = _T("");
	const TCHAR szFilter[] = _T("planner Files (*.pln)|*.pln|All Files (*.*)|*.*||");
	CFileDialog dlg(TRUE, _T("pln"), NULL, OFN_FILEMUSTEXIST, szFilter, this);

	// Opening a modal file dialog
	if (dlg.DoModal() == IDOK)
	{
		sFilePath = dlg.GetPathName();
	}
	// If the pathname recieved is not empty
	if (sFilePath != _T(""))
	{
		//Sleep(4000);
		//AfxMessageBox(_T("h"));
		//AfxMessageBox(sFilePath);

		// Opens the file
		GetDocument()->OnOpenDocument(sFilePath);

		// Sets new document title
		CString String;
		String.Format(L"Plan-IT! V1.0.0 -- ");
		String = String + sFilePath;
		(AfxGetMainWnd())->SetWindowText(String);


		// Moves active subview to monthly view
		SetActiveSubview(SubView::Monthly);

		m_CurrentPathname = sFilePath;

		GetDocument()->CreatePreviousOpenedFile(sFilePath);
	}

	m_HasSaved = 1;
}


void CPlannerView::OnSavePlanner()
{
	if (m_Planner == nullptr || m_CurrentView == &m_DefaultView)
	{
		AfxMessageBox(_T("Please Create or Open A Planner First"));
		return;
	}

	if (m_HasSaved) 
	{
		//AfxMessageBox(m_CurrentPathname);
		GetDocument()->OnSaveDocument(m_CurrentPathname);
	}
	else
	{
		// Block local vars
		CString sFilePath = _T("");
		const TCHAR szFilter[] = _T("planner Files (*.pln)|*.pln|All Files (*.*)|*.*||");
		CFileDialog dlg(FALSE, _T("pln"), m_Planner->ReturnPlannerName(), OFN_FILEMUSTEXIST, szFilter, this);

		// Opening a modal file dialog
		if (dlg.DoModal() == IDOK)
		{
			sFilePath = dlg.GetPathName();
		}
		// If the pathname recieved is not empty
		if (sFilePath != _T(""))
		{
			// Opens the file
			GetDocument()->OnSaveDocument(sFilePath);

			// Sets new document title
			CString String;
			String.Format(L"Plan-IT! V1.0.0 -- ");
			String = String + sFilePath;
			m_CurrentPathname = sFilePath;
			(AfxGetMainWnd())->SetWindowText(String);
			GetDocument()->CreatePreviousOpenedFile(sFilePath);

			m_HasSaved = 1;
		}
	}
}


void CPlannerView::OnSavePlannerAs()
{
	if (m_Planner == nullptr || m_CurrentView == &m_DefaultView)
	{
		AfxMessageBox(_T("Please Create or Open A Planner First"));
		return;
	}

	// Block local vars
	CString sFilePath = _T("");
	const TCHAR szFilter[] = _T("planner Files (*.pln)|*.pln|All Files (*.*)|*.*||");
	CFileDialog dlg(FALSE, _T("pln"), _T("New File Name"), OFN_FILEMUSTEXIST, szFilter, this);

	// Opening a modal file dialog
	if (dlg.DoModal() == IDOK)
	{
		sFilePath = dlg.GetPathName();
	}
	// If the pathname recieved is not empty
	if (sFilePath != _T(""))
	{
		// Opens the file
		GetDocument()->OnSaveDocument(sFilePath);

		// Sets new document title
		CString String;
		String.Format(L"Plan-IT! V1.0.0 -- ");
		String = String + sFilePath;
		m_CurrentPathname = sFilePath;
		(AfxGetMainWnd())->SetWindowText(String);
		GetDocument()->CreatePreviousOpenedFile(sFilePath);

	}
}

void CPlannerView::OnViewStartPage()
{

	m_CurrentView = &m_DefaultView;

}

