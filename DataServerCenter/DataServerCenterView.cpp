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

// DataServerCenterView.cpp : CDataServerCenterView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
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
	// 标准打印命令
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

	// CDataServerCenterView 构造/析构

CDataServerCenterView::CDataServerCenterView()
{
	// TODO: 在此处添加构造代码
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
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式
	cs.style &= ~WS_BORDER;
	return CView::PreCreateWindow(cs);
}

// CDataServerCenterView 绘制

void CDataServerCenterView::OnDraw(CDC* /*pDC*/)
{
	CDataServerCenterDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CDataServerCenterView 打印


void CDataServerCenterView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CDataServerCenterView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CDataServerCenterView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CDataServerCenterView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
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


// CDataServerCenterView 诊断

#ifdef _DEBUG
void CDataServerCenterView::AssertValid() const
{
	CView::AssertValid();
}

void CDataServerCenterView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDataServerCenterDoc* CDataServerCenterView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDataServerCenterDoc)));
	return (CDataServerCenterDoc*)m_pDocument;
}
#endif //_DEBUG


// CDataServerCenterView 消息处理程序
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
	//检测用户ODBC数据源是否存在。
	
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
	//检测系统ODBC数据源是否存在。
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

		EnterCriticalSection(&g_csListFinishedAutoPredict);//进入临界区
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
	// TODO: 在此处添加消息处理程序代码
	GetClientRect(&rc);
}


void CDataServerCenterView::OnPaint()
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

	dc.BitBlt(0,0,rc.Width(), rc.Height(),&memDC,0,0,SRCCOPY); //拷贝内存DC到屏幕
	//memDC.SelectObject(oldBitmap) ;
	memDC.DeleteDC(); 
}

void CDataServerCenterView::OnUpdateButtonManualana(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码

}


void CDataServerCenterView::OnUpdateButton2(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
}


void CDataServerCenterView::OnUpdateButtonSpeccompare(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	//pCmdUI->Enable(isManualAnalasisDlg);
}


void CDataServerCenterView::OnUpdateButtonDbmanage(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	//pCmdUI->Enable(isUISelectDlg);
}


void CDataServerCenterView::OnUpdateCheckTrue(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(!SelectPage);
//	pCmdUI->Enable(isUISelectDlg);
}


void CDataServerCenterView::OnUpdateCheckFake(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(SelectPage);
	//pCmdUI->Enable(isUISelectDlg);
}


void CDataServerCenterView::OnCheckTrue()
{
	// TODO: 在此添加命令处理程序代码
	SelectPage=0;
}


void CDataServerCenterView::OnCheckFake()
{
	// TODO: 在此添加命令处理程序代码
	SelectPage=1;
}


void CDataServerCenterView::OnButtonManualana()
{
	// TODO: 在此添加命令处理程序代码
	OnUidisplay(NULL,NULL);
}


void CDataServerCenterView::OnButtonDbmanage()
{
	// TODO: 在此添加命令处理程序代码
	if(IDOK == MessageBox(L"启动光谱库管理功能将暂时停止自动鉴定\n功能（退出光谱库管理后将自动恢复），是否继续？",L"",MB_OKCANCEL|MB_ICONQUESTION))
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
	// TODO: 在此添加命令处理程序代码
	if(IDOK == MessageBox(L"启动光谱库管理功能将暂时停止自动鉴定\n功能（退出光谱库管理后将自动恢复），是否继续？",L"",MB_OKCANCEL|MB_ICONQUESTION))
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
	// TODO: 在此添加命令处理程序代码
	if(IDOK == MessageBox(L"启动光谱库管理功能将暂时停止自动鉴定\n功能（退出光谱库管理后将自动恢复），是否继续？",L"",MB_OKCANCEL|MB_ICONQUESTION))
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
	// TODO: 在此添加命令处理程序代码
	CMFCRibbonBar *pRibbon = ((CFrameWndEx*)AfxGetMainWnd())->GetRibbonBar(); 
	ASSERT_VALID(pRibbon); 
	CMFCRibbonColorButton *m_pLineColorButton; 
	m_pLineColorButton = DYNAMIC_DOWNCAST(CMFCRibbonColorButton, pRibbon->FindByID(ID_BUTTON_LINECOLOR));

	//m_LineColor=m_pLineColorButton->GetColor();
	((CSpectrumCompareDlg *)(this->pSpectrumCompareDlg))->m_LineColor=m_pLineColorButton->GetColor();

}


