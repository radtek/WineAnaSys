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

// DataServerCenterView.cpp : CDataServerCenterView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "DataServerCenter.h"
#endif
#include "MainUIDlg.h"
#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")
#include "DataServerCenterDoc.h"
#include "DataServerCenterView.h"
#include "DBManageDlg.h"
#include "UserManageDlg.h"
#include <iostream>
#include <fstream>
#include <odbcinst.h>
#include "DBBackUpAndRestoreDlg.h"
using std::ofstream;
using std::ios;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define PORT 8000
#define DATA_BUFSIZE 12288

#define  RECV_POSTED 0
#define  SEND_POSTED 1

extern CString g_CurrentDir;
bool MaxChart=0;

bool ExitCheckLibZip;
bool ExitCheckAnaZip;

extern CRITICAL_SECTION g_csListFinishedAutoPredict;
// CDataServerCenterView

IMPLEMENT_DYNCREATE(CDataServerCenterView, CView);
BEGIN_MESSAGE_MAP(CDataServerCenterView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CDataServerCenterView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_MESSAGE(UIDisplay, &CDataServerCenterView::OnUidisplay)
	ON_MESSAGE(ManualAnalasisUIChange, &CDataServerCenterView::OnManualanalasisuichange)
	ON_WM_SIZE()
	ON_UPDATE_COMMAND_UI(ID_BUTTON_MANUALANA, &CDataServerCenterView::OnUpdateButtonManualana)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_SPECCOMPARE, &CDataServerCenterView::OnUpdateButtonSpeccompare)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_DBMANAGE, &CDataServerCenterView::OnUpdateButtonDbmanage)
	ON_UPDATE_COMMAND_UI(ID_CHECK_TRUE, &CDataServerCenterView::OnUpdateCheckTrue)
	ON_UPDATE_COMMAND_UI(ID_CHECK_FAKE, &CDataServerCenterView::OnUpdateCheckFake)
	ON_COMMAND(ID_CHECK_TRUE, &CDataServerCenterView::OnCheckTrue)
	ON_COMMAND(ID_CHECK_FAKE, &CDataServerCenterView::OnCheckFake)
	ON_COMMAND(ID_BUTTON_MANUALANA, &CDataServerCenterView::OnButtonManualana)
	ON_COMMAND(ID_BUTTON_DBMANAGE, &CDataServerCenterView::OnButtonDbmanage)
	ON_COMMAND(ID_BUTTON_LINECOLOR, &CDataServerCenterView::OnButtonLineColor)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_LINECOLOR, &CDataServerCenterView::OnUpdateButtonLineColor)
	ON_COMMAND(ID_BUTTON_LINECOLORDIY, &CDataServerCenterView::OnButtonLineColorDiy)
	ON_WM_NCDESTROY()
	ON_WM_PAINT()
	ON_COMMAND(ID_BUTTON_USERMANAGE, &CDataServerCenterView::OnButtonUsermanage)
	ON_WM_CLOSE()
	ON_COMMAND(ID_BUTTON_DBMANAGE_TRUE, &CDataServerCenterView::OnButtonDbmanageTrue)
	ON_COMMAND(ID_BUTTON_DBMANAGE_FAKE, &CDataServerCenterView::OnButtonDbmanageFake)
	ON_COMMAND(ID_DELETEFINPRO, &CDataServerCenterView::OnDeletefinpro)
	ON_COMMAND(ID_MANUALANA, &CDataServerCenterView::OnManualana)
	ON_COMMAND(ID_TRACEBACKHISTORY, &CDataServerCenterView::OnTracebackhistory)
	ON_COMMAND(ID_DBBACKUPANDRESTORE, &CDataServerCenterView::OnDbbackupandrestore)
	ON_BN_CLICKED(ID_BUTTON_ENTERSYSTEMMAINUI,&CDataServerCenterView::OnEnterSystemMainui)
	//ON_MESSAGE(WM_ENTERSYSTEMMAINUI, &CDataServerCenterView::OnEntersystemmainui)
	ON_COMMAND(ID_RESULT_DETAIL, &CDataServerCenterView::OnResultDetail)
	ON_COMMAND(ID_FILE_OPEN, &CDataServerCenterView::OnFileOpen)
END_MESSAGE_MAP()

	// CDataServerCenterView ����/����

CDataServerCenterView::CDataServerCenterView()
	:pDlg(NULL)
{
	// TODO: �ڴ˴���ӹ������
}

