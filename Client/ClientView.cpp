// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面 
// (“Fluent UI”)。该示例仅供参考，
// 用以补充《Microsoft 基础类参考》和 
// MFC C++ 库软件随附的相关电子文档。
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问  
// http://msdn.microsoft.com/officeui。
//
// 版权所有(C) Microsoft Corporation
// 保留所有权利。

// ClientView.cpp : CClientView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "Client.h"
#endif

#include "ClientDoc.h"
#include "ClientView.h"
#include "UserLoginDlg.h"
#include "AnalasisDlg.h"
#include "AnalasisOfflineDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CClientView

IMPLEMENT_DYNCREATE(CClientView, CView)

	BEGIN_MESSAGE_MAP(CClientView, CView)
		// 标准打印命令
		ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
		ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
		ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CClientView::OnFilePrintPreview)
		ON_WM_CONTEXTMENU()
		ON_WM_RBUTTONUP()
		ON_MESSAGE(WM_UIDISPLAY, &CClientView::OnUidisplay)
		ON_UPDATE_COMMAND_UI(ID_BUTTON_LOGIN, &CClientView::OnUpdateButtonLogin)
		ON_COMMAND(ID_BUTTON_LOGOUT, &CClientView::OnButtonLogout)
		ON_WM_SIZE()
		ON_WM_CLOSE()
		ON_WM_PAINT()
		ON_MESSAGE(WM_UISELECT, &CClientView::OnUiselect)
	END_MESSAGE_MAP()

// CClientView 构造/析构

CClientView::CClientView()
	: isDlgOpen(false)
	  ,pDlg(NULL)
	  ,isDlgLogin(true)
	  ,isSpecCompareDlg(false)
	  ,m_pCallPrintDlg(NULL)
	
{
	// TODO: 在此处添加构造代码
	TransProject=false;
}

CClientView::~CClientView()
{
	closesocket(m_sClient);
	WSACleanup();
}

BOOL CClientView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式
	cs.style &= ~WS_BORDER;
	return CView::PreCreateWindow(cs);
}

// CClientView 绘制

void CClientView::OnDraw(CDC* /*pDC*/)
{
	CClientDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码	
	GetClientRect(&rc);
}


// CClientView 打印


void CClientView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

void CClientView::CallPrint(void)
{
	OnFilePrint();
}
void CClientView::CallPrint(CWnd *pCallWnd)
{
	this->m_pCallPrintDlg = pCallWnd;
	OnFilePrint();
}

void CClientView::CallPrintPreview(void)
{
	OnFilePrintPreview();
}

void CClientView::CallPrintPreview(CWnd *pCallWnd)
{
	this->m_pCallPrintDlg = pCallWnd;
	OnFilePrintPreview();
}

BOOL CClientView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CClientView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: 添加额外的打印前进行的初始化过程
	if(!m_pCallPrintDlg)
	{
		m_pCallPrintDlg = pDlg;
	}
	::SendMessage(m_pCallPrintDlg->m_hWnd,WM_BEGIN_PRINTING,(WPARAM) pDC, (LPARAM) pInfo);
}

void CClientView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: 在此添加专用代码和/或调用基类
	::SendMessage(m_pCallPrintDlg->m_hWnd,WM_MY_PRINT,(WPARAM) pDC, (LPARAM) pInfo) ;
	CView::OnPrint(pDC, pInfo);
}

void CClientView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: 添加打印后进行的清理过程
	::SendMessage(m_pCallPrintDlg->m_hWnd,WM_END_PRINTING,(WPARAM) pDC, (LPARAM) pInfo);
}

void CClientView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CClientView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CClientView 诊断

#ifdef _DEBUG
void CClientView::AssertValid() const
{
	CView::AssertValid();
}

void CClientView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CClientDoc* CClientView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CClientDoc)));
	return (CClientDoc*)m_pDocument;
}
#endif //_DEBUG


// CClientView 消息处理程序


