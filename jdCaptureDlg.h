// jdCaptureDlg.h : header file
//

#if !defined(AFX_JDCAPTUREDLG_H__95779668_5336_4E94_9E37_540F940CE87C__INCLUDED_)
#define AFX_JDCAPTUREDLG_H__95779668_5336_4E94_9E37_540F940CE87C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define ID_TIMER_CAPTURE 1
#define TIME_ELAPSE 500
/////////////////////////////////////////////////////////////////////////////
// CJdCaptureDlg dialog

class CJdCaptureDlg : public CDialog
{
// Construction
public:
	CJdCaptureDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CJdCaptureDlg)
	enum { IDD = IDD_JDCAPTURE_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CJdCaptureDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CJdCaptureDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonFindWindow();
	afx_msg void OnButtonCapture();
	afx_msg void OnChangeEditBookName();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnButtonStop();
	afx_msg BOOL PreTranslateMessage(MSG* );
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	bool isFound,isCapturing;
	CWnd* cWnd;
	CString bookName;		//
	int _page;				//Ò³Êý¼ÆÊý
	int _totalPage;
};

void OnCapture(CWnd* cWnd,CString filename,int page);
void CV_OnCapture(CWnd* cWnd,CString filename,int page);
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.


#endif // !defined(AFX_JDCAPTUREDLG_H__95779668_5336_4E94_9E37_540F940CE87C__INCLUDED_)