CDataServerCenterView::~CDataServerCenterView()
{
	if(pDlg!=NULL)
	{
		pDlg->DestroyWindow();
	}
}

BOOL CDataServerCenterView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ
	cs.style &= ~WS_BORDER;
	return CView::PreCreateWindow(cs);
}

// CDataServerCenterView ����

void CDataServerCenterView::OnDraw(CDC* /*pDC*/)
{
	CDataServerCenterDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CDataServerCenterView ��ӡ


void CDataServerCenterView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CDataServerCenterView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CDataServerCenterView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CDataServerCenterView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}

void CDataServerCenterView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CDataServerCenterView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CDataServerCenterView ���

#ifdef _DEBUG
void CDataServerCenterView::AssertValid() const
{
	CView::AssertValid();
}

void CDataServerCenterView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDataServerCenterDoc* CDataServerCenterView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDataServerCenterDoc)));
	return (CDataServerCenterDoc*)m_pDocument;
}
#endif //_DEBUG


// CDataServerCenterView ��Ϣ�������
BOOL ExistAccessDSN(LPCTSTR szDSN, LPCTSTR szDBName)
{
	HKEY hkey = {0};
	BYTE szBuffer[MAXBYTE] = {0};
	DWORD nSize = sizeof(szBuffer);
	CString szReg = L"Software\\ODBC\\ODBC.INI\\";
	szReg+=szDSN;
	//����û�ODBC����Դ�Ƿ���ڡ�
	
	LONG Ret = RegOpenKey(HKEY_CURRENT_USER, szReg, &hkey);
	if(Ret == ERROR_SUCCESS)
	{
		Ret = RegQueryValueEx(hkey,L"DBQ", NULL, NULL, szBuffer, &nSize);
		CString s = (LPTSTR)szBuffer;
		if(Ret == ERROR_SUCCESS)
		{ 
			//if(strcmp((LPCTSTR)szBuffer, szDBName) == 0)
			//if(szBuffer==szDBName)
			if(s==szDBName)
			{
				RegCloseKey(hkey);
				return TRUE;
			}
		} 
	}
	if(hkey != NULL)
	{
		RegCloseKey(hkey);
		ZeroMemory(hkey, sizeof(hkey));
	}
	//���ϵͳODBC����Դ�Ƿ���ڡ�
	szReg = L"SOFTWARE\\ODBC\\ODBC.INI\\";
	szReg+=szDSN;
	Ret = RegOpenKey(HKEY_LOCAL_MACHINE, szReg, &hkey);
	if(Ret == ERROR_SUCCESS)
	{
		ZeroMemory(szBuffer, MAXBYTE);
		Ret = RegQueryValueEx(hkey,L"DBQ", NULL, NULL, szBuffer, &nSize);
		if(Ret == ERROR_SUCCESS)
		{
			CString s = (LPTSTR)szBuffer;
			//if(strcmp((LPCTSTR)szBuffer, szDBName) == 0)
			if(s==szDBName)
			{
				RegCloseKey(hkey);
				return TRUE;
			}
		}
	}
	if(hkey != NULL)
	{
		RegCloseKey(hkey);
		ZeroMemory(hkey, sizeof(hkey));
	}
	return FALSE;
}

void CDataServerCenterView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	isMainUIDlg = 0;
	GetClientRect(&rc);

	m_btnEnterSystem.Create(L"����\nϵͳ",WS_CHILD|WS_VISIBLE,CRect(0,0,128,128),this,ID_BUTTON_ENTERSYSTEMMAINUI);
	CFont Font; //= m_btnEnterSystem.GetFont();
	LOGFONT LogFont;
	LogFont.lfWeight=0; //�������=10
	LogFont.lfHeight=0;	 //����߶�(��ת���������)=56
	LogFont.lfWidth=0;   //������(��ת�������߶�)=20
	LogFont.lfUnderline=FALSE; //���»���
	LogFont.lfStrikeOut=FALSE; //��ɾ����
	LogFont.lfItalic=FALSE; //��б��
	//LogFont.lfCharSet=DEFAULT_CHARSET; //ʹ��ȱʡ�ַ���
	wcscpy(LogFont.lfFaceName,L"΢���ź�"); //������=@system L"����_GB2312"
	//LogFont.lfFaceName =L"@system";  //��������
	//pFont->Detach();
	//���Ĳ���Ŀ���ǽ�pFont��װ�е�HFONT�������������pFont�޷����ý��ӵ�Create������
	Font.CreateFontIndirect(&LogFont);
	m_btnEnterSystem.SetFont(&Font);
	//::SetTextColor(m_btnEnterSystem.GetDC()->m_hDC,RGB(255,0,0));
	//m_btnEnterSystem.GetDC()->SetTextColor(RGB(255,0,0));
	m_btnEnterSystem.SetColor(CButtonST::BTNST_COLOR_FG_OUT,RGB(3,48,139));
	m_btnEnterSystem.SetColor(CButtonST::BTNST_COLOR_FG_IN,RGB(255,0,0));

	HICON hIcon=AfxGetApp()->LoadIcon(IDI_ICON_ENTERSYSTEM); 
	m_btnEnterSystem.SetIcon(hIcon); 
	m_btnEnterSystem.DrawTransparent(TRUE);
	m_btnEnterSystem.SetTooltipText(L"����ϵͳ������");
	m_btnEnterSystem.DrawBorder(FALSE);
}

