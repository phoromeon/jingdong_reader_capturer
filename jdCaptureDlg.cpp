// jdCaptureDlg.cpp : implementation file
//

#include "stdafx.h"
#include "jdCapture.h"
#include "jdCaptureDlg.h"
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CJdCaptureDlg dialog

CJdCaptureDlg::CJdCaptureDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CJdCaptureDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CJdCaptureDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	isFound = false;		//���ô���δ�ҵ�
	isCapturing =false;		//��δ��ʼ��ͼ
	cWnd = NULL;			//�����ʼ������
	_page = 0;				//���㿪ʼ����ҳ��
	_totalPage = 10;
}

void CJdCaptureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CJdCaptureDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CJdCaptureDlg, CDialog)
	//{{AFX_MSG_MAP(CJdCaptureDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_FIND_WINDOW, OnButtonFindWindow)
	ON_BN_CLICKED(IDC_BUTTON_CAPTURE, OnButtonCapture)
	ON_EN_CHANGE(IDC_EDIT_BOOK_NAME, OnChangeEditBookName)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_STOP, OnButtonStop)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CJdCaptureDlg message handlers

BOOL CJdCaptureDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CJdCaptureDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
		this->SetDlgItemText(IDC_EDIT_PAGE,"10");			//ȱʡ��ҳ��Ϊ10
		GetDlgItem(IDC_EDIT_BOOK_NAME)->SetFocus();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CJdCaptureDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CJdCaptureDlg::OnButtonFindWindow() 
{
	// TODO: Add your control notification handler code here
	//CWnd* cWnd;
	char bkName[256];
	GetDlgItemText(IDC_EDIT_BOOK_NAME,bkName,256);		//��ȡ����
	bookName.Format("%s",bkName);
	cWnd = cWnd->FindWindow("CReadWnd",bookName);		//��һ���������Ҵ����࣬�ڶ����������ڱ���
	if(!cWnd)
	{
		MessageBox("δ�ҵ��������鴰��");
		isFound = false;
		_page = 0;
	}
	else
	{
		MessageBox("���ҳɹ�");
		CreateDirectory(bkName,NULL);						//����һ��������Ϊ���ֵ�Ŀ¼
		isFound = true;
	}
}

void CJdCaptureDlg::OnButtonCapture() 
{
	// TODO: Add your control notification handler code here
	if(isFound && cWnd)
	{
		if(!isCapturing)							//��ֹ����������ʱ��
		{
			isCapturing = true;						//��ʼ��ͼ
			_totalPage = GetDlgItemInt(IDC_EDIT_PAGE);		//��ȡ��ҳ��
			cWnd->PostMessage(WM_SYSCOMMAND,SC_MAXIMIZE,0);		//��ԭ����ͼ�Ĵ���
			this->PostMessage(WM_SYSCOMMAND,SC_MINIMIZE,0);		//ʹ���Ի�����С��
			SetTimer(ID_TIMER_CAPTURE,TIME_ELAPSE,NULL);
		}
	}
	else
	{
		MessageBox("���Ȳ��Ҵ���");
	}
}

void CJdCaptureDlg::OnChangeEditBookName() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	
}

void CJdCaptureDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(ID_TIMER_CAPTURE == nIDEvent)
	{
		//�ﵽ���õ�ҳ��ʱֹͣ
		if(_page == _totalPage)
		{
			this->OnButtonStop();
			return;
		}
		CV_OnCapture(cWnd,bookName,_page);
		cWnd->SendMessage(WM_KEYDOWN,VK_RIGHT,0);		//�Զ�������һҳ
		_page++;
	}
	CDialog::OnTimer(nIDEvent);
}

//���ھ����������ҳ��
void OnCapture(CWnd* cWnd,CString filename,int page)
{
	CFile file;
	CString filePath;
	int BitPerPixel,width,height;
	CDC *pDC = cWnd->GetDC();
	BitPerPixel = pDC->GetDeviceCaps(BITSPIXEL);
	width = pDC->GetDeviceCaps(HORZRES);
	height = pDC->GetDeviceCaps(VERTRES);			//��ȡ���ڲ�������ȣ��߶�
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	
	CBitmap memBitmap, *oldmemBitmap;//��������Ļ���ݵ�bitmap
	memBitmap.CreateCompatibleBitmap(pDC, width, height);
	
	oldmemBitmap = memDC.SelectObject(&memBitmap);//��memBitmapѡ���ڴ�DC
	memDC.BitBlt(0, 0, width, height, pDC, 0, 0, SRCCOPY);//������Ļͼ���ڴ�DC
	
	//���´��뱣��memDC�е�λͼ���ļ�
	BITMAP bmp;
	memBitmap.GetBitmap(&bmp);//���λͼ��Ϣ
    
	filePath.Format("%s%04d.bmp",filename,page);		//��������Ϊλͼ�ļ����������ҳ��
	file.Open(filePath,CFile::modeCreate|CFile::modeReadWrite);		//���ļ�
	
	//λͼ��ص����ò�����ب�Ų���
	BITMAPINFOHEADER bih = {0};//λͼ��Ϣͷ
	bih.biBitCount = bmp.bmBitsPixel;//ÿ�������ֽڴ�С
	bih.biCompression = BI_RGB;
	bih.biHeight = bmp.bmHeight;//�߶�
	bih.biPlanes = 1;
	bih.biSize = sizeof(BITMAPINFOHEADER);
	bih.biSizeImage = bmp.bmWidthBytes * bmp.bmHeight;//ͼ�����ݴ�С
	bih.biWidth = bmp.bmWidth;//���
    
	BITMAPFILEHEADER bfh = {0};//λͼ�ļ�ͷ
	bfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);//��λͼ���ݵ�ƫ����
	bfh.bfSize = bfh.bfOffBits + bmp.bmWidthBytes * bmp.bmHeight;//�ļ��ܵĴ�С
	bfh.bfType = (WORD)0x4d42;
    
	file.Write(&bfh,sizeof(BITMAPFILEHEADER));		//д��λͼ�ļ�ͷ
	file.Write(&bih,sizeof(BITMAPINFOHEADER));		//д��λͼ��Ϣͷ
    
	byte * p = new byte[bmp.bmWidthBytes * bmp.bmHeight];		//�����ڴ汣��λͼ����
	
	GetDIBits(memDC.m_hDC, (HBITMAP) memBitmap.m_hObject, 0, height, p, 
        (LPBITMAPINFO) &bih, DIB_RGB_COLORS);					//��ȡλͼ����
	
	file.Write(p,bmp.bmWidthBytes * bmp.bmHeight);				//д��λͼ����
	
	delete [] p;
	
	file.Close();
	
	memDC.SelectObject(oldmemBitmap);
}

