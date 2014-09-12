// PrintFrame.cpp : implementation file
//

#include "stdafx.h"
#include "PrintFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPrintFrame

IMPLEMENT_DYNCREATE(CPrintFrame, CFrameWnd)

CPrintFrame::CPrintFrame()
{
}
CPrintFrame::CPrintFrame(CWnd* pOld)
{
    m_pOldWnd= pOld;
    if(!Create(NULL,L"打印预览", WS_OVERLAPPEDWINDOW|FWS_ADDTOTITLE,CRect(200,200,500,500)))
        TRACE0("Failed to create view window＼n");
}
CPrintFrame::~CPrintFrame()
{
	    m_pOldWnd->ShowWindow(SW_SHOW);
}


BEGIN_MESSAGE_MAP(CPrintFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CPrintFrame)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_WM_CLOSE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPrintFrame message handlers

int CPrintFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
    if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

    CCreateContext context;
    context.m_pNewViewClass = RUNTIME_CLASS(CPrintView);
    context.m_pCurrentFrame = this;
    context.m_pCurrentDoc = NULL;
    context.m_pLastView = NULL;
    m_pView = STATIC_DOWNCAST(CPrintView, CreateView(&context)); 
    if(m_pView != NULL)
    {
        m_pView->ShowWindow(SW_SHOW);
        SetActiveView(m_pView);
    }
    SetIcon(m_pOldWnd->GetIcon(FALSE),FALSE);
	SetIcon(m_pOldWnd->GetIcon(TRUE),TRUE);
	ShowWindow(SW_MAXIMIZE);
	CWinApp* pApp=AfxGetApp();
	m_pOldFram = pApp->m_pMainWnd;
	pApp->m_pMainWnd=this;
	m_pView->OnFilePrintPreview();
    //m_pView->SendMessage(WM_COMMAND, ID_FILE_PRINT);//直接打印
    m_pOldWnd->ShowWindow(SW_HIDE);
    return 0;
}



void CPrintFrame::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CWinApp* pApp=AfxGetApp();
	pApp->m_pMainWnd=m_pOldFram;
	CFrameWnd::OnClose();
}
