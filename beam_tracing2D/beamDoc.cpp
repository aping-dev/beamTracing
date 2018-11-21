// beamDoc.cpp : implementation of the CBeamDoc class
//

#include "stdafx.h"
#include "beam.h"

#include "beamDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBeamDoc

IMPLEMENT_DYNCREATE(CBeamDoc, CDocument)

BEGIN_MESSAGE_MAP(CBeamDoc, CDocument)
	//{{AFX_MSG_MAP(CBeamDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBeamDoc construction/destruction

CBeamDoc::CBeamDoc()
{
	// TODO: add one-time construction code here

}

CBeamDoc::~CBeamDoc()
{
}

BOOL CBeamDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CBeamDoc serialization

void CBeamDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CBeamDoc diagnostics

#ifdef _DEBUG
void CBeamDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CBeamDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CBeamDoc commands
