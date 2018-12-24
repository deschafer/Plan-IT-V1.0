#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "..\resource.h"
#include "afxbutton.h"

// CDialogNewPlanner dialog

class CDialogNewPlanner : public CDialogEx
{
private:

	CBrush m_Brush;
	CBrush m_EditBackground;
	CFont m_OkFont;
	DECLARE_DYNAMIC(CDialogNewPlanner)
	CString m_BegDate;
	CString m_EndDate;

	bool CheckData();

public:
	CDialogNewPlanner(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDialogNewPlanner();

// Dialog Data

	enum { IDD = IDD_NEW_PLN };

	int m_BegDateNumeric;
	int m_EndDateNumeric;
	CString m_PlannerName;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CSpinButtonCtrl m_EndingYearSpin;
	CSpinButtonCtrl m_BegYearSpin;
	CRichEditCtrl m_PlannerNameEdit;
	CRichEditCtrl m_BegYearEdit;
	CRichEditCtrl m_EndYearEdit;
	CMFCButton m_OkButton;
};

int GetYearFromToday();