afx_msg void CDataServerCenterView::OnEnterSystemMainui()
{
	CMainUIDlg *tempDlg=new CMainUIDlg(this);
	tempDlg->Create(IDD_DIALOG_MAINUI,this);
	tempDlg->ShowWindow(SW_SHOW);
	tempDlg->MoveWindow(rc);
	pDlg=tempDlg;

	m_btnEnterSystem.ShowWindow(SW_HIDE);
	Invalidate();
}

afx_msg LRESULT CDataServerCenterView::OnUidisplay(WPARAM wParam, LPARAM lParam)
{

	return 0;
}


afx_msg LRESULT CDataServerCenterView::OnManualanalasisuichange(WPARAM wParam, LPARAM lParam)
{
	if(isSpecCompareDlg)
	{
		int nIndex;
		nIndex=((CMainUIDlg*)pDlg)->m_ListFinishedAutoPredict.GetSelectionMark();

		EnterCriticalSection(&g_csListFinishedAutoPredict);//�����ٽ���
		list<proFinInfo>::iterator itprojectFinishedList = ((CMainUIDlg*)pDlg)->projectFinishedList.begin();
		for(int i=0;i<nIndex;i++)
		{
			++itprojectFinishedList;
		}

		pDlg->ShowWindow(SW_HIDE);
		CSpectrumCompareDlg *SpectrumCompareDlg=new CSpectrumCompareDlg(this);
		SpectrumCompareDlg->m_projectName= itprojectFinishedList->projectFinishedName;
		SpectrumCompareDlg->m_projectPath= itprojectFinishedList->projectFinishedPath;
		LeaveCriticalSection(&g_csListFinishedAutoPredict);

		SpectrumCompareDlg->Create(IDD_DIALOG_SPECCOMPARE,this);
		SpectrumCompareDlg->ShowWindow(SW_SHOW);
		SpectrumCompareDlg->MoveWindow(rc); 
		pSpectrumCompareDlg=SpectrumCompareDlg;

	}
	else
	{
		if(pSpectrumCompareDlg!=NULL)
		{
			pSpectrumCompareDlg->DestroyWindow();
			pSpectrumCompareDlg=NULL;
		}
		pDlg->ShowWindow(SW_SHOW);
	}
	return 0;
}


void CDataServerCenterView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	// TODO: �ڴ˴������Ϣ����������
	GetClientRect(&rc);
}


