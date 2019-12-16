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

//��������ͼ�����ݱ���Ϊ�ļ�
BOOL SaveBitmapToFile(HDC hDc, CBitmap & bitmap, LPCTSTR lpszFileName)
{
	BOOL ret = TRUE;
	BITMAP btm;
	bitmap.GetBitmap(&btm);
	//���������ڲ鿴CBitmap�������Ϣ�����ص���Ϣ�����pBitMapָ���BITMAP�ṹ��
	DWORD size = btm.bmWidthBytes * btm.bmHeight;
	//�����ڴ�
	HGLOBAL hMem = GlobalAlloc(GMEM_FIXED|GMEM_ZEROINIT, size);
	if(hMem == NULL)
	{
		return FALSE;
	}
 
	//���� �����ڴ�
	LPSTR lpData = (LPSTR)GlobalLock(hMem);
	//����λͼͷ�ṹ����ʼ��
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
 
	//��λͼ�е�������bits��ʽ����lpDataָ���λͼ������
	if(GetDIBits(hDc, bitmap, 0, bih.biHeight, lpData,(BITMAPINFO *)&bih, DIB_RGB_COLORS) == 0)
	{
		GlobalFree(hMem);
		return FALSE;
	}
 
	//λͼ�ļ��ṹ��ʼ��
	BITMAPFILEHEADER bfh;
	bfh.bfType = ((WORD)('M'<< 8)|'B');
	bfh.bfReserved1 = 0;
	bfh.bfReserved2 = 0;
	bfh.bfSize = sizeof(bfh) + size;
	bfh.bfOffBits = sizeof(bfh);
 
	//����λͼ�ļ���д������
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
 
	//�ͷ��ڴ�
	GlobalFree(hMem);
	return ret;
}
//�����ã���ͼ
void Test_captureScreenMFC()
{
    CDC *pDesktopDC = CDC::FromHandle(::GetDC(NULL));//��ȡ��ǰ������ĻDC
    int width = pDesktopDC->GetDeviceCaps(HORZRES);
    int height = pDesktopDC->GetDeviceCaps(VERTRES);

    CBitmap    memBmp;
    CDC memDC;
    memBmp.CreateCompatibleBitmap(pDesktopDC, width, height);
    memDC.CreateCompatibleDC(pDesktopDC);
    memDC.SelectObject(&memBmp);  //��memBitmapѡ���ڴ�DC

    memDC.BitBlt(0, 0, width, height, pDesktopDC, 0, 0, SRCCOPY);//������Ļͼ���ڴ�DC
    
    BITMAP bmp;
    memBmp.GetBitmap(&bmp); //CBitmap��BITMAP

    BITMAPINFOHEADER m_bihScreen;  
    ZeroMemory(&m_bihScreen, sizeof(BITMAPINFOHEADER));//λͼ��Ϣͷ
    m_bihScreen.biBitCount = bmp.bmBitsPixel;//ÿ�������ֽڴ�С
    m_bihScreen.biCompression = BI_RGB;
    m_bihScreen.biHeight = bmp.bmHeight;//�߶�
    m_bihScreen.biPlanes = 1;
    m_bihScreen.biSize = sizeof(BITMAPINFOHEADER);
    m_bihScreen.biSizeImage = bmp.bmWidthBytes * bmp.bmHeight;//ͼ�����ݴ�С
    m_bihScreen.biWidth = bmp.bmWidth;//���

    byte *m_pbmScreenData = new byte[bmp.bmWidthBytes * bmp.bmHeight];
    GetDIBits(/*pDesktopDC->m_hDC*/memDC.m_hDC, (HBITMAP)memBmp.m_hObject, 0, height, m_pbmScreenData,  
        (LPBITMAPINFO) &m_bihScreen, DIB_RGB_COLORS);//��ȡλͼ����  ��BITMAP���ڴ�����
/*
	//û��������ṹ�����ﶨ���
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
