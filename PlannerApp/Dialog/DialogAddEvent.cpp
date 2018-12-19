// C:\Users\Damon\source\repos\PlannerApp\PlannerApp\Dialog\DialogAddEvent.cpp : implementation file
//

#include <cctype>
#include "stdafx.h"
#include "..\PlannerApp.h"
#include "DialogAddEvent.h"
#include "afxdialogex.h"


bool Validate(CString str);

// CDialogAddEvent dialog

IMPLEMENT_DYNAMIC(CDialogAddEvent, CDialogEx)

CDialogAddEvent::CDialogAddEvent(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ADD_EVENT, pParent), m_AllDayEvent(0), m_Day(new CDay)
{

}

CDialogAddEvent::~CDialogAddEvent()
{
}



void CDialogAddEvent::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_END_DATE, m_EndTime);
	DDX_Text(pDX, IDC_BEG_DATE, m_BegTime);
	DDX_Text(pDX, IDC_EVENT_DESCRIP, m_Description);
	DDX_Text(pDX, IDC_EVENT_PLACE, m_Place);
	DDX_Control(pDX, IDC_EVENT_PLACE, m_PlaceEdit);
	DDX_Control(pDX, IDC_EVENT_DESCRIP, m_DescriptionEdit);
	DDX_Control(pDX, IDC_END_DATE, m_EndEdit);
	DDX_Control(pDX, IDC_BEG_DATE, m_BegEdit);
	DDX_Control(pDX, IDC_DESCRIPTION_ENTRY, m_EventDescript);
}


BEGIN_MESSAGE_MAP(CDialogAddEvent, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDOK, &CDialogAddEvent::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDialogAddEvent::OnBnClickedCancel)
	ON_WM_DESTROY()
	ON_EN_CHANGE(IDC_END_DATE, &CDialogAddEvent::OnEnChangeEndDate)
END_MESSAGE_MAP()


// CDialogAddEvent message handlers

BOOL CDialogAddEvent::OnInitDialog()
{

	CDialogEx::OnInitDialog();

	// Setting the 
	m_EventDescript.SetWindowText(m_TempEventName);
	m_Brush.CreateSolidBrush(RGB(82, 95, 120));

	// Setting text for each of the edit controls
	SetDlgItemText(IDC_END_DATE, m_EndTime);
	SetDlgItemText(IDC_BEG_DATE, m_BegTime);
	SetDlgItemText(IDC_EVENT_DESCRIP, m_Description);
	SetDlgItemText(IDC_EVENT_PLACE, m_Place);

	m_PlaceEdit.SetBackgroundColor(FALSE, RGB(180, 180, 180));
	m_DescriptionEdit.SetBackgroundColor(FALSE, RGB(180, 180, 180));
	m_BegEdit.SetBackgroundColor(FALSE, RGB(180, 180, 180));
	m_EndEdit.SetBackgroundColor(FALSE, RGB(180, 180, 180));


	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CDialogAddEvent::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{

	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(255, 255, 255));
	CFont Font;
	VERIFY(Font.CreateFont(
		45,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_LIGHT,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("Calibri")));

	m_DescriptionEdit.SetFont(&Font);
	Font.DeleteObject();
	VERIFY(Font.CreateFont(
		24,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_LIGHT,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("Calibri")));

	m_PlaceEdit.SetFont(&Font);


	return m_Brush;

}

bool CDialogAddEvent::CheckData()
{

	m_BegEdit.GetWindowText(m_BegTime);
	m_EndEdit.GetWindowText(m_EndTime);
	m_DescriptionEdit.GetWindowText(m_Description);
	m_PlaceEdit.GetWindowText(m_Place);

	if (CheckTimes() == 0)
	{
		AfxMessageBox(_T("Time not formatted correctly."));
		return 0;
	}
	else if (m_Description.GetLength() > 36)
	{
		AfxMessageBox(_T("Description size too large."));
		return 0;
	}
	else if (m_BegTime == _T("") && m_EndTime.GetLength() != 0)
	{
		AfxMessageBox(_T("Beginning Time not entered."));
		return 0;
	}
	else if (m_EndTime == _T("") && m_BegTime.GetLength() != 0)
	{
		AfxMessageBox(_T("Ending Time not entered."));
		return 0;
	}
	else if (m_Description == _T(""))
	{
		AfxMessageBox(_T("Description not entered."));
		return 0;
	}
	else return 1;

}