void CClientView::OnInitialUpdate()
{
	CView::OnInitialUpdate();	
	// TODO: 在此添加专用代码和/或调用基类
	
}

afx_msg LRESULT CClientView::OnUiselect(WPARAM wParam, LPARAM lParam)
{
	if(!wParam)
	{
		if(NULL!=pDlg)
		{
			CAnalasisDlg *pAnalasisDlg=(CAnalasisDlg *)pDlg;
			//pAnalasisDlg->ReleaseResource();
			pDlg->DestroyWindow();
			pDlg=NULL;
		}
		(this->GetParent())->ShowWindow(SW_HIDE);
		::PostMessage(this->m_hWnd,WM_UIDISPLAY,wParam,NULL);
	}
	else
	{
		(this->GetParent())->ShowWindow(SW_SHOW);
		if(NULL!=pDlg)
		{
			pDlg->DestroyWindow();
			pDlg=NULL;
			g_pCAnalasisDlg = NULL;
		}
		::PostMessage(this->m_hWnd,WM_UIDISPLAY,wParam,NULL);
	}
	return 0;
}

afx_msg LRESULT CClientView::OnUidisplay(WPARAM wParam, LPARAM lParam)
{
	if(!wParam)
	{
		CUserLoginDlg loginDlg;
		loginDlg.m_pParent=this;
		if(IDOK==loginDlg.DoModal())
		{
			m_UserName=loginDlg.m_UserName;
			m_PassWord=loginDlg.m_PassWord;
			m_sClient =loginDlg.m_sock;
			::PostMessage(this->m_hWnd,WM_UISELECT,1,NULL);
		}
		m_pCallPrintDlg = pDlg;
	}
	else
	{
		CAnalasisDlg *pAnalasisDlg=new CAnalasisDlg(this);
		pAnalasisDlg->m_UserName=m_UserName;
		pAnalasisDlg->m_sock = m_sClient;
		m_UserName=L"";
	    pAnalasisDlg->Create(IDD_ANALASIS,this);
	    pAnalasisDlg->ShowWindow(SW_SHOW);
	    pAnalasisDlg->MoveWindow(rc); 
        pDlg=pAnalasisDlg;
	}
	return 0;
}


void CClientView::OnUpdateButtonLogin(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(isDlgLogin);
}

void CClientView::OnUpdateButtonNormspecsend(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
//	pCmdUI->Enable(isDlgUISelect);
}


void CClientView::OnButtonLogout()
{
	// TODO: 在此添加命令处理程序代码
	((CAnalasisDlg*)(this->pDlg))->ReleaseResource();
	OnUiselect(0, NULL);
}


void CClientView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	GetClientRect(&rc);
}


void CClientView::ConnectServer(void)
{
	OnUidisplay( 1, NULL);
	return ;
}


void CClientView::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	pDlg->DestroyWindow();
	CView::OnClose();
}


void CClientView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CView::OnPaint()
	GetClientRect(&rc);
	CDC memDC ;
	CBitmap memBitmap ;
	CBitmap* oldBitmap ; // bitmap originally found in CMemDC
	memDC.CreateCompatibleDC(&dc) ;
	memBitmap.CreateCompatibleBitmap(&dc, rc.Width(), rc.Height()) ;
	oldBitmap = (CBitmap *)memDC.SelectObject(&memBitmap) ;
	memBitmap.DeleteObject(); 
	memDC.FillSolidRect(0,0,rc.Width(),rc.Height(),GetSysColor(COLOR_BTNFACE)); 
	if(NULL!=pDlg)
	{
		pDlg->MoveWindow(rc);
		memDC.BitBlt(0,0,rc.Width(),rc.Height(),pDlg->GetWindowDC(),0,0,SRCCOPY);
	}

	dc.BitBlt(0,0,rc.Width(), rc.Height(),&memDC,0,0,SRCCOPY); //拷贝内存DC到屏幕
	//memDC.SelectObject(oldBitmap) ;
	memDC.DeleteDC(); 
}