void CDataServerCenterView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CView::OnPaint()
	if(pDlg)
	{
		pDlg->MoveWindow(rc);
	}
	else
	{
		CRect rc;
		GetClientRect(&rc);
		CDC dcMem;
		dcMem.CreateCompatibleDC(&dc);
		CBitmap bmpBackground;
		bmpBackground.LoadBitmap(IDB_BACKGROUND);

		BITMAP bitmap;
		bmpBackground.GetBitmap(&bitmap);
		CBitmap* pbmpPri = dcMem.SelectObject(&bmpBackground);
		dc.StretchBlt(0,0,rc.Width(), rc.Height(), &dcMem,0,0,bitmap.bmWidth, bitmap.bmHeight, SRCCOPY);

		CFont Font; //= m_btnEnterSystem.GetFont();
		LOGFONT LogFont;
		LogFont.lfWeight=10; //�������=10
		LogFont.lfHeight=30;	 //����߶�(��ת���������)=56
		LogFont.lfWidth=18;   //������(��ת�������߶�)=20
		LogFont.lfUnderline=FALSE; //���»���
		LogFont.lfStrikeOut=FALSE; //��ɾ����
		LogFont.lfItalic=FALSE; //��б��
		LogFont.lfCharSet=DEFAULT_CHARSET; //ʹ��ȱʡ�ַ���
		LogFont.lfEscapement=0; //������ʾ�Ƕ�=90��
		wcscpy(LogFont.lfFaceName,L"����"); //������=@system_GB2312
		Font.CreateFontIndirect(&LogFont);
		CFont *oldFont = dc.SelectObject(&Font);
		COLORREF oldColor = dc.SetTextColor(RGB(3,48,139));
		CSize strsize = dc.GetTextExtent(L"�׾Ƽ�������Դ�ۺϷ���ƽ̨");
		dc.SetBkMode(TRANSPARENT);
		dc.TextOut(rc.Width()-strsize.cx - 50,rc.Height()/2,L"�׾Ƽ�������Դ�ۺϷ���ƽ̨");
		dc.SelectObject(oldFont);
		dc.SetTextColor(oldColor);
		
		m_btnEnterSystem.MoveWindow(rc.Width()-300,rc.Height()-200,128,128);
	}
}

void CDataServerCenterView::OnUpdateButtonManualana(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	
}

void CDataServerCenterView::OnUpdateButtonSpeccompare(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	//pCmdUI->Enable(isManualAnalasisDlg);
}


void CDataServerCenterView::OnUpdateButtonDbmanage(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	//pCmdUI->Enable(isUISelectDlg);
}


void CDataServerCenterView::OnUpdateCheckTrue(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->SetCheck(!SelectPage);
//	pCmdUI->Enable(isUISelectDlg);
}


void CDataServerCenterView::OnUpdateCheckFake(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->SetCheck(SelectPage);
	//pCmdUI->Enable(isUISelectDlg);
}


void CDataServerCenterView::OnCheckTrue()
{
	// TODO: �ڴ���������������
	SelectPage=0;
}


void CDataServerCenterView::OnCheckFake()
{
	// TODO: �ڴ���������������
	SelectPage=1;
}


void CDataServerCenterView::OnButtonManualana()
{
	// TODO: �ڴ���������������
	OnUidisplay(NULL,NULL);
}


void CDataServerCenterView::OnButtonDbmanage()
{
	// TODO: �ڴ���������������
	if(IDOK == MessageBox(L"�������׿�����ܽ���ʱֹͣ�Զ���Դ\n���ܣ��˳����׿������Զ��ָ������Ƿ������",L"",MB_OKCANCEL|MB_ICONQUESTION))
	{
		::SendMessage(pDlg->m_hWnd,WM_PAUSEPREDICTPROJECT,NULL,NULL);

		CDBManageDlg DBManageDlg;
		DBManageDlg.SelectPage=SelectPage;

		DBManageDlg.pwnd = this;
		DBManageDlg.DoModal();
	}
}

void CDataServerCenterView::OnButtonDbmanageTrue()
{
	// TODO: �ڴ���������������
	if(IDOK == MessageBox(L"�������׿�����ܽ���ʱֹͣ�Զ���Դ\n���ܣ��˳����׿������Զ��ָ������Ƿ������",L"",MB_OKCANCEL|MB_ICONQUESTION))
	{
		if(!pDlg)
		{
			MessageBox(L"���ȵ�¼ϵͳ",L"���׿����",MB_ICONWARNING);
			return;
		}
		::SendMessage(pDlg->m_hWnd,WM_PAUSEPREDICTPROJECT,NULL,NULL);
		CDBManageDlg DBManageDlg;
		DBManageDlg.SelectPage=0;
		DBManageDlg.pwnd = this;
		DBManageDlg.DoModal();
	}
}


void CDataServerCenterView::OnButtonDbmanageFake()
{
	// TODO: �ڴ���������������
	if(IDOK == MessageBox(L"�������׿�����ܽ���ʱֹͣ�Զ���Դ\n���ܣ��˳����׿������Զ��ָ������Ƿ������",L"",MB_OKCANCEL|MB_ICONQUESTION))
	{
		::SendMessage(pDlg->m_hWnd,WM_PAUSEPREDICTPROJECT,NULL,NULL);
		CDBManageDlg DBManageDlg;
		DBManageDlg.SelectPage=1;
		DBManageDlg.pwnd = this;
		DBManageDlg.DoModal();
	}

}

