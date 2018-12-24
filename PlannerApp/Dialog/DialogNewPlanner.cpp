// DialogNewPlanner.cpp : implementation file
//

#include "stdafx.h"
#include "..\PlannerApp.h"
#include "DialogNewPlanner.h"
#include "afxdialogex.h"
#include "DialogAddEvent.h"

// CDialogNewPlanner dialog

IMPLEMENT_DYNAMIC(CDialogNewPlanner, CDialogEx)

CDialogNewPlanner::CDialogNewPlanner(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_NEW_PLN, pParent), m_BegDate(_T("")), m_EndDate(_T("")),
	m_PlannerName(_T(""))
{

}

CDialogNewPlanner::~CDialogNewPlanner()
{
}

void CDialogNewPlanner::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_BEG_YEAR, m_BegDate);
	DDX_Text(pDX, IDC_END_YEAR, m_EndDate);
	DDX_Text(pDX, IDC_PLN_NAME, m_PlannerName);
	DDX_Control(pDX, IDC_SPIN2, m_EndingYearSpin);
	DDX_Control(pDX, IDC_SPIN1, m_BegYearSpin);
	DDX_Control(pDX, IDC_BEG_YEAR, m_BegYearEdit);
	DDX_Control(pDX, IDC_END_YEAR, m_EndYearEdit);
	DDX_Control(pDX, IDC_PLN_NAME, m_PlannerNameEdit);
	DDX_Control(pDX, IDC_END_YEAR, m_EndYearEdit);
	DDX_Control(pDX, IDC_BEG_YEAR, m_BegYearEdit);
	DDX_Control(pDX, IDOK, m_OkButton);
}


BEGIN_MESSAGE_MAP(CDialogNewPlanner, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDialogNewPlanner::OnBnClickedOk)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CDialogNewPlanner message handlers


BOOL CDialogNewPlanner::OnInitDialog()
{
	// Local vars
	CString String;
	int YDate = 0;

	CDialogEx::OnInitDialog();

	// Manually setting the buddy's for the spin controls
	m_BegYearSpin.SetBuddy(&m_BegYearEdit);
	m_EndingYearSpin.SetBuddy(&m_EndYearEdit);

	// Setting the integer range for the spin controls
	m_BegYearSpin.SetRange32(1999, 20000);
	m_EndingYearSpin.SetRange32(2000, 20000);

	// Setting richEdit background color
	m_PlannerNameEdit.SetBackgroundColor(FALSE, RGB(180, 180, 180));
	m_BegYearEdit.SetBackgroundColor(FALSE, RGB(180, 180, 180));
	m_EndYearEdit.SetBackgroundColor(FALSE, RGB(180, 180, 180));
	m_OkButton.EnableWindowsTheming(FALSE);
	m_OkButton.SetFaceColor(RGB(180, 180, 180));
	m_OkButton.SetTextColor(RGB(0, 0, 0));

	// Creates member brush to color in the dialog bkgrnd
	m_Brush.CreateSolidBrush(RGB(82, 95, 120));
	m_EditBackground.CreateSolidBrush(RGB(180, 180, 180));

	// Setting new font for OK button
	VERIFY(m_OkFont.CreateFont(
		24,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_LIGHT,                  // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("Calibri")));
	m_OkButton.SetFont(&m_OkFont);

	// Setting dialog text for the dates based off of today's
	// year date
	YDate = GetYearFromToday();
	String.Format(L"%d", YDate);
	SetDlgItemText(IDC_BEG_YEAR, String);
	String.Format(L"%d", YDate + 10);
	SetDlgItemText(IDC_END_YEAR, String);


	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CDialogNewPlanner::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	if (CheckData())
	{
		CDialogEx::OnOK();
	}

	
}


HBRUSH CDialogNewPlanner::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// Sets transparacey and text color
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(255, 255, 255));

	CFont aFont;
	VERIFY(aFont.CreateFont(
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
	m_PlannerNameEdit.SetFont(&aFont);

	aFont.DeleteObject();
	VERIFY(aFont.CreateFont(
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
	m_BegYearEdit.SetFont(&aFont);
	m_EndYearEdit.SetFont(&aFont);

	aFont.DeleteObject();

	// Returns new color of dialog
	return m_Brush;
}

bool CDialogNewPlanner::CheckData()
{
	// Gets the appropriate text for comparisons
	m_BegYearEdit.GetWindowText(m_BegDate);
	m_EndYearEdit.GetWindowText(m_EndDate);
	m_PlannerNameEdit.GetWindowText(m_PlannerName);

	// Checks if any fields are empty
	if (m_BegDate == _T(""))
	{
		AfxMessageBox(_T("Beg empty"));
		return false;
	}
	if (m_EndDate == _T(""))
	{
		AfxMessageBox(_T("end empty"));
		return false;
	}
	if (m_PlannerName == _T(""))
	{
		AfxMessageBox(_T("Please Supply a Planner Name"));
		return false;
	}

	// If not a complete numeric string
	if (!Validate(m_BegDate)) 
	{
		AfxMessageBox(_T("Beginning Year Is Not Numeric"));
		return false;
	}
	if (!Validate(m_EndDate))
	{
		AfxMessageBox(_T("Enging Year Is Not Numeric"));
		return false;
	}

	if (m_PlannerName.GetLength() > )
	{
		AfxMessageBox(_T("Planner Name Is Too Long"));
		return false;
	}

	// Finally, verifying the year range is valid
	m_BegDateNumeric = _ttoi(m_BegDate);
	m_EndDateNumeric = _ttoi(m_EndDate);
	if (!(m_BegDateNumeric < m_EndDateNumeric))
	{
		AfxMessageBox(_T("The Ending Year has a Date that is not Greater than the Beginning Year"));
		return false;
	}

	// If it made it here, the data is formatted correctly
	return true;
}


int GetYearFromToday()
{

	time_t Time = time(NULL);
	tm* p_Time = localtime(&Time);

	// Converting to today's date
	int YearDate = p_Time->tm_year + 1900;

	return YearDate;
}