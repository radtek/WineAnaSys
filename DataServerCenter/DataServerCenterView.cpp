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
#pragma coment(lib,"ws2_32.lib")
#include "DataServerCenterDoc.h"
#include "DataServerCenterView.h"
#include "DBManageDlg.h"
#include "UserManageDlg.h"
#include <iostream>
#include <fstream>
#include <odbcinst.h>
using std::ofstream;
using std::ios;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define PORT 8000
#define DATA_BUFSIZE 12288

#define  RECV_POSTED 0
#define  SEND_POSTED 1


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
	ON_UPDATE_COMMAND_UI(ID_BUTTON2, &CDataServerCenterView::OnUpdateButton2)
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
	ON_UPDATE_COMMAND_UI(ID_BUTTON_LINECOLORDIY, &CDataServerCenterView::OnUpdateOnButtonLineColorDiy)
	ON_WM_NCDESTROY()
	ON_COMMAND(ID_BUTTON_BACKUP, &CDataServerCenterView::OnButtonBackup)
	ON_COMMAND(ID_BUTTON_RESTORE, &CDataServerCenterView::OnButtonRestore)
	ON_WM_PAINT()
	ON_COMMAND(ID_BUTTON_USERMANAGE, &CDataServerCenterView::OnButtonUsermanage)
	ON_WM_CLOSE()
	ON_COMMAND(ID_BUTTON_DBMANAGE_TRUE, &CDataServerCenterView::OnButtonDbmanageTrue)
	ON_COMMAND(ID_BUTTON_DBMANAGE_FAKE, &CDataServerCenterView::OnButtonDbmanageFake)
END_MESSAGE_MAP()

	// CDataServerCenterView ����/����

CDataServerCenterView::CDataServerCenterView()
{
	// TODO: �ڴ˴���ӹ������
	SelectPage=0;
	isSpecCompareDlg=0;
	pDlg=NULL;
	pClusterDlg=NULL;
	pSpectrumCompareDlg=NULL;
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

	//CString szBuffer;
	DWORD nSize = sizeof(szBuffer);
	//char szReg[MAXBYTE] = {0};
	CString szReg = L"Software\\ODBC\\ODBC.INI\\";
	
	//strcpy(szReg, "Software\\ODBC\\ODBC.INI\\");
	//strcat(szReg, szDSN);
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
	GetClientRect(&rc);
	CMainUIDlg *tempDlg=new CMainUIDlg(this);
	tempDlg->Create(IDD_DIALOG_MAINUI,this);
	tempDlg->ShowWindow(SW_SHOW);
	tempDlg->MoveWindow(rc); 
	pDlg=tempDlg;

	
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

	GetClientRect(&rc);

	CDC memDC ;
	CBitmap memBitmap ;
	CBitmap* oldBitmap ; // bitmap originally found in CMemDC


	memDC.CreateCompatibleDC(&dc) ;
	memBitmap.CreateCompatibleBitmap(&dc, rc.Width(), rc.Height()) ;
	oldBitmap = (CBitmap *)memDC.SelectObject(&memBitmap) ;
	memBitmap.DeleteObject(); 
	//CRect rc;
	//::GetClientRect(this->m_hWnd,&rc);

	memDC.FillSolidRect(0,0,rc.Width(),rc.Height(),GetSysColor(COLOR_BTNFACE)); 
	if(NULL!=pSpectrumCompareDlg)
	{
		pSpectrumCompareDlg->MoveWindow(rc);
		memDC.BitBlt(0,0,rc.Width(),rc.Height(),pSpectrumCompareDlg->GetWindowDC(),0,0,SRCCOPY);
	}
	if(NULL!=pClusterDlg)
	{
		pClusterDlg->MoveWindow(rc);
		memDC.BitBlt(0,0,rc.Width(),rc.Height(),pClusterDlg->GetWindowDC(),0,0,SRCCOPY);
	}
	if(NULL!=pDlg)
	{
		pDlg->MoveWindow(rc);
		memDC.BitBlt(0,0,rc.Width(),rc.Height(),pDlg->GetWindowDC(),0,0,SRCCOPY);
	}

	dc.BitBlt(0,0,rc.Width(), rc.Height(),&memDC,0,0,SRCCOPY); //�����ڴ�DC����Ļ
	//memDC.SelectObject(oldBitmap) ;
	memDC.DeleteDC(); 
}

void CDataServerCenterView::OnUpdateButtonManualana(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������

}


void CDataServerCenterView::OnUpdateButton2(CCmdUI *pCmdUI)
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
	if(IDOK == MessageBox(L"�������׿�����ܽ���ʱֹͣ�Զ�����\n���ܣ��˳����׿������Զ��ָ������Ƿ������",L"",MB_OKCANCEL|MB_ICONQUESTION))
	{
		::SendMessage(pDlg->m_hWnd,WM_PAUSEPREDICTPROJECT,NULL,NULL);

		CDBManageDlg DBManageDlg;
		DBManageDlg.SelectPage=SelectPage;

		DBManageDlg.pwnd = this;
		//DBManageDlg.Create(IDD_DIALOG_DATABASEMANAGE,this);
		//DBManageDlg.ShowWindow(SW_SHOW);
		DBManageDlg.DoModal();
	}
}