void CDataServerCenterView::OnButtonLineColor()
{
	// TODO: �ڴ���������������
	CMFCRibbonBar *pRibbon = ((CFrameWndEx*)AfxGetMainWnd())->GetRibbonBar(); 
	ASSERT_VALID(pRibbon); 
	CMFCRibbonColorButton *m_pLineColorButton; 
	m_pLineColorButton = DYNAMIC_DOWNCAST(CMFCRibbonColorButton, pRibbon->FindByID(ID_BUTTON_LINECOLOR));

	((CSpectrumCompareDlg *)(this->pSpectrumCompareDlg))->m_LineColor=m_pLineColorButton->GetColor();

}


void CDataServerCenterView::OnUpdateButtonLineColor(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->Enable(this->isSpecCompareDlg);
}

void CDataServerCenterView::OnButtonLineColorDiy()
{
	CColorDialog colotDlg;
	if(colotDlg.DoModal() == IDOK)
	{
		((CSpectrumCompareDlg *)(this->pSpectrumCompareDlg))->m_LineColor=colotDlg.GetColor();
	}
}

void CDataServerCenterView::OnNcDestroy()
{
	CView::OnNcDestroy();
	// TODO: �ڴ˴������Ϣ����������
}
int CALLBACK BrowserCallbackProc(HWND   hWnd,UINT   uMsg,LPARAM  lParam,LPARAM  lpData)  
{  
	switch   (uMsg)  
	{  
	case   BFFM_INITIALIZED:  
		::SendMessage(hWnd, BFFM_SETSELECTION, 1,lpData);  
		break;  
	default:  
		break;  
	}  
	return   0;  
} 

void CDataServerCenterView::OnButtonUsermanage()
{
	// TODO: �ڴ���������������
	CUserManageDlg UserManageDlg;
	UserManageDlg.DoModal();
}


void CDataServerCenterView::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(pDlg)
	{
		pDlg->DestroyWindow();
	}
	CView::OnClose();
}




void CDataServerCenterView::OnDeletefinpro()
{
	// TODO: �ڴ���������������
	((CMainUIDlg*)pDlg)->OnBnClickedButtonDeletefinishedproject();
}


void CDataServerCenterView::OnManualana()
{
	// TODO: �ڴ���������������
	((CMainUIDlg*)pDlg)->OnBnClickedButtonManualcompare();
}


void CDataServerCenterView::OnTracebackhistory()
{
	// TODO: �ڴ���������������
	((CMainUIDlg*)pDlg)->OnBnClickedButtonPredictedrecord();
}



void CDataServerCenterView::OnResultDetail()
{
	// TODO: �ڴ���������������
	NMHDR nmhdr;
	nmhdr.hwndFrom = pDlg->GetDlgItem(IDC_LIST_FINISHEDPREDICT)->m_hWnd;//pDlg->m_hWnd;
	nmhdr.idFrom = IDC_LIST_FINISHEDPREDICT;//(UINT_PTR)pDlg->GetDlgItem(IDC_LIST_FINISHEDPREDICT);
	nmhdr.code = NM_DBLCLK;  // �û��Զ�����Ϣ
	::SendMessage(pDlg->m_hWnd,WM_NOTIFY, (WPARAM)nmhdr.idFrom, (LPARAM)&nmhdr);

	//::PostMessage(pDlg->m_hWnd,WM_NOTIFY,NM_DBLCLK,(LPARAM)pDlg->);
	//((CMainUIDlg*)pDlg)->OnNMDblclkListFinishedpredict();
}

