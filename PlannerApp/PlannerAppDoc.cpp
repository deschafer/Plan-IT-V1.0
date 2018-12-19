
// PlannerAppDoc.cpp : implementation of the CPlannerAppDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "PlannerApp.h"
#endif

#include "PlannerAppDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CPlannerAppDoc

IMPLEMENT_DYNCREATE(CPlannerDoc, CDocument)

BEGIN_MESSAGE_MAP(CPlannerDoc, CDocument)
	ON_COMMAND(ID_FILE_SEND_MAIL, &CPlannerDoc::OnFileSendMail)
	ON_UPDATE_COMMAND_UI(ID_FILE_SEND_MAIL, &CPlannerDoc::OnUpdateFileSendMail)
END_MESSAGE_MAP()


// CPlannerAppDoc construction/destruction

CPlannerDoc::CPlannerDoc()
{
	// TODO: add one-time construction code here

}

CPlannerDoc::~CPlannerDoc()
{
}

BOOL CPlannerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CPlannerAppDoc serialization

void CPlannerDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{

		ar << m_Planner;


	}
	else
	{
		m_NewPlanner = new CPlannerObject;

		ar >> m_NewPlanner;

		CString String;
		String.Format(L"%d", m_NewPlanner->ReturnBegYear());
		AfxMessageBox(String);

		InvalidateRect(GetActiveWindow(), nullptr, TRUE);
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CPlannerAppDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CPlannerAppDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CPlannerAppDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CPlannerAppDoc diagnostics

#ifdef _DEBUG
void CPlannerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CPlannerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CPlannerAppDoc commands
