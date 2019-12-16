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
	isFound = false;		//设置窗口未找到
	isCapturing =false;		//还未开始截图
	cWnd = NULL;			//避免初始化错误
	_page = 0;				//从零开始计算页码
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
		this->SetDlgItemText(IDC_EDIT_PAGE,"10");			//缺省总页码为10
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
	GetDlgItemText(IDC_EDIT_BOOK_NAME,bkName,256);		//获取书名
	bookName.Format("%s",bkName);
	cWnd = cWnd->FindWindow("CReadWnd",bookName);		//第一个参数查找窗口类，第二个参数窗口标题
	if(!cWnd)
	{
		MessageBox("未找到京东读书窗口");
		isFound = false;
		_page = 0;
	}
	else
	{
		MessageBox("查找成功");
		CreateDirectory(bkName,NULL);						//创建一个以书名为名字的目录
		isFound = true;
	}
}

void CJdCaptureDlg::OnButtonCapture() 
{
	// TODO: Add your control notification handler code here
	if(isFound && cWnd)
	{
		if(!isCapturing)							//防止反复启动定时器
		{
			isCapturing = true;						//开始截图
			_totalPage = GetDlgItemInt(IDC_EDIT_PAGE);		//获取总页数
			cWnd->PostMessage(WM_SYSCOMMAND,SC_MAXIMIZE,0);		//还原待截图的窗口
			this->PostMessage(WM_SYSCOMMAND,SC_MINIMIZE,0);		//使本对话框最小化
			SetTimer(ID_TIMER_CAPTURE,TIME_ELAPSE,NULL);
		}
	}
	else
	{
		MessageBox("请先查找窗口");
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
		//达到设置的页数时停止
		if(_page == _totalPage)
		{
			this->OnButtonStop();
			return;
		}
		CV_OnCapture(cWnd,bookName,_page);
		cWnd->SendMessage(WM_KEYDOWN,VK_RIGHT,0);		//自动翻到下一页
		_page++;
	}
	CDialog::OnTimer(nIDEvent);
}

//窗口句柄，书名，页码
void OnCapture(CWnd* cWnd,CString filename,int page)
{
	CFile file;
	CString filePath;
	int BitPerPixel,width,height;
	CDC *pDC = cWnd->GetDC();
	BitPerPixel = pDC->GetDeviceCaps(BITSPIXEL);
	width = pDC->GetDeviceCaps(HORZRES);
	height = pDC->GetDeviceCaps(VERTRES);			//获取窗口参数，宽度，高度
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	
	CBitmap memBitmap, *oldmemBitmap;//建立和屏幕兼容的bitmap
	memBitmap.CreateCompatibleBitmap(pDC, width, height);
	
	oldmemBitmap = memDC.SelectObject(&memBitmap);//将memBitmap选入内存DC
	memDC.BitBlt(0, 0, width, height, pDC, 0, 0, SRCCOPY);//复制屏幕图像到内存DC
	
	//以下代码保存memDC中的位图到文件
	BITMAP bmp;
	memBitmap.GetBitmap(&bmp);//获得位图信息
    
	filePath.Format("%s%04d.bmp",filename,page);		//以书名作为位图文件名，并后接页码
	file.Open(filePath,CFile::modeCreate|CFile::modeReadWrite);		//打开文件
	
	//位图相关的设置参数，亘古不变
	BITMAPINFOHEADER bih = {0};//位图信息头
	bih.biBitCount = bmp.bmBitsPixel;//每个像素字节大小
	bih.biCompression = BI_RGB;
	bih.biHeight = bmp.bmHeight;//高度
	bih.biPlanes = 1;
	bih.biSize = sizeof(BITMAPINFOHEADER);
	bih.biSizeImage = bmp.bmWidthBytes * bmp.bmHeight;//图像数据大小
	bih.biWidth = bmp.bmWidth;//宽度
    
	BITMAPFILEHEADER bfh = {0};//位图文件头
	bfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);//到位图数据的偏移量
	bfh.bfSize = bfh.bfOffBits + bmp.bmWidthBytes * bmp.bmHeight;//文件总的大小
	bfh.bfType = (WORD)0x4d42;
    
	file.Write(&bfh,sizeof(BITMAPFILEHEADER));		//写入位图文件头
	file.Write(&bih,sizeof(BITMAPINFOHEADER));		//写入位图信息头
    
	byte * p = new byte[bmp.bmWidthBytes * bmp.bmHeight];		//申请内存保存位图数据
	
	GetDIBits(memDC.m_hDC, (HBITMAP) memBitmap.m_hObject, 0, height, p, 
        (LPBITMAPINFO) &bih, DIB_RGB_COLORS);					//获取位图数据
	
	file.Write(p,bmp.bmWidthBytes * bmp.bmHeight);				//写入位图数据
	
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

