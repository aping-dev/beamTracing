// beamView.h : interface of the CBeamView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_BEAMVIEW_H__0CCF7E41_6126_433C_BB00_A760645A9DDE__INCLUDED_)
#define AFX_BEAMVIEW_H__0CCF7E41_6126_433C_BB00_A760645A9DDE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CBeamView : public CView
{
protected: // create from serialization only
	CBeamView();
	DECLARE_DYNCREATE(CBeamView)

// Attributes
public:
	CBeamDoc* GetDocument();
	

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBeamView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBeamView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CBeamView)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in beamView.cpp
inline CBeamDoc* CBeamView::GetDocument()
   { return (CBeamDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BEAMVIEW_H__0CCF7E41_6126_433C_BB00_A760645A9DDE__INCLUDED_)
