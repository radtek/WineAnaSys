// PrintView.cpp : implementation file
//

#include "stdafx.h"
#include "PrintPreviewView.h"
#include "PrintView.h"
#include "PrintFrame.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPrintView

IMPLEMENT_DYNCREATE(CPrintView, CScrollView)

CPrintView::CPrintView()
{
	m_nMapMode = MM_TEXT;
}

CPrintView::~CPrintView()
{
}


BEGIN_MESSAGE_MAP(CPrintView, CScrollView)

	ON_COMMAND(ID_FILE_PRINT,CView::OnFilePrint)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPrintView drawing

void CPrintView::OnInitialUpdate()
{
	SetScrollSizes(MM_LOENGLISH,CSize(1920,1080));
	CScrollView::OnInitialUpdate();

	//CSize sizeTotal;
	// TODO: calculate the total size of this view
	//sizeTotal.cx = sizeTotal.cy = 100;
	//SetScrollSizes(MM_TEXT, sizeTotal);

	
}

void CPrintView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CPrintView diagnostics

#ifdef _DEBUG
void CPrintView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CPrintView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPrintView message handlers

BOOL CPrintView::OnPreparePrinting(CPrintInfo* pInfo) 
{
	/*********************加入设置打印机分辨率代码***********************/
	/*PRINTDLG pd;  //该结构包含打印对话框中的所有信息
	LPDEVMODE  lpDevMode; 
	if(AfxGetApp()->GetPrinterDeviceDefaults(&pd)) //获得默认的打印机的信息
	{
		lpDevMode=(LPDEVMODE)GlobalLock(pd.hDevMode); 
		if(lpDevMode)
		{   
			lpDevMode->dmPaperSize=DMPAPER_A4;    //将打印纸设置为A4
			// lpDevMode->dmOrientation=DMORIENT_LANDSCAPE; //将打印机设置为横向打印。
			lpDevMode->dmOrientation=DMORIENT_PORTRAIT;   //将打印机设置为纵向打印。
			lpDevMode->dmPrintQuality=900;   //打印分辨率为600dpi
		}
		GlobalUnlock(pd.hDevMode);
	}*/
    return DoPreparePrinting(pInfo);
}



void CPrintView::OnFilePrintPreview( )
{
    CPrintPreviewState* pState = new CPrintPreviewState;
    pState->lpfnCloseProc =_AfxMyPreviewCloseProc;//设置打印预览窗口关闭时的调用函数；
    if(!DoPrintPreview(AFX_IDD_PREVIEW_TOOLBAR, this,RUNTIME_CLASS(CPrintPreviewView), pState))
    {
        TRACE0("Error: DoPrintPreview failed.＼n");
        AfxMessageBox(AFX_IDP_COMMAND_FAILURE);
        delete pState;
    }
}
BOOL CALLBACK _AfxMyPreviewCloseProc(CFrameWnd* pFrameWnd)
{
    ASSERT_VALID(pFrameWnd);
    CPrintPreviewView* pView = (CPrintPreviewView*) pFrameWnd->GetDlgItem(AFX_IDW_PANE_FIRST);
    ASSERT_KINDOF(CPreviewView, pView);
    pView->OnPreviewClose();
    return FALSE;
}

void CPrintView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: 在此添加专用代码和/或调用基类

	CPrintFrame *pCPrintFrame = (CPrintFrame *)this->GetParent();
	::SendMessage(pCPrintFrame->m_pOldWnd->m_hWnd,WM_BEGIN_PRINTING,(WPARAM) pDC, (LPARAM) pInfo);
	CScrollView::OnBeginPrinting(pDC, pInfo);
}

void CPrintView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: 在此添加专用代码和/或调用基类
	CPrintFrame *pCPrintFrame = (CPrintFrame *)this->GetParent();
	::SendMessage(pCPrintFrame->m_pOldWnd->m_hWnd,WM_MY_PRINT,(WPARAM) pDC, (LPARAM) pInfo) ;
	CScrollView::OnPrint(pDC, pInfo);
}

void CPrintView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: 在此添加专用代码和/或调用基类
	CPrintFrame *pCPrintFrame = (CPrintFrame *)this->GetParent();
	::SendMessage(pCPrintFrame->m_pOldWnd->m_hWnd,WM_END_PRINTING,(WPARAM) pDC, (LPARAM) pInfo);
	CScrollView::OnEndPrinting(pDC, pInfo);
}


void CPrintView::OnEndPrintPreview(CDC* pDC, CPrintInfo* pInfo, POINT point, CPreviewView* pView)
{
	// TODO: 在此添加专用代码和/或调用基类

	CScrollView::OnEndPrintPreview(pDC, pInfo, point, pView);
}