void CJdCaptureDlg::OnButtonStop() 
{
	// TODO: Add your control notification handler code here
	KillTimer(ID_TIMER_CAPTURE);
	_page = 0;
	if(isCapturing)
	{
		isCapturing = false;
		isFound = false;
	}
}

//��Ӧ������Ϣ,��ֹ��������
BOOL CJdCaptureDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)		//�Ƿ��˻س���
	{
		if(GetFocus() == GetDlgItem(IDC_EDIT_BOOK_NAME))				//�����Ƿ���IDC_EDIT_BOOK_NAME�༭��
		{
			OnButtonFindWindow();										//���˻س�����Ϳ��Ե��ò��Ҵ��ں����ˣ������鷳���ƶ������
		}
		return true;
	}
	return CDialog::PreTranslateMessage(pMsg);
}
//ʹ��opencv�ع���ͼ�ͱ���ͼƬ����,�ɽ���ͼ����Ϊ��ѹ����jpeg,png�ȸ�ʽ�ļ�
void CV_OnCapture(CWnd* cWnd,CString filename,int page)
{
	CString filePath;
	char outname[256];
	int BitPerPixel,width,height;
	CDC *pDC = cWnd->GetDC();
	BitPerPixel = pDC->GetDeviceCaps(BITSPIXEL);
	width = pDC->GetDeviceCaps(HORZRES);
	height = pDC->GetDeviceCaps(VERTRES);			//��ȡ���ڲ�������ȣ��߶�
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	CBitmap memBitmap, *oldmemBitmap;//��������Ļ���ݵ�bitmap
	memBitmap.CreateCompatibleBitmap(pDC, width, height);
	oldmemBitmap = memDC.SelectObject(&memBitmap);//��memBitmapѡ���ڴ�DC
	memDC.BitBlt(0, 0, width, height, pDC, 0, 0, SRCCOPY);//������Ļͼ���ڴ�DC
	//���´��뱣��memDC�е�λͼ���ļ�
	BITMAP bmp;
	memBitmap.GetBitmap(&bmp);//���λͼ��Ϣ
	//λͼ��ص����ò�����ب�Ų���
	BITMAPINFOHEADER bih = {0};//λͼ��Ϣͷ
	bih.biBitCount = bmp.bmBitsPixel;//ÿ�������ֽڴ�С
	bih.biCompression = BI_RGB;
	bih.biHeight = bmp.bmHeight;//�߶�
	bih.biPlanes = 1;
	bih.biSize = sizeof(BITMAPINFOHEADER);
	bih.biSizeImage = bmp.bmWidthBytes * bmp.bmHeight;//ͼ�����ݴ�С
	bih.biWidth = bmp.bmWidth;//���
	filePath.Format("%s\\%s%04d.jpeg",filename,filename,page);		//��������Ϊλͼ�ļ����������ҳ��
	strcpy(outname,filePath);							//outname�����ļ���
	IplImage* image = cvCreateImage(cvSize(width,height),IPL_DEPTH_8U,bmp.bmWidthBytes/bmp.bmWidth);	//����opencvͼ��
	GetDIBits(memDC.m_hDC, 
		(HBITMAP) memBitmap.m_hObject, 
		0,
		height, 
		image->imageData,										//���ɲ�������image����������ֱ��ָ��memDC����������
        (LPBITMAPINFO) &bih, DIB_RGB_COLORS);					//��ȡλͼ����
	cvFlip(image,NULL,0);										//ͼ�����µߵ���
	cvSaveImage(outname,image);									//����ͼƬ
	cvReleaseImage(&image);
	memDC.SelectObject(oldmemBitmap);
	Sleep(20);													//��ʱ20���룬���ܱ���©�ص����
}