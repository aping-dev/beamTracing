// beam.h : main header file for the BEAM application
//

#if !defined(AFX_BEAM_H__3DA706BC_FB1C_4008_AB35_DF1D25AE5B5B__INCLUDED_)
#define AFX_BEAM_H__3DA706BC_FB1C_4008_AB35_DF1D25AE5B5B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CBeamApp:
// See beam.cpp for the implementation of this class
//

class CBeamApp : public CWinApp
{
public:
	CBeamApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBeamApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CBeamApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BEAM_H__3DA706BC_FB1C_4008_AB35_DF1D25AE5B5B__INCLUDED_)
