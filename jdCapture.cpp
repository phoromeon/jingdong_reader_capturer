// jdCapture.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "jdCapture.h"
#include "jdCaptureDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CJdCaptureApp

BEGIN_MESSAGE_MAP(CJdCaptureApp, CWinApp)
	//{{AFX_MSG_MAP(CJdCaptureApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CJdCaptureApp construction

CJdCaptureApp::CJdCaptureApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CJdCaptureApp object

CJdCaptureApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CJdCaptureApp initialization

BOOL CJdCaptureApp::InitInstance()
{
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

	CJdCaptureDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

//这个嘛，将截图的内容保存为文件
BOOL SaveBitmapToFile(HDC hDc, CBitmap & bitmap, LPCTSTR lpszFileName)
{
	BOOL ret = TRUE;
	BITMAP btm;
	bitmap.GetBitmap(&btm);
	//本函数用于查看CBitmap对象的信息。返回的信息存放在pBitMap指向的BITMAP结构中
	DWORD size = btm.bmWidthBytes * btm.bmHeight;
	//分配内存
	HGLOBAL hMem = GlobalAlloc(GMEM_FIXED|GMEM_ZEROINIT, size);
	if(hMem == NULL)
	{
		return FALSE;
	}
 
	//加锁 保护内存
	LPSTR lpData = (LPSTR)GlobalLock(hMem);
	//定义位图头结构并初始化
	BITMAPINFOHEADER bih;
	bih.biSize = sizeof(BITMAPINFOHEADER);
	bih.biWidth = btm.bmWidth;
	bih.biHeight = btm.bmHeight;
	bih.biPlanes = 1;
	bih.biBitCount = btm.bmBitsPixel;
	bih.biCompression = 0;
	bih.biSizeImage = size;
	bih.biXPelsPerMeter = 0;
	bih.biYPelsPerMeter = 0;
	bih.biClrUsed = 0;
	bih.biClrImportant = 0;
 
	//将位图中的数据以bits形式放入lpData指向的位图数组中
	if(GetDIBits(hDc, bitmap, 0, bih.biHeight, lpData,(BITMAPINFO *)&bih, DIB_RGB_COLORS) == 0)
	{
		GlobalFree(hMem);
		return FALSE;
	}
 
	//位图文件结构初始化
	BITMAPFILEHEADER bfh;
	bfh.bfType = ((WORD)('M'<< 8)|'B');
	bfh.bfReserved1 = 0;
	bfh.bfReserved2 = 0;
	bfh.bfSize = sizeof(bfh) + size;
	bfh.bfOffBits = sizeof(bfh);
 
	//创建位图文件并写入数据
	CFile bf;
	if(bf.Open(lpszFileName, CFile::modeCreate | CFile::modeWrite))
	{
		bf.WriteHuge(&bfh, sizeof(BITMAPFILEHEADER));
		bf.WriteHuge(&bih, sizeof(BITMAPINFOHEADER));
		bf.WriteHuge(lpData, size);
		bf.Close();
	}
	else
	{
		ret = FALSE;
	}
 
	//释放内存
	GlobalFree(hMem);
	return ret;
}
//测试用，截图
void Test_captureScreenMFC()
{
    CDC *pDesktopDC = CDC::FromHandle(::GetDC(NULL));//获取当前整个屏幕DC
    int width = pDesktopDC->GetDeviceCaps(HORZRES);
    int height = pDesktopDC->GetDeviceCaps(VERTRES);

    CBitmap    memBmp;
    CDC memDC;
    memBmp.CreateCompatibleBitmap(pDesktopDC, width, height);
    memDC.CreateCompatibleDC(pDesktopDC);
    memDC.SelectObject(&memBmp);  //将memBitmap选入内存DC

    memDC.BitBlt(0, 0, width, height, pDesktopDC, 0, 0, SRCCOPY);//复制屏幕图像到内存DC
    
    BITMAP bmp;
    memBmp.GetBitmap(&bmp); //CBitmap到BITMAP

    BITMAPINFOHEADER m_bihScreen;  
    ZeroMemory(&m_bihScreen, sizeof(BITMAPINFOHEADER));//位图信息头
    m_bihScreen.biBitCount = bmp.bmBitsPixel;//每个像素字节大小
    m_bihScreen.biCompression = BI_RGB;
    m_bihScreen.biHeight = bmp.bmHeight;//高度
    m_bihScreen.biPlanes = 1;
    m_bihScreen.biSize = sizeof(BITMAPINFOHEADER);
    m_bihScreen.biSizeImage = bmp.bmWidthBytes * bmp.bmHeight;//图像数据大小
    m_bihScreen.biWidth = bmp.bmWidth;//宽度

    byte *m_pbmScreenData = new byte[bmp.bmWidthBytes * bmp.bmHeight];
    GetDIBits(/*pDesktopDC->m_hDC*/memDC.m_hDC, (HBITMAP)memBmp.m_hObject, 0, height, m_pbmScreenData,  
        (LPBITMAPINFO) &m_bihScreen, DIB_RGB_COLORS);//获取位图数据  从BITMAP到内存数据
/*
	//没看懂这个结构在哪里定义的
    TwoDimesionArray<RGBQUAD> colors(width,height);
    for (int j=0;j<height;++j)
    {
        for (int i=0;i<width;++i)
        {
            byte *p=m_pbmScreenData+(height-1-j)*bmp.bmWidthBytes+i*4;
            RGBQUAD c={int(*p),int(*(p+1)),int(*(p+2))};
            colors.SetValue(i,j,c);
        }
    }

    ImageIO writer;
    writer.WriteBmp(colors,"D:\\z.bmp");
*/
}
