// ��� MFC ʾ��Դ������ʾ���ʹ�� MFC Microsoft Office Fluent �û����� 
// (��Fluent UI��)����ʾ�������ο���
// ���Բ��䡶Microsoft ������ο����� 
// MFC C++ ������渽����ص����ĵ���
// ���ơ�ʹ�û�ַ� Fluent UI ����������ǵ����ṩ�ġ�
// ��Ҫ�˽��й� Fluent UI ��ɼƻ�����ϸ��Ϣ�������  
// http://msdn.microsoft.com/officeui��
//
// ��Ȩ����(C) Microsoft Corporation
// ��������Ȩ����

// ClientView.cpp : CClientView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "Client.h"
#endif

#include "ClientDoc.h"
#include "ClientView.h"
#include "UserLoginDlg.h"
#include "AnalasisDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CClientView

IMPLEMENT_DYNCREATE(CClientView, CView)

	BEGIN_MESSAGE_MAP(CClientView, CView)
		// ��׼��ӡ����
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

// CClientView ����/����

CClientView::CClientView()
	: isDlgOpen(false)
	  ,pDlg(NULL)
	  ,isDlgLogin(true)
	  ,isSpecCompareDlg(false)
	  ,m_pCallPrintDlg(NULL)
	
{
	// TODO: �ڴ˴���ӹ������
	ExitThreadCheckConnect=false;
}

CClientView::~CClientView()
{
	closesocket(m_sClient);
	WSACleanup();
}

BOOL CClientView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ
	cs.style &= ~WS_BORDER;
	return CView::PreCreateWindow(cs);
}

// CClientView ����

void CClientView::OnDraw(CDC* /*pDC*/)
{
	CClientDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���	
	GetClientRect(&rc);
}


// CClientView ��ӡ


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
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CClientView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
	if(!m_pCallPrintDlg)
	{
		m_pCallPrintDlg = pDlg;
	}
	::SendMessage(m_pCallPrintDlg->m_hWnd,WM_BEGIN_PRINTING,(WPARAM) pDC, (LPARAM) pInfo);
}

void CClientView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: �ڴ����ר�ô����/����û���
	::SendMessage(m_pCallPrintDlg->m_hWnd,WM_MY_PRINT,(WPARAM) pDC, (LPARAM) pInfo) ;
	CView::OnPrint(pDC, pInfo);
}

void CClientView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: ��Ӵ�ӡ����е��������
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


// CClientView ���

#ifdef _DEBUG
void CClientView::AssertValid() const
{
	CView::AssertValid();
}

void CClientView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CClientDoc* CClientView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CClientDoc)));
	return (CClientDoc*)m_pDocument;
}
#endif //_DEBUG


// CClientView ��Ϣ�������


void CClientView::OnInitialUpdate()
{
	CView::OnInitialUpdate();	
	// TODO: �ڴ����ר�ô����/����û���

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
	    pAnalasisDlg->Create(IDD_DIALOG_ANALASIS,this);
	    pAnalasisDlg->ShowWindow(SW_SHOW);
	    pAnalasisDlg->MoveWindow(rc); 
        pDlg=pAnalasisDlg;
	}
	return 0;
}


void CClientView::OnUpdateButtonLogin(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->Enable(isDlgLogin);
}

void CClientView::OnUpdateButtonNormspecsend(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
//	pCmdUI->Enable(isDlgUISelect);
}


void CClientView::OnButtonLogout()
{
	// TODO: �ڴ���������������
	((CAnalasisDlg*)(this->pDlg))->ReleaseResource();
	OnUiselect(0, NULL);
}


void CClientView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	GetClientRect(&rc);
}


void CClientView::ConnectServer(void)
{
	OnUidisplay( 1, NULL);
	return ;
}


void CClientView::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(pDlg)
	{
		pDlg->DestroyWindow();
	}
	CView::OnClose();	
}


void CClientView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CView::OnPaint()
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

	dc.BitBlt(0,0,rc.Width(), rc.Height(),&memDC,0,0,SRCCOPY); //�����ڴ�DC����Ļ
	memDC.DeleteDC(); 
}