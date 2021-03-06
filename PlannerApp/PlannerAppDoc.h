
// PlannerAppDoc.h : interface of the CPlannerAppDoc class
//


#pragma once
#include "PlanAPI\PlannerObject.h"
#include "SubView\ViewBase.h"

class CPlannerView;


class CPlannerDoc : public CDocument
{
protected: // create from serialization only
	CPlannerDoc();
	DECLARE_DYNCREATE(CPlannerDoc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CPlannerDoc();

	CPlannerObject* m_Planner;
	CPlannerObject* m_NewPlanner;

	CPlannerView* View;

	bool m_Modified;

	int CreatePreviousOpenedFile(CString Filename);
	
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
};
