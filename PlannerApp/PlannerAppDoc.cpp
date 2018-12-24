// PlannerAppDoc.cpp : implementation of the CPlannerAppDoc class
// Modified by Damon E Schafer -- Dec. 2018
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "PlannerApp.h"
#endif

#include "PlannerAppDoc.h"

#include <propkey.h>
#include <fstream>

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
	m_NewPlanner = nullptr;
	View = nullptr;

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

		View->InvalidateRect(nullptr);
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


BOOL CPlannerDoc::OnOpenDocument(LPCTSTR lpszPathName)
{

	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// TODO:  Add your specialized creation code here

	return TRUE;
}

//
// CreatePreviousOpenedFile()
// Creates the record file to store the filename of the 
// previously opened file for Continue feature in start
// screen to work properly.
// Returns a pos integer if successfull
//
int CPlannerDoc::CreatePreviousOpenedFile(CString Filename)
{
	// Local vars
	std::ofstream Output;

	Output.open("Previous.txt", std::ofstream::trunc);

	if (Output.fail())
	{
		AfxMessageBox(_T("Unable to open file"));
		return -1;
	}

	int Len = Filename.GetLength();

	if (Len == 0) return -2;

	for (int i = 0; i < Len; i++)
	{
		Output.put(Filename[i]);
	}
	
	Output.close();

	return 1;
}