//响应按键消息,防止程序闪退
BOOL CJdCaptureDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)		//是否按了回车键
	{
		if(GetFocus() == GetDlgItem(IDC_EDIT_BOOK_NAME))				//焦点是否在IDC_EDIT_BOOK_NAME编辑框
		{
			OnButtonFindWindow();										//按了回车键后就可以调用查找窗口函数了，不用麻烦地移动鼠标了
		}
		return true;
	}
	return CDialog::PreTranslateMessage(pMsg);
}
//使用opencv重构截图和保存图片函数,可将截图保存为已压缩的jpeg,png等格式文件
void CV_OnCapture(CWnd* cWnd,CString filename,int page)
{
	CString filePath;
	char outname[256];
	int BitPerPixel,width,height;
	CDC *pDC = cWnd->GetDC();
	BitPerPixel = pDC->GetDeviceCaps(BITSPIXEL);
	width = pDC->GetDeviceCaps(HORZRES);
	height = pDC->GetDeviceCaps(VERTRES);			//获取窗口参数，宽度，高度
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	CBitmap memBitmap, *oldmemBitmap;//建立和屏幕兼容的bitmap
	memBitmap.CreateCompatibleBitmap(pDC, width, height);
	oldmemBitmap = memDC.SelectObject(&memBitmap);//将memBitmap选入内存DC
	memDC.BitBlt(0, 0, width, height, pDC, 0, 0, SRCCOPY);//复制屏幕图像到内存DC
	//以下代码保存memDC中的位图到文件
	BITMAP bmp;
	memBitmap.GetBitmap(&bmp);//获得位图信息
	//位图相关的设置参数，亘古不变
	BITMAPINFOHEADER bih = {0};//位图信息头
	bih.biBitCount = bmp.bmBitsPixel;//每个像素字节大小
	bih.biCompression = BI_RGB;
	bih.biHeight = bmp.bmHeight;//高度
	bih.biPlanes = 1;
	bih.biSize = sizeof(BITMAPINFOHEADER);
	bih.biSizeImage = bmp.bmWidthBytes * bmp.bmHeight;//图像数据大小
	bih.biWidth = bmp.bmWidth;//宽度
	filePath.Format("%s\\%s%04d.jpeg",filename,filename,page);		//以书名作为位图文件名，并后接页码
	strcpy(outname,filePath);							//outname保存文件名
	IplImage* image = cvCreateImage(cvSize(width,height),IPL_DEPTH_8U,bmp.bmWidthBytes/bmp.bmWidth);	//创建opencv图像
	GetDIBits(memDC.m_hDC, 
		(HBITMAP) memBitmap.m_hObject, 
		0,
		height, 
		image->imageData,										//神仙操作，让image的数据区域直接指向memDC的数据区域
        (LPBITMAPINFO) &bih, DIB_RGB_COLORS);					//获取位图数据
	cvFlip(image,NULL,0);										//图像上下颠倒了
	cvSaveImage(outname,image);									//保存图片
	cvReleaseImage(&image);
	memDC.SelectObject(oldmemBitmap);
	Sleep(20);													//延时20毫秒，可能避免漏截的情况
}