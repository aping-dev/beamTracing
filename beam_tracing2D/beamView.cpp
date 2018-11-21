// beamView.cpp : implementation of the CBeamView class
//

#include "stdafx.h"
#include "beam.h"

#include "beamDoc.h"
#include "beamView.h"
#include "beamT.h"
#include <windows.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBeamView

IMPLEMENT_DYNCREATE(CBeamView, CView)

BEGIN_MESSAGE_MAP(CBeamView, CView)
	//{{AFX_MSG_MAP(CBeamView)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

CBeamView::~CBeamView()
{
}

BOOL CBeamView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CBeamView drawing

void drawInit(CDC* pDC, const BeamTrace &beam)
{
	int mSize = 10;

	CBrush fillbrush, fillbrush1;
	CBrush *oldbrush;
	fillbrush.CreateSolidBrush(RGB(255, 0, 0));
	oldbrush = pDC->SelectObject(&fillbrush);
	pDC->Ellipse(beam.source.x - mSize, beam.source.y - mSize, beam.source.x + mSize, beam.source.y + mSize);
	
	fillbrush1.CreateSolidBrush(RGB(0, 255, 0));
	oldbrush = pDC->SelectObject(&fillbrush1);
	pDC->Ellipse(beam.listener.x - mSize, beam.listener.y - mSize, beam.listener.x + mSize, beam.listener.y + mSize);

	CPen newPen(PS_SOLID, 2, RGB(0, 0, 0));
    pDC->SelectObject(&newPen);

	for(int i=0; i<beam.walls.size(); i++)
	{
		pDC->MoveTo(beam.walls[i].p1.x, beam.walls[i].p1.y);
		pDC->LineTo(beam.walls[i].p2.x, beam.walls[i].p2.y);
	}
}

void drawUpdate(CDC* pDC, const vector< vector<Point> > &paths)
{
	//ÇúÏß
    CPen newPen(PS_SOLID, 1, RGB(255, 255, 0));
    pDC->SelectObject(&newPen);

	for(int i=0; i<paths.size(); i++)
	{
		pDC->MoveTo(paths[i][0].x, paths[i][0].y);

		for(int j=1; j<paths[i].size(); j++)
		{
			pDC->LineTo(paths[i][j].x, paths[i][j].y);
		}
	}
}

BeamTrace beam;
vector< vector<Point> > paths;

// CBeamView construction/destruction

CBeamView::CBeamView()
{
	// TODO: add construction code here
	beam.init();
	paths = beam.getPaths();
}

void CBeamView::OnDraw(CDC* pDC)
{
	CBeamDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here

	CRect  rcClient; 
    GetClientRect(&rcClient); 
	pDC->FillSolidRect(rcClient, RGB(255, 255, 255));

	/*
	CView  *pView;   
	pView=(CView*)((CFrameWnd*)AfxGetApp()->m_pMainWnd)->GetActiveView(); 
	pView->Invalidate();
	*/

	drawInit(pDC, beam);
	drawUpdate(pDC, paths);
}

void CBeamView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	beam.listener.x = point.x;
	beam.listener.y = point.y;
	paths = beam.getPaths();

	CDC* pDC = GetDC();
	OnDraw(pDC);

	CView::OnLButtonDown(nFlags, point);
}

void CBeamView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	CView::OnMouseMove(nFlags, point);
}

/////////////////////////////////////////////////////////////////////////////
// CBeamView printing

BOOL CBeamView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CBeamView::OnBeginPrinting(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}
void CBeamView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CBeamView diagnostics

#ifdef _DEBUG
void CBeamView::AssertValid() const
{
	CView::AssertValid();
}

void CBeamView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CBeamDoc* CBeamView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBeamDoc)));
	return (CBeamDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////



