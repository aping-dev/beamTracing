// beamDoc.h : interface of the CBeamDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_BEAMDOC_H__ACF9E126_CEF5_4A9E_BC43_78A6A30CB837__INCLUDED_)
#define AFX_BEAMDOC_H__ACF9E126_CEF5_4A9E_BC43_78A6A30CB837__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CBeamDoc : public CDocument
{
protected: // create from serialization only
	CBeamDoc();
	DECLARE_DYNCREATE(CBeamDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBeamDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBeamDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CBeamDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BEAMDOC_H__ACF9E126_CEF5_4A9E_BC43_78A6A30CB837__INCLUDED_)