void CDataServerCenterView::OnButtonDbmanageTrue()
{
	// TODO: �ڴ���������������
	if(IDOK == MessageBox(L"�������׿�����ܽ���ʱֹͣ�Զ�����\n���ܣ��˳����׿������Զ��ָ������Ƿ������",L"",MB_OKCANCEL|MB_ICONQUESTION))
	{
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
	if(IDOK == MessageBox(L"�������׿�����ܽ���ʱֹͣ�Զ�����\n���ܣ��˳����׿������Զ��ָ������Ƿ������",L"",MB_OKCANCEL|MB_ICONQUESTION))
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

	//m_LineColor=m_pLineColorButton->GetColor();
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
void CDataServerCenterView::OnUpdateOnButtonLineColorDiy(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->Enable(this->isSpecCompareDlg);
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

void CDataServerCenterView::OnButtonBackup()
{
	// TODO: �ڴ���������������
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
	strSQL="backup database �׾Ƽ�������Դ���ݿ� to disk='";
	CString sBackupPath;

	CString _currentDir;
	GetCurrentDirectory(MAX_PATH,_currentDir.GetBuffer(MAX_PATH));
	_currentDir.ReleaseBuffer(); 

	CString sInitPath=_currentDir;
	sInitPath+=L"\\";
	sInitPath+=L"Backup";

	BROWSEINFO bi;
	char Buffer[MAX_PATH];
	//CString m_DatasetFoldPath ;//��·��������һ��CString������
	bi.hwndOwner = this->m_hWnd;
	bi.pidlRoot =NULL;//��ʼ���ƶ���rootĿ¼�ܲ�����
	bi.pszDisplayName =(LPWSTR)Buffer;//�˲�����ΪNULL������ʾ�Ի���
	bi.lpszTitle = L"��ѡ�񱸷�·��";
	bi.ulFlags =BIF_USENEWUI ; //BIF_EDITBOX;//���༭��ķ��
	bi.lpfn = BrowserCallbackProc;
	bi.lParam =(LPARAM)sInitPath.GetBuffer();
	bi.iImage=IDR_MAINFRAME;
	//��ʼ����ڲ���bi����
	LPITEMIDLIST pIDList = SHBrowseForFolder(&bi);//������ʾѡ��Ի���

	if(pIDList==NULL)
	{	
		return;
	}
	SHGetPathFromIDList(pIDList,(LPWSTR)Buffer);
	//ȡ���ļ���·����Buffer��
	sBackupPath.Format(L"%s",Buffer);
	// free memory used      
	IMalloc * imalloc = 0;
	if (SUCCEEDED(SHGetMalloc(&imalloc)))
	{
		imalloc->Free (pIDList);
		imalloc->Release();
	}


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
	//������strSQL="backup database studentManager to disk='f:\\backup\\studentManager'"�����ľ���·���������￼����ֲ�����������·��
	strSQL=strSQL+sBackupPath+L"'";//ֱ�� strSQL="backup database studentManager to disk='sPath'"�ǲ��еģ�sqlʶ����sPath����
	m_database.ExecuteSQL(strSQL); 
	m_database.Close(); 
	AfxMessageBox(L"���ݿⱸ�ݳɹ�!");
	return;
}


void CDataServerCenterView::OnButtonRestore()
{
	// TODO: �ڴ���������������
	CDatabase m_database;
	if(!m_database.IsOpen()) 
	{
		if(!m_database.Open(_T( "�׾Ƽ�������Դ���ݿ�"))) 
		{
			MessageBox(L"���ܴ򿪵�������Դ������! "); 
			return;
		}
	}
	CFileDialog FileDialog(true,L"bak",NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,L"�����ļ�(*.bak)|*.bak||", this);

	CString _currentDir;
	GetCurrentDirectory(MAX_PATH,_currentDir.GetBuffer(MAX_PATH));
	_currentDir.ReleaseBuffer(); 

	CString sInitPath=_currentDir;
	sInitPath+=L"\\";
	sInitPath+=L"Backup";

	FileDialog.m_ofn.lpstrInitialDir=sInitPath;
	FileDialog.m_ofn.lpstrTitle=L"ѡ�񱸷��ļ�";
	if(IDOK==FileDialog.DoModal())
	{
		CString sPath=FileDialog.GetPathName();
		sPath.Replace(L"\\",L"\\\\");
		CString strSQL; 
		strSQL="use master";  //����ת����master���ݿ�
		m_database.ExecuteSQL(strSQL);
		strSQL="restore database �׾Ƽ�������Դ���ݿ� from disk = '";  //�Ϳ��Ի�ԭ���ݿ���
		strSQL=strSQL+sPath+L"'";
		m_database.ExecuteSQL(strSQL); 
		strSQL="use �׾Ƽ�������Դ���ݿ�"; //��ת����
		m_database.ExecuteSQL(strSQL);
		m_database.Close(); 
		AfxMessageBox(L"���ݿ⻹ԭ�ɹ�!");
		return;
	}
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


