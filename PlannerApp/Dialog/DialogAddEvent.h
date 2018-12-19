#pragma once
#include "..\resource.h"
#include "afxcmn.h"
#include "..\PlannerAppView.h"
#include "afxwin.h"

// CDialogAddEvent dialog

class CDialogAddEvent : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogAddEvent)
	CBrush m_Brush;

	bool CheckData();
	bool CheckTimes();
	int ConvertToMTime(int Time, bool pm);

public:
	CString m_Description;
	CString m_Place;
	CString m_BegTime;
	CString m_EndTime;
	int m_BegDate;
	int m_EndDate;
	int m_BegDateMinutes;
	int m_EndDateMinutes;
	bool m_AllDayEvent;

	CString m_TempEventName;

	CPlannerView* View;
	CDay *m_Day;

	bool SetBegTime(int Hours, int Minutes);
	bool SetEndTime(int Hours, int Minutes);

	CDialogAddEvent(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDialogAddEvent();

	// Dialog Data

	enum { IDD = IDD_ADD_EVENT };

	virtual BOOL OnInitDialog();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedOk();
	// Place edit control
	CRichEditCtrl m_PlaceEdit;
	CRichEditCtrl m_DescriptionEdit;
	CRichEditCtrl m_EndEdit;
	CRichEditCtrl m_BegEdit;
	CStatic m_EventDescript;
	afx_msg void OnBnClickedCancel();
	afx_msg void OnDestroy();
	afx_msg void OnEnChangeEndDate();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};