//
// CheckTimes()
// Verifies that all the times in the dialog both follow
// the format specefied, are numbers, and are a valid range.
// the format is XX:XXp or XX:XXa, such as 12:34p or 3:45a.
bool CDialogAddEvent::CheckTimes()
{
	bool FirstPm = 0;
	bool SecondPm = 0;
	int i = 0;
	CString temp = _T("");

	if (m_BegTime.GetLength() == 0 && m_EndTime.GetLength() == 0)
	{
		m_AllDayEvent = true;
		return 1;
	}

	// Looing at the beginning time
	if (m_BegTime.GetLength() >= 4)
	{
		// Looking at first section of the time
		for (i = 0; m_BegTime[i] != ':'; i++)
		{
			if (!isdigit(m_BegTime[i]))
				return 0;

			if (i > 2)
				return 0;

			temp = temp + m_BegTime[i];
		}


		m_BegDate = _ttoi(temp);

		// Verifying that this section is valid and within 1-12
		if (m_BegDate > 12 || m_BegDate <= 0) return 0;

		// Reseting the cstring obj
		temp = _T("");

		// Looking at second part of the time
		for (; i < m_BegTime.GetLength(); i++)
		{
			// If a digit, add to total
			if (isdigit(m_BegTime[i]))
				temp = temp + m_BegTime[i];

			// If a letter, the time is at the end
			if (isalpha(m_BegTime[i])) break;

		}

		// specifies pm
		if ((m_BegTime[i] == 'p'))
		{
			FirstPm = 1;
		}
		// Specefies am
		else if (m_BegTime[i] == 'a')
		{
			FirstPm = 0;
		}
		// meaning an invalid character was found
		else return 0;

		m_BegDateMinutes = _ttoi(temp);

		// Verifying the minutes part of the time is valid
		if (m_BegDateMinutes > 59 || m_BegDateMinutes < 0) return 0;
	}
	else
	{
		return 0;
	}

	temp = _T("");

	// Loking at the end time
	if (m_EndTime.GetLength() >= 4)
	{
		// Looking at first section of the time
		for (i = 0; m_EndTime[i] != ':'; i++)
		{
			if (!isdigit(m_EndTime[i]))
				return 0;

			if (i > 2)
				return 0;

			temp = temp + m_EndTime[i];
		}

		m_EndDate = _ttoi(temp);

		// Verifying that is section is valid and within 1-12
		if (m_EndDate > 12 || m_EndDate <= 0) return 0;

		// Reseting the cstring obj
		temp = _T("");

		// Looking at second part of the time
		for (; i < m_EndTime.GetLength(); i++)
		{
			// If a digit, add to total
			if (isdigit(m_EndTime[i]))
				temp = temp + m_EndTime[i];

			// If a letter, the time is at the end
			if (isalpha(m_EndTime[i])) break;

		}

		// specifies pm
		if ((m_EndTime[i] == 'p'))
		{
			SecondPm = 1;
		}
		// Specefies am
		else if (m_EndTime[i] == 'a')
		{
			SecondPm = 0;
		}
		// meaning an invalid character was found
		else return 0;

		m_EndDateMinutes = _ttoi(temp);

		// Verifying the minutes part of the time is valid
		if (m_EndDateMinutes > 59 || m_EndDateMinutes < 0) return 0;
	}
	else
	{
		return 0;
	}

	// Need to convert into military time equivalent

	m_BegDate = ConvertToMTime(m_BegDate, FirstPm);
	m_EndDate = ConvertToMTime(m_EndDate, SecondPm);

	CString test;
	test.Format(L"%d %d", m_BegDate, m_EndDate);

	// Now if we have made it this far, we need to verify that
	// the beginning time is less than or equal to the end time.

	if (m_BegDate < m_EndDate)
		return 1;
	else if (m_BegDate == m_EndDate && m_BegDateMinutes <= m_EndDateMinutes)
		return 1;
	else
		return 0;

}

