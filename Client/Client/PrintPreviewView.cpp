// PrintPreviewView.cpp: implementation of the CPrintPreviewView class.
#include "stdafx.h"
#include "PrintPreviewView.h"
#include "PrintFrame.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//////////////////////////////////////////
IMPLEMENT_DYNCREATE(CPrintPreviewView, CPreviewView)
CPrintPreviewView::CPrintPreviewView()
{
}
CPrintPreviewView::~CPrintPreviewView()
{
}

BEGIN_MESSAGE_MAP(CPrintPreviewView, CPreviewView)
	ON_COMMAND(AFX_ID_PREVIEW_CLOSE, OnPreviewClose)
	ON_COMMAND(AFX_ID_PREVIEW_PRINT, OnPreviewPrint)
END_MESSAGE_MAP()

void CPrintPreviewView::OnDraw(CDC *pDC)
{
	CPreviewView::OnDraw(pDC);
	m_pToolBar->PostMessage(WM_IDLEUPDATECMDUI, (WPARAM)TRUE);// 控制条的命令状态更新
}

void CPrintPreviewView::OnEndPrintPreview(CDC* pDC, CPrintInfo* pInfo, POINT point, CPreviewView* pView) 
{
	CPreviewView::OnEndPrintPreview(pDC, pInfo, point, pView);
}

void CPrintPreviewView::OnPreviewClose()
{
	CPrintFrame* pf=(CPrintFrame*)::AfxGetMainWnd();
	CWinApp *pApp=AfxGetApp();      
	pApp->m_pMainWnd=pf->m_pOldFram;
	pf->m_pOldFram->ShowWindow(SW_SHOW);
	pf->DestroyWindow();
}

void CPrintPreviewView::OnPreviewPrint()
{
	m_pPrintView->SendMessage(WM_COMMAND, ID_FILE_PRINT);
}