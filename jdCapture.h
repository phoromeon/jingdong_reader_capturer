// jdCapture.h : main header file for the JDCAPTURE application
//

#if !defined(AFX_JDCAPTURE_H__26D9CF77_3310_4633_B12A_48604D519AFF__INCLUDED_)
#define AFX_JDCAPTURE_H__26D9CF77_3310_4633_B12A_48604D519AFF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CJdCaptureApp:
// See jdCapture.cpp for the implementation of this class
//

class CJdCaptureApp : public CWinApp
{
public:
	CJdCaptureApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CJdCaptureApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CJdCaptureApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_JDCAPTURE_H__26D9CF77_3310_4633_B12A_48604D519AFF__INCLUDED_)