int CDialogAddEvent::ConvertToMTime(int Time, bool pm)
{
	if (pm)
	{

		if (Time == 12)
			return 23;
		else
			return Time + 12;

	}
	else
	{
		if (Time == 12)
			return 0;
		else
			return Time;
	}
}

bool Validate(CString str)
{
	for (int i = 0; i<str.GetLength(); i++) {
		if (!isdigit(str[i]))
			return 0;
	}
	return 1;
}

void CDialogAddEvent::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here

	if (CheckData())
	{
		//if (m_Description == (" ")) AfxMessageBox(_T("ERROR"));
		CPlannerEvent* NewEvent = new CPlannerEvent(m_Description, &m_Place, m_BegDate, m_EndDate, m_BegDateMinutes, m_EndDateMinutes, m_AllDayEvent);

		//View->SendData(NewEvent);
		m_Day->AddEvent(NewEvent);

		View->SetDayObject(m_Day);
		//View->WindowDestroyed();
		//View->UpdateView();
		DestroyWindow();
	}


}


void CDialogAddEvent::OnBnClickedCancel()
{
	//View->UpdateView();
	View->WindowDestroyed();


	CDialogEx::OnCancel();
}


void CDialogAddEvent::OnDestroy()
{
	//View->UpdateView();
	View->WindowDestroyed();

	CDialogEx::OnDestroy();

	// TODO: Add your message handler code here
}


void CDialogAddEvent::OnEnChangeEndDate()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}

bool CDialogAddEvent::SetBegTime(int Hours, int Minutes)
{

	CString Time;
	int NewHours;
	bool pm = 0;

	if (Hours == 0)
		NewHours = 12;
	else if (Hours < 12)
		NewHours = Hours;
	else if (Hours >= 12)
	{
		if (Hours == 12)
			NewHours = 12;
		else
			NewHours = Hours - 12;
		pm = true;
	}
	else
	{
		AfxMessageBox(_T("SetBegTime function error -- time passed was not in correct format."));
		return 0;
	}

	if (pm)
	{
		Time.Format(L"%d:0%dp", NewHours, Minutes);
	}
	else
	{
		Time.Format(L"%d:0%da", NewHours, Minutes);
	}


	m_BegTime = Time;
	m_BegEdit.SetWindowTextW(Time);

	return 1;
}

bool CDialogAddEvent::SetEndTime(int Hours, int Minutes)
{
	// Local Vars
	CString Time;
	int NewHours;
	bool pm = 0;

	if (Hours == 0)
		NewHours = 12;
	else if (Hours < 12)
		NewHours = Hours;
	else if (Hours >= 12)
	{
		if (Hours == 12)
			NewHours = 12;
		else
			NewHours = Hours - 12;
		pm = true;
	}
	else
	{
		AfxMessageBox(_T("SetEndTime function error -- time passed was not in correct format."));
		return 0;
	}

	if (pm)
	{
		Time.Format(L"%d:0%dp", NewHours, Minutes);
	}
	else
	{
		Time.Format(L"%d:0%da", NewHours, Minutes);
	}


	m_EndTime = Time;
	m_EndEdit.SetWindowTextW(Time);

	return 1;
}

//
// PreTranslateMessage()
// Overrride that handles the keypresses before they are given
// to the appropriate edit control in the dialog. Binds the enter
// key to the ok button and binds the esc key to the cancel button.
//
BOOL CDialogAddEvent::PreTranslateMessage(MSG* pMsg)
{
	// Local variables
	bool OkIndicator = false;
	bool CancelIndicator = false;

	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE) CancelIndicator = true;
	}

	BOOL DefaultProcedure = CDialogEx::PreTranslateMessage(pMsg);


	if (CancelIndicator) CDialogEx::OnCancel();
	return DefaultProcedure;
}