void CDataServerCenterView::OnUpdateButtonLineColor(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
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
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(this->isSpecCompareDlg);
}

void CDataServerCenterView::OnNcDestroy()
{
	CView::OnNcDestroy();
	// TODO: 在此处添加消息处理程序代码
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
	// TODO: 在此添加命令处理程序代码
	CDatabase m_database;
	if(!m_database.IsOpen()) 
	{ 
		if(!m_database.Open(_T( "白酒鉴定与溯源数据库"))) 
		{
			MessageBox(L"不能打开到该数据源的连接! "); 
			return;
		}

	}
	CString strSQL; 
	strSQL="backup database 白酒鉴定与溯源数据库 to disk='";
	CString sBackupPath;

	CString _currentDir;
	GetCurrentDirectory(MAX_PATH,_currentDir.GetBuffer(MAX_PATH));
	_currentDir.ReleaseBuffer(); 

	CString sInitPath=_currentDir;
	sInitPath+=L"\\";
	sInitPath+=L"Backup";

	BROWSEINFO bi;
	char Buffer[MAX_PATH];
	//CString m_DatasetFoldPath ;//将路径保存在一个CString对象里
	bi.hwndOwner = this->m_hWnd;
	bi.pidlRoot =NULL;//初始化制定的root目录很不容易
	bi.pszDisplayName =(LPWSTR)Buffer;//此参数如为NULL则不能显示对话框
	bi.lpszTitle = L"请选择备份路径";
	bi.ulFlags =BIF_USENEWUI ; //BIF_EDITBOX;//带编辑框的风格
	bi.lpfn = BrowserCallbackProc;
	bi.lParam =(LPARAM)sInitPath.GetBuffer();
	bi.iImage=IDR_MAINFRAME;
	//初始化入口参数bi结束
	LPITEMIDLIST pIDList = SHBrowseForFolder(&bi);//调用显示选择对话框

	if(pIDList==NULL)
	{	
		return;
	}
	SHGetPathFromIDList(pIDList,(LPWSTR)Buffer);
	//取得文件夹路径到Buffer里
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
	//可以用strSQL="backup database studentManager to disk='f:\\backup\\studentManager'"这样的绝对路径，我这里考虑移植问题用了相对路径
	strSQL=strSQL+sBackupPath+L"'";//直接 strSQL="backup database studentManager to disk='sPath'"是不行的，sql识别不了sPath变量
	m_database.ExecuteSQL(strSQL); 
	m_database.Close(); 
	AfxMessageBox(L"数据库备份成功!");
	return;
}


void CDataServerCenterView::OnButtonRestore()
{
	// TODO: 在此添加命令处理程序代码
	CDatabase m_database;
	if(!m_database.IsOpen()) 
	{
		if(!m_database.Open(_T( "白酒鉴定与溯源数据库"))) 
		{
			MessageBox(L"不能打开到该数据源的连接! "); 
			return;
		}
	}
	CFileDialog FileDialog(true,L"bak",NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,L"备份文件(*.bak)|*.bak||", this);

	CString _currentDir;
	GetCurrentDirectory(MAX_PATH,_currentDir.GetBuffer(MAX_PATH));
	_currentDir.ReleaseBuffer(); 

	CString sInitPath=_currentDir;
	sInitPath+=L"\\";
	sInitPath+=L"Backup";

	FileDialog.m_ofn.lpstrInitialDir=sInitPath;
	FileDialog.m_ofn.lpstrTitle=L"选择备份文件";
	if(IDOK==FileDialog.DoModal())
	{
		CString sPath=FileDialog.GetPathName();
		sPath.Replace(L"\\",L"\\\\");
		CString strSQL; 
		strSQL="use master";  //这里转换到master数据库
		m_database.ExecuteSQL(strSQL);
		strSQL="restore database 白酒鉴定与溯源数据库 from disk = '";  //就可以还原数据库了
		strSQL=strSQL+sPath+L"'";
		m_database.ExecuteSQL(strSQL); 
		strSQL="use 白酒鉴定与溯源数据库"; //再转回来
		m_database.ExecuteSQL(strSQL);
		m_database.Close(); 
		AfxMessageBox(L"数据库还原成功!");
		return;
	}
}



void CDataServerCenterView::OnButtonUsermanage()
{
	// TODO: 在此添加命令处理程序代码
	CUserManageDlg UserManageDlg;
	UserManageDlg.DoModal();
}


void CDataServerCenterView::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(pDlg)
	{
		pDlg->DestroyWindow();
	}
	CView::OnClose();
}


