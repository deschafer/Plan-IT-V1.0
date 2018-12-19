
// PlannerAppView.cpp : implementation of the CPlannerAppView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "PlannerApp.h"
#endif

#include "PlannerAppDoc.h"
#include "PlannerAppView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPlannerAppView

IMPLEMENT_DYNCREATE(CPlannerAppView, CView)

BEGIN_MESSAGE_MAP(CPlannerAppView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CPlannerAppView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CPlannerAppView construction/destruction

CPlannerAppView::CPlannerAppView()
{
	// TODO: add construction code here

}

CPlannerAppView::~CPlannerAppView()
{
}

BOOL CPlannerAppView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CPlannerAppView drawing

void CPlannerAppView::OnDraw(CDC* /*pDC*/)
{
	CPlannerAppDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CPlannerAppView printing


void CPlannerAppView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CPlannerAppView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CPlannerAppView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CPlannerAppView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CPlannerAppView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CPlannerAppView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CPlannerAppView diagnostics

#ifdef _DEBUG
void CPlannerAppView::AssertValid() const
{
	CView::AssertValid();
}

void CPlannerAppView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CPlannerAppDoc* CPlannerAppView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPlannerAppDoc)));
	return (CPlannerAppDoc*)m_pDocument;
}
#endif //_DEBUG


// CPlannerAppView message handlers