void CDataServerCenterView::OnDbbackupandrestore()
{
	// TODO: �ڴ���������������
	CDBBackUpAndRestoreDlg DBBackUpAndRestoreDlg;
	if(IDOK == DBBackUpAndRestoreDlg.DoModal())
	{
		CDatabase m_database;
		if(!m_database.IsOpen()) 
		{ 
			if(!m_database.Open(_T( "�׾Ƽ�������Դ���ݿ�"))) 
			{
				MessageBox(L"���ܴ򿪵�������Դ������! "); 
				return;
			}

		}
		CString strSQL; 
		if(DBBackUpAndRestoreDlg.m_radioBackupOrRestore == 0)
		{
			strSQL="backup database �׾Ƽ�������Դ���ݿ� to disk='";
			CString sBackupPath = DBBackUpAndRestoreDlg.m_BackupOrRestorePath;
			CString sBackName;

			SYSTEMTIME sys; 
			GetLocalTime(&sys); 
			CString sTime;
			sTime.Format(L"%d-%d-%d",sys.wYear,sys.wMonth,sys.wDay);
			sBackName=sTime;

			sBackupPath+=L"\\";
			sBackupPath+=sBackName;

			CString tempsBackupPath=sBackupPath;
			int i=1;
			while(PathFileExists(tempsBackupPath))
			{
				CString num;
				num.Format(L"-%d",i);
				tempsBackupPath=sBackupPath;
				tempsBackupPath+=num;
				i++;
			}
			sBackupPath=tempsBackupPath;
			sBackupPath+=L".bak";
			strSQL=strSQL+sBackupPath+L"'";//ֱ�� strSQL="backup database studentManager to disk='sPath'"�ǲ��еģ�sqlʶ����sPath����

			bool bFaile = 0;
			try
			{
				m_database.ExecuteSQL(strSQL);
				m_database.Close(); 
			}
			catch (_com_error& e)
			{
				bFaile = 1;
			}			
			if(!bFaile)
			{
				AfxMessageBox(L"���ݿⱸ�ݳɹ�!");
			}
			else
			{
				AfxMessageBox(L"���ݿⱸ��ʧ��!");
			}		
		}
		else
		{
			CString sRestoreFilePath = DBBackUpAndRestoreDlg.m_BackupOrRestorePath;
			CString strSQL; 
			strSQL="use master";  //����ת����master���ݿ�
			m_database.ExecuteSQL(strSQL);
			strSQL="restore database �׾Ƽ�������Դ���ݿ� from disk = '";  //�Ϳ��Ի�ԭ���ݿ���
			strSQL=strSQL+sRestoreFilePath+L"'";
			strSQL="use �׾Ƽ�������Դ���ݿ�"; //��ת����
			bool bFaile = 0;
			try
			{
				m_database.ExecuteSQL(strSQL);
				m_database.Close(); 
			}
			catch (_com_error& e)
			{
				bFaile = 1;
			}
			if(!bFaile)
			{
				AfxMessageBox(L"���ݿ⻹ԭ�ɹ�!");
			}
			else
			{
				AfxMessageBox(L"���ݿ⻹ԭʧ��!");
			}		
			return;
		}
	}
}



void CDataServerCenterView::OnFileOpen()
{
	// TODO: �ڴ���������������
	if(!pDlg)
	{
		MessageBox(L"���ȵ�¼ϵͳ",L"������Ŀ",MB_ICONWARNING);
		return;
	}
	CString ProPath;
	BROWSEINFO bi;
	char Buffer[MAX_PATH];
	//CString m_DatasetFoldPath ;//��·��������һ��CString������
	bi.hwndOwner = this->m_hWnd;
	bi.pidlRoot =NULL;//��ʼ���ƶ���rootĿ¼�ܲ�����
	bi.pszDisplayName =(LPWSTR)Buffer;//�˲�����ΪNULL������ʾ�Ի���
	bi.lpszTitle = L"ѡ����Ŀ·��";
	bi.ulFlags =BIF_USENEWUI ; //BIF_EDITBOX;//���༭��ķ��
	bi.lpfn = NULL;
	bi.lParam = 0;
	bi.iImage=IDR_MAINFRAME;
	//��ʼ����ڲ���bi����
	LPITEMIDLIST pIDList = SHBrowseForFolder(&bi);//������ʾѡ��Ի���

	if(pIDList==NULL)
	{	
		return;
	}
	SHGetPathFromIDList(pIDList,(LPWSTR)Buffer);
	//ȡ���ļ���·����Buffer��
	ProPath.Format(L"%s",Buffer);
	// free memory used      
	IMalloc * imalloc = 0;
	if (SUCCEEDED(SHGetMalloc(&imalloc)))
	{
		imalloc->Free (pIDList);
		imalloc->Release();
	}
	CString ProPathNew;
	ProPathNew.Format(L"%s\\Project\\WaitForAutoAnalasis\\%s",g_CurrentDir,ProPath.Right(ProPath.GetLength() - ProPath.ReverseFind('\\')-1));
	CopyFolder(ProPath,ProPathNew);
	((CMainUIDlg*)pDlg)->AddOneProject(ProPathNew);

}
