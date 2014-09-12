// AnalasisDlg.cpp : 实现文件

#include "stdafx.h"
#include "MainFrm.h"
#include "Client.h"
#include "AnalasisDlg.h"
#include "PrintFrame.h"
#include "PrintView.h"
#include "afxdialogex.h"
#include "AddSpecToAnaInfo.h"
#include "ProjectInfoWriteDlg.h"
#include "ProjectInfoConfirmDlg.h"
#include "zipimplement.h" 
#include "ExcelOperator.h"
#include "ProjectSendProgressDlg.h"
#include "UserLoginDlg.h"
#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")
#include <Mswsock.h>
#include <iostream>
#include <fstream>
using namespace std;

CAnalasisDlg *g_pCAnalasisDlg = 0;

RSA *rsa_publickey = NULL;
CString UserName;
volatile int g_ThreadRecvingCount = 0;
volatile bool TransProject;
volatile bool ExitAllThread;

//线程终止标志
volatile bool ExitThreadCheckConnect;
volatile bool ExitThreadReceiveResult;
volatile bool ExitThreadPackAndSendProject;

//临界区
CRITICAL_SECTION g_csFinishedProInfo;
CRITICAL_SECTION g_csConns;

//服务器发送鉴定完成项目建立的连接
ConnectionList g_conns;

CString g_CurrentDir;
//监听套接字
SOCKET g_hListenSocket;

CString GetLoginUserName()
{
	return UserName;
}

// CAnalasisDlg 对话框


IMPLEMENT_DYNAMIC(CAnalasisDlg, CDialogEx)

CAnalasisDlg::CAnalasisDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAnalasisDlg::IDD, pParent)
{
	pCThreadWnd = NULL;
}

CAnalasisDlg::~CAnalasisDlg()
{

}

void CAnalasisDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_TOPREDICT, m_ListInput);
	DDX_Control(pDX, IDC_LIST_FINISHEDPROJECT, m_ListFinishedProject);
	DDX_Control(pDX, IDC_LIST_ANALASISRESULT, m_ListDetailResult);
}


BEGIN_MESSAGE_MAP(CAnalasisDlg, CDialogEx)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_SPECCOMPARE, &CAnalasisDlg::OnBnClickedButtonSpeccompare)
	ON_COMMAND(IDC_BUTTON_SPECCOMPARE, &CAnalasisDlg::OnButtonSpeccompare)
	ON_BN_CLICKED(IDC_BUTTON_IMPORTSPEC, &CAnalasisDlg::OnBnClickedButtonImportspec)
	ON_BN_CLICKED(IDC_BUTTON_LASTPIC, &CAnalasisDlg::OnBnClickedButtonLastpic)
	ON_BN_CLICKED(IDC_BUTTON_NEXTPIC, &CAnalasisDlg::OnBnClickedButtonNextpic)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_SENDFORANALASIS, &CAnalasisDlg::OnBnClickedButtonSendforanalasis)
	ON_MESSAGE(WM_BEGINSHOWSENDPROGRESS, &CAnalasisDlg::OnBeginshowsendprogress)
	ON_MESSAGE(WM_ENDSHOWSENDPROGRESS, &CAnalasisDlg::OnEndshowsendprogress)
	ON_MESSAGE(WM_BEGINCHECKCONNECT, &CAnalasisDlg::OnBegincheckconnect)
	ON_WM_DESTROY()
	ON_NOTIFY(NM_CLICK, IDC_LIST_FINISHEDPROJECT, &CAnalasisDlg::OnNMClickListFinishedproject)
	ON_BN_CLICKED(IDC_BUTTON_IMPORTFINISHEDPROJECT, &CAnalasisDlg::OnBnClickedButtonImportfinishedproject)
	ON_BN_CLICKED(IDC_BUTTON_PRINTRESULT, &CAnalasisDlg::OnBnClickedButtonPrintresult)
	ON_MESSAGE(WM_BEGIN_PRINTING, &CAnalasisDlg::OnBeginPrinting)
	ON_MESSAGE(WM_END_PRINTING, &CAnalasisDlg::OnEndPrinting)
	ON_MESSAGE(WM_MY_PRINT, &CAnalasisDlg::OnMyPrint)
	ON_BN_CLICKED(IDC_BUTTON_PRINTPREVIEW, &CAnalasisDlg::OnBnClickedButtonPrintpreview)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CAnalasisDlg 消息处理程序
void CAnalasisDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	if(!m_ary_PicturesPath.IsEmpty())
	{
		CRect rect;
		GetDlgItem(IDC_PICTURE)->GetWindowRect(rect);
		DisplayPic(m_ary_PicturesPath[m_PicIndex],this->GetDlgItem(IDC_PICTURE)->GetSafeHwnd(), rect.Width(), rect.Height());
	}

	GetDlgItem(IDC_STATIC_NEWPROJECT)->MoveWindow(0,0,m_rect.Width()*0.7,m_rect.Height()*0.5-5);
	GetDlgItem(IDC_LIST_TOANALASIS)->MoveWindow(10,20,m_rect.Width()*0.7-20,m_rect.Height()*0.5-5-60-10);

	GetDlgItem(IDC_BUTTON_IMPORTSPEC)->MoveWindow(10,m_rect.Height()*0.5-5-10-30,80,30);
	GetDlgItem(IDC_BUTTON_SENDFORANALASIS)->MoveWindow(100,m_rect.Height()*0.5-5-10-30,80,30);

	GetDlgItem(IDC_STATIC_DETAILRESULT)->MoveWindow(0,m_rect.Height()*0.5+5,m_rect.Width()*0.7,m_rect.Height()*0.5-5);
	GetDlgItem(IDC_LIST_ANALASISRESULT)->MoveWindow(10,m_rect.Height()*0.5+5+10+10,m_rect.Width()*0.7-20,m_rect.Height()*0.5-5-60-10);

	GetDlgItem(IDC_BUTTON_PRINTRESULT)->MoveWindow(10,m_rect.Height()-40,80,30);
	GetDlgItem(IDC_BUTTON_PRINTPREVIEW)->MoveWindow(100,m_rect.Height()-40,80,30);
	GetDlgItem(IDC_BUTTON_SPECCOMPARE)->MoveWindow(190,m_rect.Height()-40,80,30);

	GetDlgItem(IDC_PICTURE)->MoveWindow(m_rect.Width()*0.7+10,10,m_rect.Width()*0.3-20,m_rect.Height()*0.5-5-10-30-10-10);
	GetDlgItem(IDC_BUTTON_LASTPIC)->MoveWindow(m_rect.Width()*0.7+10,m_rect.Height()*0.5-5-10-30,80,30);
	GetDlgItem(IDC_BUTTON_NEXTPIC)->MoveWindow(m_rect.Width()-10-80,m_rect.Height()*0.5-5-10-30,80,30);


	GetDlgItem(IDC_STATIC_FINISHEDPROJECT)->MoveWindow(m_rect.Width()*0.7+10,m_rect.Height()*0.5+5,m_rect.Width()*0.3-20,m_rect.Height()*0.5-5);
	GetDlgItem(IDC_LIST_FINISHEDPROJECT)->MoveWindow(m_rect.Width()*0.7+10+10,m_rect.Height()*0.5+5+10+10,m_rect.Width()*0.3-20-20,m_rect.Height()*0.5-5-60-10);
	GetDlgItem(IDC_BUTTON_IMPORTFINISHEDPROJECT)->MoveWindow(m_rect.Width()*0.7+10,m_rect.Height()-40,80,30);
	GetDlgItem(IDC_BUTTON_DELETEFINISHED)->MoveWindow(m_rect.Width()-10-80,m_rect.Height()-40,80,30);


	// 不为绘图消息调用 CDialogEx::OnPaint()
}

void CAnalasisDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	GetClientRect(&m_rect);
}


BOOL CAnalasisDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  在此添加额外的初始化
	ExitAllThread=0;
	ExitThreadCheckConnect=1;
	ExitThreadReceiveResult=1;
	ExitThreadPackAndSendProject=1;

	g_hListenSocket = INVALID_SOCKET;
	g_pCAnalasisDlg = this;

	InitializeCriticalSection(&g_csFinishedProInfo);
	InitializeCriticalSection(&g_csConns);
	

	m_ListInput.InsertColumn(0,L"白酒光谱名",LVCFMT_LEFT,200);
	m_ListInput.InsertColumn(1,L"白酒名",LVCFMT_LEFT,200);
	m_ListInput.InsertColumn(2,L"白酒信息",LVCFMT_LEFT,200);

	DWORD dwStyle = m_ListInput.GetExtendedStyle(); //获取当前扩展样式
	dwStyle |= LVS_EX_FULLROWSELECT; //选中某行使整行高亮（report风格时）
	dwStyle |= LVS_EX_GRIDLINES; //网格线（report风格时）
	m_ListInput.SetExtendedStyle(dwStyle); //设置扩展风格

	//完成项目列表
	m_ListFinishedProject.InsertColumn(0,L"项目名称",LVCFMT_LEFT,100);
	m_ListFinishedProject.InsertColumn(1,L"项目发送日期",LVCFMT_LEFT,100);
	m_ListFinishedProject.InsertColumn(2,L"发送人",LVCFMT_LEFT,100);
	m_ListFinishedProject.InsertColumn(3,L"结果接收日期",LVCFMT_LEFT,100);
	m_ListFinishedProject.InsertColumn(4,L"鉴定结果",LVCFMT_LEFT,100);


	dwStyle = m_ListFinishedProject.GetExtendedStyle(); //获取当前扩展样式
	dwStyle |= LVS_EX_FULLROWSELECT; //选中某行使整行高亮（report风格时）
	dwStyle |= LVS_EX_GRIDLINES; //网格线（report风格时）
	m_ListFinishedProject.SetExtendedStyle(dwStyle); //设置扩展风格

	//详细结果列表
	m_ListDetailResult.InsertColumn(0,L"项目名称",LVCFMT_LEFT,200);
	m_ListDetailResult.InsertColumn(1,L"光谱文件名",LVCFMT_LEFT,200);
	m_ListDetailResult.InsertColumn(2,L"鉴定结果",LVCFMT_LEFT,200);

	dwStyle = m_ListDetailResult.GetExtendedStyle(); //获取当前扩展样式
	dwStyle |= LVS_EX_FULLROWSELECT; //选中某行使整行高亮（report风格时）
	dwStyle |= LVS_EX_GRIDLINES; //网格线（report风格时）
	m_ListDetailResult.SetExtendedStyle(dwStyle); //设置扩展风格

	AddFinishedPro();
	//开启接受结果线程
	pThreadReceiveResult=AfxBeginThread(ThreadReceiveResult,this);
	
	//获取CClientView指针
	CClientView *pCClientView=(CClientView*) this->GetParent();
	m_sock=pCClientView->m_sClient;

	UserName = m_UserName;
	::SendMessage(::AfxGetApp()->GetMainWnd()->m_hWnd,WM_SHOWUSERNAME,NULL,NULL);

	OnBegincheckconnect(NULL,NULL);


	::SendMessage(AfxGetApp()->GetMainWnd()->m_hWnd,WM_SHOWUSERNAME,0,0);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

int CAnalasisDlg::InitWind(void)
{
	//获取CClientView指针
	CClientView *pCClientView=(CClientView*) this->GetParent();
	m_sock=pCClientView->m_sClient;

	UserName = m_UserName;
	::SendMessage(::AfxGetApp()->GetMainWnd()->m_hWnd,WM_SHOWUSERNAME,NULL,NULL);

	AfxGetApp()->GetMainWnd()->ShowWindow(SW_SHOW);
	this->GetParent()->ShowWindow(SW_SHOW);
	this->ShowWindow(SW_SHOW);
	this->GetParent()->GetClientRect(&m_rect);
	this->MoveWindow(m_rect);
	return 0;
}



afx_msg LRESULT CAnalasisDlg::OnBegincheckconnect(WPARAM wParam, LPARAM lParam)
{
	pCThreadWnd=AfxBeginThread(ThreadCheckConnect,this->GetParent());
	return 0;
}

int CAnalasisDlg::EndCheckConnect(void)
{
	TransProject=1;
	while(!ExitThreadCheckConnect)
	{
		Sleep(200);
	}
	if(NULL!=pCThreadWnd)
	{
		TerminateThread(pCThreadWnd->m_hThread,100);
		pCThreadWnd = NULL;
	}
	return 0;
}

UINT ThreadCheckConnect(LPVOID lparam)
{
	CClientView *pCClientView=(CClientView*) lparam;
	char Buf[DATA_BUFSIZE] = {0};
	int count = 0;	
	TransProject=0;
	while(!TransProject)
	{
		if(ExitAllThread)
		{	
			ExitThreadCheckConnect=1;
			return -2;
		}
		while(!TransProject)
		{
			if(ExitAllThread)
			{	
				ExitThreadCheckConnect=1;
				return -2;
			}
			memset(Buf,0,DATA_BUFSIZE);
			memcpy(Buf,"Connect",strlen("Connect"));
			int retval=send(pCClientView->m_sClient,Buf,DATA_BUFSIZE,0);
			count=0;
			if(SOCKET_ERROR==retval)
			{
				int err=WSAGetLastError();
				if(err==WSAEWOULDBLOCK)
				{
					Sleep(500);
					continue;
				}
				else
				{
					closesocket(pCClientView->m_sClient);
					//重新登录
					MessageBox(pCClientView->m_hWnd,L"与服务器连接超时，请重新登录！",L"连接失败",MB_ICONERROR);
					pCClientView->isDlgLogin=true;
					pCClientView->isDlgAnalasis=false;
					pCClientView->isSpecCompareDlg=false;
					/*ExitAllThread=1;
					closesocket(g_hListenSocket);
					while(!(ExitThreadReceiveResult&ExitThreadPackAndSendProject&(!g_ThreadRecvingCount)))
					{
						Sleep(100);
					}
					if(rsa_publickey)
					{
						RSA_free(rsa_publickey);
						rsa_publickey = NULL;
					}
					DeleteCriticalSection(&g_csFinishedProInfo);*/
					::SendMessage(pCClientView->m_hWnd,WM_UISELECT,NULL,NULL);
					ExitThreadCheckConnect=1;
					return -1;
				}
			}
			break;
		}
		Sleep(1000);
	}
	ExitThreadCheckConnect=1;
	return 0;
}


void CAnalasisDlg::OnBnClickedButtonSpeccompare()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_ListFinishedProject.GetSelectedCount()==0)
	{
		MessageBox(L"请选择一个项目！",L"人工辅助分析",MB_ICONWARNING);
		return;
	}
	int nIndex;
	nIndex=m_ListFinishedProject.GetSelectionMark();

	CSpectrumCompareDlg SpectrumCompareDlg;

	EnterCriticalSection(&g_csFinishedProInfo);
	SpectrumCompareDlg.m_projectName=m_ary_FinishedPro[nIndex].m_FinishedProjectName;
	SpectrumCompareDlg.m_projectPath=m_ary_FinishedPro[nIndex].m_FinishedProjectPath;
	LeaveCriticalSection(&g_csFinishedProInfo);

	SpectrumCompareDlg.DoModal();	
}


void CAnalasisDlg::OnButtonSpeccompare()
{
	// TODO: 在此添加命令处理程序代码
	OnBnClickedButtonSpeccompare();
}


void CAnalasisDlg::OnBnClickedButtonImportspec()
{
	// TODO: 在此添加控件通知处理程序代码
	BROWSEINFO bi;
	char Buffer[MAX_PATH];
	bi.hwndOwner = this->m_hWnd;
	bi.pidlRoot =NULL;
	bi.pszDisplayName =(LPWSTR)Buffer;
	bi.lpszTitle = L"选择光谱数据集文件夹路径";
	bi.ulFlags =BIF_USENEWUI ; 
	bi.lpfn = NULL;
	bi.lParam = 0;
	bi.iImage=IDR_MAINFRAME;
	//初始化入口参数bi结束
	LPITEMIDLIST pIDList = SHBrowseForFolder(&bi);

	if(pIDList==NULL)
	{	
		return;
	}
	SHGetPathFromIDList(pIDList,(LPWSTR)Buffer);
	//取得文件夹路径到Buffer里
	m_SpecsToAnaFolderPath.Format(L"%s",Buffer);
	// free memory used      
	IMalloc * imalloc = 0;
	if (SUCCEEDED(SHGetMalloc(&imalloc)))
	{
		imalloc->Free(pIDList);
		imalloc->Release();
	}
	//从文件夹中导入光谱数据及相关信息
	CAddSpecToAnaInfo AddSpecToAnaInfo;

	if(IDOK==AddSpecToAnaInfo.DoModal())
	{
		m_WineName=AddSpecToAnaInfo.m_Winename;
		m_AlcoholContent=AddSpecToAnaInfo.m_Alcoholcontent;
		m_Flavour=AddSpecToAnaInfo.m_Flavour;
		m_Brand=AddSpecToAnaInfo.m_Brand;
		CString strDir=m_SpecsToAnaFolderPath;

		strDir.Replace(L"\\",L"\\\\");
		strDir += "\\*.*";
		// 遍历得到所有子文件夹名
		CFileFind finder; 
		BOOL bWorking = finder.FindFile(strDir);

		m_ary_PicturesPath.RemoveAll();
		m_ary_SpecFileTitle.RemoveAll();
		while (bWorking)  
		{
			bWorking = finder.FindNextFile();
			CString str=finder.GetFileName();
			if(str.Right(4)==L".CSV"||str.Right(4)==L".csv" &&L"." !=str &&L".." != str)//注意该句需要排除“.”“..”
			{
				CString filename;
				CString fileTitle;

				m_ary_SpecFilePath.Add(finder.GetFilePath());
				m_ary_SpecFileName.Add(finder.GetFileName());
				//获取文件名(不包含后缀)  
				//采用CString的Left(int count)截取CString中从左往右数的count个字符  
				//fileName.GetLength()-4中的4表示".csv"四个字符  
				m_ary_SpecFileTitle.Add(finder.GetFileTitle());//将文件名(不包含后缀)添加到数组中  
			}
			if(str.Right(4)==L".jpg" ||str.Right(4)==L".bmp"||str.Right(4)==L".gif"&&L"." !=str &&L".." != str)
			{
				m_ary_PicturesName.Add(finder.GetFileName());;
				m_ary_PicturesPath.Add(finder.GetFilePath());
			}
		}
		finder.Close();
		if(m_ary_SpecFileTitle.GetCount()==0)
		{
			m_WineName="";
			m_AlcoholContent="";
			m_Flavour="";
			m_Brand="";
			m_ary_SpecFileTitle.RemoveAll();
			m_ary_PicturesPath.RemoveAll();
			MessageBox(L"该文件夹中没有规定格式的光谱数据，请重新选择！",L"警告",MB_ICONWARNING);

			return;
		}
		if(m_ary_SpecFileTitle.GetCount()<10)
		{			
			m_WineName="";
			m_AlcoholContent="";
			m_Flavour="";
			m_Brand="";
			m_ary_SpecFileTitle.RemoveAll();
			m_ary_PicturesPath.RemoveAll();
			MessageBox(L"光谱数量太少",L"警告",MB_ICONWARNING);
			return;
		}
		m_PicIndex=0;
		SortCArray(m_ary_SpecFileTitle);

		for(int i=0;i<m_ary_SpecFileTitle.GetCount();i++)
		{
			CString Wineinfo=L"";
			Wineinfo.Format(L"%s度,%s,%s",m_AlcoholContent,m_Flavour,m_Brand);
			m_ListInput.InsertItem(i,m_ary_SpecFileTitle[i]);
			m_ListInput.SetItemText(i,1,m_WineName);
			m_ListInput.SetItemText(i,2,Wineinfo);
		}

	}
}


void CAnalasisDlg::OnBnClickedButtonLastpic()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_PicIndex==0)
	{
		m_PicIndex=m_ary_PicturesPath.GetCount()-1;
	}
	else
	{
		m_PicIndex--;
	}
	OnPaint();	
}


void CAnalasisDlg::OnBnClickedButtonNextpic()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_PicIndex==m_ary_PicturesPath.GetCount()-1)
	{
		m_PicIndex=0;
	}
	else
	{
		m_PicIndex++;
	}
	OnPaint();
}
void CAnalasisDlg::OnBnClickedButtonSendforanalasis()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_ary_SpecFilePath.GetCount() == 0)
	{
		MessageBox(L"请先导入光谱！",L"发送项目",MB_ICONWARNING);
		return;
	}

	CProjectInfoWriteDlg ProjectInfoWriteDlg;
	ProjectInfoWriteDlg.m_WineName=m_WineName;
	ProjectInfoWriteDlg.m_AlcoholContent=m_AlcoholContent;
	ProjectInfoWriteDlg.m_Flavour=m_Flavour;
	ProjectInfoWriteDlg.m_Brand=m_Brand;

	if(IDOK==ProjectInfoWriteDlg.DoModal())
	{
		//打包项目
		CProjectInfoConfirmDlg ProjectInfoConfirmDlg;
		ProjectInfoConfirmDlg.m_ProjectName=ProjectInfoWriteDlg.m_ProjectName;
		ProjectInfoConfirmDlg.m_WineName=ProjectInfoWriteDlg.m_WineName;
		ProjectInfoConfirmDlg.m_AlcoholContent=ProjectInfoWriteDlg.m_AlcoholContent;
		ProjectInfoConfirmDlg.m_Flavour=ProjectInfoWriteDlg.m_Flavour;
		ProjectInfoConfirmDlg.m_Brand=ProjectInfoWriteDlg.m_Brand;
		ProjectInfoConfirmDlg.m_MailAddr=ProjectInfoWriteDlg.m_MailAddr;
		ProjectInfoConfirmDlg.m_SenderName=ProjectInfoWriteDlg.m_SenderName;
		ProjectInfoConfirmDlg.m_AnaType=ProjectInfoWriteDlg.m_AnaType;

		if(IDOK==ProjectInfoConfirmDlg.DoModal())
		{
			m_ProjectName=ProjectInfoWriteDlg.m_ProjectName;
			m_WineName=ProjectInfoWriteDlg.m_WineName;
			m_AlcoholContent=ProjectInfoWriteDlg.m_AlcoholContent;
			m_Flavour=ProjectInfoWriteDlg.m_Flavour;
			m_Brand=ProjectInfoWriteDlg.m_Brand;
			m_MailAddr=ProjectInfoWriteDlg.m_MailAddr;
			m_SenderName=ProjectInfoWriteDlg.m_SenderName;
			m_AnaType=ProjectInfoWriteDlg.m_AnaType;
			//打包并发送项目
			AfxBeginThread(ThreadPackAndSendProject,this);
		}
	}
}
//创建发送项目进度窗口
afx_msg LRESULT CAnalasisDlg::OnBeginshowsendprogress(WPARAM wParam, LPARAM lParam)
{
	pCProjectSendProgressDlg=new CProjectSendProgressDlg;

	pCProjectSendProgressDlg->m_CurrentProgress=1;
	pCProjectSendProgressDlg->m_MaxRange=0;
	pCProjectSendProgressDlg->m_SendState=L"正在准备项目";
	pCProjectSendProgressDlg->m_SendPercent=L"0%";
	pCProjectSendProgressDlg->m_ProjectSize=L"0 kb";
	pCProjectSendProgressDlg->m_SendedSize=L"0 kb";
	pCProjectSendProgressDlg->Create(IDD_DIALOG_PROJECTSENDPROGRESS,this);
	pCProjectSendProgressDlg->ShowWindow(SW_SHOW);

	return 0;
}
afx_msg LRESULT CAnalasisDlg::OnEndshowsendprogress(WPARAM wParam, LPARAM lParam)
{
	if(NULL!=pCProjectSendProgressDlg)
	{
		pCProjectSendProgressDlg->DestroyWindow();
		pCProjectSendProgressDlg=NULL;
	}

	return 0;
}



//打包发送项目
UINT ThreadPackAndSendProject(LPVOID lparam)
{
	AfxOleInit();
	ExitThreadPackAndSendProject=0;

	//获取CAnalasisDlg指针
	CAnalasisDlg *pCAnalasisDlg=(CAnalasisDlg*) lparam;

	//获取CClientView指针
	CClientView *pCClientView=(CClientView*) pCAnalasisDlg->GetParent();


	::SendMessage(pCAnalasisDlg->m_hWnd,WM_BEGINSHOWSENDPROGRESS,0,0);
	//::SendMessage();

	//创建压缩包
	CString strFolder;
	strFolder.Format(L"tempFile\\%s",pCAnalasisDlg->m_ProjectName);
	CreateDirectory(strFolder,NULL);

	CString strDir=strFolder;
	strDir+=L"\\";
	/*for(int i=0;i<pCAnalasisDlg->m_ary_SpecFilePath.GetCount();i++)
	{
		CString newPath=strDir+pCAnalasisDlg->m_ary_SpecFileName[i];
		CopyFile(pCAnalasisDlg->m_ary_SpecFilePath[i],newPath,0);
	}*/

	for(int i=0;i<pCAnalasisDlg->m_ary_SpecFilePath.GetCount();i++)
	{
		CString newPath=strDir+pCAnalasisDlg->m_ary_SpecFileName[i];
		CopyFile(pCAnalasisDlg->m_ary_SpecFilePath[i],newPath,0);
	}
	for(int i=0;i<pCAnalasisDlg->m_ary_PicturesPath.GetCount();i++)
	{
		CString newPath=strDir+pCAnalasisDlg->m_ary_PicturesName[i];
		CopyFile(pCAnalasisDlg->m_ary_PicturesPath[i],newPath,0);
	}


	CString projectInfoPath=strDir;
	projectInfoPath+=L"项目信息.xlsx";
	
	//项目信息表格备份绝对路径
	CString strprojectInfoBackupFullPath=g_CurrentDir;
	strprojectInfoBackupFullPath+="\\tempFile\\项目信息.xlsx";

	//获取项目文件夹绝对路径
	CString strFolderFullPath=g_CurrentDir;		
	strFolderFullPath+="\\tempFile\\";
	strFolderFullPath+=pCAnalasisDlg->m_ProjectName;

	//新生成的项目信息表格绝对路径
	CString strprojectInfoFullPath=strFolderFullPath;
	strprojectInfoFullPath+="\\项目信息.xlsx";

	CExcelOperator ExcelOperator;
	ExcelOperator.LoadExcel(strprojectInfoBackupFullPath);
	ExcelOperator.SetItem(1,1,pCAnalasisDlg->m_ProjectName);

	//用户名
	ExcelOperator.SetItem(2,1,pCAnalasisDlg->m_UserName);
	ExcelOperator.SetItem(3,1,pCAnalasisDlg->m_WineName);
	ExcelOperator.SetItem(4,1,pCAnalasisDlg->m_AlcoholContent);
	ExcelOperator.SetItem(5,1,pCAnalasisDlg->m_Flavour);
	ExcelOperator.SetItem(6,1,pCAnalasisDlg->m_Brand);
	ExcelOperator.SetItem(7,1,pCAnalasisDlg->m_MailAddr);
	ExcelOperator.SetItem(8,1,pCAnalasisDlg->m_SenderName);
	ExcelOperator.SetItem(9,1,pCAnalasisDlg->m_AnaType);

	//日期
	SYSTEMTIME sys; 
	GetLocalTime(&sys); 

	CString sTime;
	sTime.Format(L"%d-%d-%d",sys.wYear,sys.wMonth,sys.wDay);

	ExcelOperator.SetItem(10,1,sTime);

	ExcelOperator.SaveExcel(strprojectInfoFullPath);

	ExcelOperator.ReleaseExcel();

	CZipImplement zipObj;
	CString zipFilePath=strFolder;
	zipFilePath+=L".zip";

	pCAnalasisDlg->pCProjectSendProgressDlg->m_CurrentProgress=1;
	pCAnalasisDlg->pCProjectSendProgressDlg->m_MaxRange=0;
	pCAnalasisDlg->pCProjectSendProgressDlg->m_SendState=L"打包中";
	pCAnalasisDlg->pCProjectSendProgressDlg->m_SendPercent=L"0%";
	pCAnalasisDlg->pCProjectSendProgressDlg->m_ProjectSize=L"0 kb";
	pCAnalasisDlg->pCProjectSendProgressDlg->m_SendedSize=L"0 kb";


	::SendMessageA(pCAnalasisDlg->pCProjectSendProgressDlg->m_hWnd,WM_SHOWSENDPROGRESS,NULL,NULL);

	//压缩
	int retn=zipObj.Zip_PackFiles(strFolderFullPath,zipFilePath);

	bool fla=DeleteFolder(strFolderFullPath);

	//项目名称
	CString projectName=pCAnalasisDlg->m_ProjectName;
	//加后缀
	projectName+=".zip";
	int count;//计数器

	//缓存（每次发送的大小）
	char buf[DATA_BUFSIZE];
	long filelen;
	int len;

	SOCKET m_sock=pCAnalasisDlg->m_sock;

	ifstream infile(zipFilePath,ios::in|ios::binary);
	infile.seekg(0,ios::end);
	filelen=infile.tellg();
	infile.seekg(0,ios::beg);
	//开始传送项目
	//关闭连接检测线程
	pCAnalasisDlg->EndCheckConnect();
	
	memset(buf,0,DATA_BUFSIZE);
	memcpy(buf,"TransmitProjet",strlen("TransmitProjet"));

	while(true)
	{
		if(ExitAllThread)
		{
			infile.close();
			closesocket(m_sock);
			ExitThreadPackAndSendProject=1;
			return -2;
		}
		int retVal=send(m_sock,buf,DATA_BUFSIZE,0);
		if(SOCKET_ERROR==retVal)
		{
			int err=WSAGetLastError();
			if(err==WSAEWOULDBLOCK)
			{
				Sleep(100);
				continue;
			}
			else
			{
				infile.close();
				closesocket(m_sock);				
				::SendMessage(pCAnalasisDlg->m_hWnd,WM_BEGINCHECKCONNECT,NULL,NULL);
				::SendMessage(pCAnalasisDlg->m_hWnd,WM_ENDSHOWSENDPROGRESS,0,0);
				ExitThreadPackAndSendProject=1;
				Sleep(100);
				return -1;
			}
		}
		break;
	}
	//发送文件名
	char *ProName=CString2Char(projectName);
	memset(buf,0,DATA_BUFSIZE);
	memcpy(buf,ProName,strlen(ProName));

	count=0;
	while(true)
	{
		if(ExitAllThread)
		{
			infile.close();
			closesocket(m_sock);
			ExitThreadPackAndSendProject=1;
			return -2;
		}

		int len=strlen(ProName);
		int retVal=send(m_sock,buf,DATA_BUFSIZE,0);
		if(SOCKET_ERROR==retVal)
		{
			int err=WSAGetLastError();
			if(err==WSAEWOULDBLOCK)
			{
				count++;
				Sleep(100);
				continue;
			}
			else
			{	
				infile.close();
				DeleteFile(zipFilePath);
				closesocket(m_sock);				
				//::SendMessage(pCAnalasisDlg->m_hWnd,WM_BEGINCHECKCONNECT,NULL,NULL);
				::SendMessage(pCAnalasisDlg->m_hWnd,WM_ENDSHOWSENDPROGRESS,0,0);
				Sleep(100);
				ExitThreadPackAndSendProject=1;
				return -1;
			}
		}
		break;
	}	
	free(ProName);
	ProName = NULL;
	Sleep(200);
	//将要发送的文件长度发送给客户端
	memset(buf,0,DATA_BUFSIZE);
	sprintf(buf,"%ld",filelen);

	while(true)
	{
		if(ExitAllThread)
		{
			infile.close();
			DeleteFile(zipFilePath);
			closesocket(m_sock);
			ExitThreadPackAndSendProject=1;
			return -2;
		}
		int retVal=send(m_sock,buf,DATA_BUFSIZE,NULL);
		if(SOCKET_ERROR==retVal)
		{
			int err=WSAGetLastError();
			if(err==WSAEWOULDBLOCK)
			{
				count++;
				Sleep(100);
				continue;
			}
			else
			{
				//连接错误
				infile.close();
				DeleteFile(zipFilePath);
				closesocket(m_sock);				
				pCClientView->TransProject=0;
				Sleep(100);
				ExitThreadPackAndSendProject=1;
				return -1;
			}
		}
		break;
	}
	Sleep(200);
	char sSendedSize[100];
	int nFile=1;

	pCAnalasisDlg->pCProjectSendProgressDlg->m_CurrentProgress=1;
	pCAnalasisDlg->pCProjectSendProgressDlg->m_MaxRange=filelen/DATA_BUFSIZE+1;
	pCAnalasisDlg->pCProjectSendProgressDlg->m_SendState=L"发送";
	pCAnalasisDlg->pCProjectSendProgressDlg->m_SendPercent=L"0%";
	pCAnalasisDlg->pCProjectSendProgressDlg->m_ProjectSize.Format(L"总大小：%ld kb",filelen%1024==0?filelen/1024:filelen/1024+1);
	pCAnalasisDlg->pCProjectSendProgressDlg->m_SendedSize=L"已发送：0 kb";

	::SendMessageA(pCAnalasisDlg->pCProjectSendProgressDlg->m_hWnd,WM_SHOWSENDPROGRESS,NULL,NULL);

	int SendedSize=0;
	while(!infile.eof())
	{
		if(ExitAllThread)
		{
			infile.close();
			closesocket(m_sock);
			ExitThreadPackAndSendProject=1;
			return -2;
		}

		sprintf(sSendedSize,"%d",SendedSize);
		pCAnalasisDlg->pCProjectSendProgressDlg->m_CurrentProgress=nFile;
		float Percent=((float)SendedSize/filelen)*100;
		pCAnalasisDlg->pCProjectSendProgressDlg->m_SendPercent.Format(L"%d",(int)Percent);
		pCAnalasisDlg->pCProjectSendProgressDlg->m_SendPercent+=L"%";
		pCAnalasisDlg->pCProjectSendProgressDlg->m_SendedSize.Format(L"已发送：%d kb",(SendedSize%1024==0?((int)SendedSize/1024):((int)(SendedSize/1024+1))));

		::SendMessageA(pCAnalasisDlg->pCProjectSendProgressDlg->m_hWnd,WM_SHOWSENDPROGRESS,NULL,NULL);

		//发送
		memset(buf,0,DATA_BUFSIZE);
		infile.read(buf,DATA_BUFSIZE);
		len=infile.gcount();				
		while(true)
		{
			if(ExitAllThread)
			{
				infile.close();
				closesocket(m_sock);
				ExitThreadPackAndSendProject=1;
				return -2;
			}
			int retVal=send(m_sock,buf,len,0);
			if(SOCKET_ERROR==retVal)
			{
				int err=WSAGetLastError();
				if(err==WSAEWOULDBLOCK)
				{
					count++;
					Sleep(100);
					continue;
				}
				else
				{
					closesocket(m_sock);
					infile.close();
					infile.clear();
					::SendMessage(pCAnalasisDlg->m_hWnd,WM_BEGINCHECKCONNECT,NULL,NULL);
					::SendMessage(pCAnalasisDlg->m_hWnd,WM_ENDSHOWSENDPROGRESS,0,0);
					Sleep(200);	
					ExitThreadPackAndSendProject=1;
					return -1;
				}
			}
			break;
		}
		SendedSize+=len;
		nFile++;
		Sleep(200);
	}

	float Percent=((float)SendedSize/filelen)*100;
	pCAnalasisDlg->pCProjectSendProgressDlg->m_SendPercent.Format(L"%d",(int)Percent);
	pCAnalasisDlg->pCProjectSendProgressDlg->m_SendPercent+=L"%";
	pCAnalasisDlg->pCProjectSendProgressDlg->m_SendedSize.Format(L"已发送：%d kb",(SendedSize%1024==0?((int)SendedSize/1024):((int)(SendedSize/1024+1))));
	::SendMessageA(pCAnalasisDlg->pCProjectSendProgressDlg->m_hWnd,WM_SHOWSENDPROGRESS,NULL,NULL);
	infile.close();
	Sleep(200);
	count=0;
	while(true)
	{
		if(ExitAllThread)
		{
			closesocket(m_sock);
			ExitThreadPackAndSendProject=1;
			return -2;
		}
		ZeroMemory(buf,DATA_BUFSIZE);
		int retVal=recv(m_sock,buf,DATA_BUFSIZE,0);
		if(SOCKET_ERROR==retVal)
		{
			int err=WSAGetLastError();
			if(err==WSAEWOULDBLOCK)
			{
				count++;
				Sleep(100);
				continue;
			}
			else if(err == WSAETIMEDOUT || err == WSAENETDOWN)
			{	
				::SendMessage(pCAnalasisDlg->m_hWnd,WM_BEGINCHECKCONNECT,NULL,NULL);
				::SendMessage(pCAnalasisDlg->m_hWnd,WM_ENDSHOWSENDPROGRESS,0,0);
				ExitThreadPackAndSendProject=1;
				Sleep(200);	
				return -1;
			}
		}
		break;
	}
	if(strcmp(buf,"Success")==0)
	{
		::SendMessage(pCAnalasisDlg->m_hWnd,WM_BEGINCHECKCONNECT,NULL,NULL);
		::SendMessage(pCAnalasisDlg->m_hWnd,WM_ENDSHOWSENDPROGRESS,0,0);
		Sleep(200);
		MessageBox(pCAnalasisDlg->m_hWnd,L"项目发送成功！请耐心等待鉴定结果",L"发送成功",MB_ICONINFORMATION);
	}
	else if(strcmp(buf,"Failed")==0)
	{
		::SendMessage(pCAnalasisDlg->m_hWnd,WM_BEGINCHECKCONNECT,NULL,NULL);
		::SendMessage(pCAnalasisDlg->m_hWnd,WM_ENDSHOWSENDPROGRESS,0,0);
		Sleep(200);
		MessageBox(pCAnalasisDlg->m_hWnd,L"项目发送失败！请重新发送",L"发送失败",MB_ICONERROR);
	}
	//删除已发送项目
	::SendMessage(pCAnalasisDlg->m_hWnd,WM_BEGINCHECKCONNECT,NULL,NULL);
	ExitThreadPackAndSendProject=1;
	Sleep(200);
	return 0;
}
//采用非阻塞模式处理客户端的通信
SOCKET BindListen()
{
	SOCKET sd = socket(AF_INET,SOCK_STREAM,0);
	if(sd == INVALID_SOCKET)
	{
		int lastErr = WSAGetLastError();
		return INVALID_SOCKET;
	}

	sockaddr_in saListen;
	saListen.sin_family = AF_INET;
	saListen.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	saListen.sin_port = htons(CLIENT_RECV_PORT);

	if(SOCKET_ERROR == bind(sd,(sockaddr *)&saListen,sizeof(saListen)))
	{
		int lastErr = WSAGetLastError();
		closesocket(sd);
		return INVALID_SOCKET;
	}
	if(listen(sd,5) == SOCKET_ERROR)
	{
		int lastErr = WSAGetLastError();
		closesocket(sd);
		return INVALID_SOCKET;
	}

	return sd;
}

SOCKET AcceptConnection(SOCKET sdListen,sockaddr_in &saRemote)
{	
	int nSize = sizeof(saRemote);
	SOCKET sa = accept(sdListen,(sockaddr *)&saRemote,&nSize);
	return sa;
}
void AddToList(IOContext *pIOContext)
{
	EnterCriticalSection(&g_csConns);
	ConnectionList::iterator it = g_conns.begin();
	g_conns.push_back(pIOContext);
	LeaveCriticalSection(&g_csConns);
}

void RemoveFromList(IOContext *pIOContext)
{
	EnterCriticalSection(&g_csConns);
	//删除用户列表中相关的项
	ConnectionList::iterator it = g_conns.begin();
	while(it != g_conns.end())
	{
		if(*it == pIOContext)
		{
			g_conns.erase(it);
			break;
		}
		++it;
	}
	LeaveCriticalSection(&g_csConns);
}

void CloseIO(IOContext *pIOContext)
{
	closesocket(pIOContext->hSocket);
	pIOContext->hSocket = INVALID_SOCKET;
	if(pIOContext->oFile.is_open())
	{
		pIOContext->oFile.close();
		pIOContext->oFile.clear();
	}
	RemoveFromList(pIOContext);
	delete pIOContext;
}
UINT ThreadReceiveResult(LPVOID lpParam)
{
	ExitThreadReceiveResult=0;
	CAnalasisDlg *pCAnalasisDlg=(CAnalasisDlg *)lpParam;

	HANDLE hThreads[MAX_THREAD_NUM];
	for(int i= 0;i<MAX_THREAD_NUM;++i)
	{
		hThreads[i] = INVALID_HANDLE_VALUE;
	}

	DWORD dwThreadCount = 0;
	

	g_hListenSocket = BindListen();

	if(g_hListenSocket == INVALID_SOCKET)
		goto cleanup;

	HANDLE hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE,NULL,0,0);

	if(hIOCP == NULL)
	{
		goto cleanup;
	}

	SYSTEM_INFO systemInfo;
	GetSystemInfo(&systemInfo);
	DWORD dwCPU = systemInfo.dwNumberOfProcessors;

	for(dwThreadCount = 0;dwThreadCount < dwCPU*2;++dwThreadCount)
	{
		unsigned int dwThreadID = 0;
		HANDLE hThread = (HANDLE)_beginthreadex(NULL,0,(PTHREAD_START)ThreadRecv,hIOCP,0,&dwThreadID);
		if(hThread == NULL)
		{
			goto cleanup;
		}
		hThreads[dwThreadCount] = hThread;
	}
	while(!ExitAllThread)
	{
		sockaddr_in saRemote;
		SOCKET hAcceptSocket = AcceptConnection(g_hListenSocket,saRemote);

		if(hAcceptSocket == INVALID_SOCKET)
		{
			goto cleanup;
		}
		hIOCP = CreateIoCompletionPort((HANDLE)hAcceptSocket,hIOCP,0,0);

		if(hIOCP == NULL)
		{
			goto cleanup;
		}
		IOContext *pIOContext = new IOContext(hAcceptSocket);
		AddToList(pIOContext);
		//接收文件大小
		DWORD dwFlags = 0;
		int nRet = WSARecv(hAcceptSocket,
			&(pIOContext->wsaBuffer),
			1,
			NULL,
			&dwFlags,
			(LPWSAOVERLAPPED)pIOContext,NULL);

		int lastErr = WSAGetLastError();

		if(nRet == SOCKET_ERROR)
		{
			if(ERROR_IO_PENDING != lastErr)
			{
				goto cleanup;
			}
		}

	}
cleanup:
	if(hIOCP != NULL)
	{
		for(DWORD i = 0;i<dwThreadCount;++i)
		{
			PostQueuedCompletionStatus(hIOCP,0,EXIT_THREADRECV,NULL);
		}
	}

	if(dwThreadCount>0)
	{
		WaitForMultipleObjects(dwThreadCount,hThreads,TRUE,INFINITE);
	}
	for(DWORD i = 0;i<dwThreadCount;++i)
	{
		CloseHandle(hThreads[i]);
	}

	EnterCriticalSection(&g_csConns);
	ConnectionList::iterator it = g_conns.begin();
	while(it!=g_conns.end())
	{
		closesocket((*it)->hSocket);
		if((*it)->oFile.is_open())
		{
			(*it)->oFile.close();
			(*it)->oFile.clear();
		}
		delete *it;
		++it;
	}
	g_conns.clear();

	LeaveCriticalSection(&g_csConns);

	if(g_hListenSocket != INVALID_SOCKET)
	{
		closesocket(g_hListenSocket);
		//g_hListenSocket = INVALID_SOCKET;
	}
	ExitThreadReceiveResult=1;
	return 0;
}
DWORD WINAPI ThreadRecv(LPVOID param)//UINT ThreadRecv(LPVOID pContext)
{
	CoInitialize(NULL);
	HANDLE hIOCP = (HANDLE)param;
	IOContext *pIOContext = NULL;
	DWORD dwBytes = 0;
	ULONG ulKey = 0;
	DWORD dwFlags = 0;
	int nRet = 0;
	while(true)
	{
		BOOL bOK = GetQueuedCompletionStatus(hIOCP,
			&dwBytes,
			&ulKey,
			(LPOVERLAPPED*)&pIOContext,
			INFINITE);

		if(bOK && ulKey == EXIT_THREADRECV)
		{
			break;
		}

		if(!bOK)
		{
			if(pIOContext->oFile.is_open())
			{
				pIOContext->oFile.close();
				pIOContext->oFile.clear();
			}
			CloseIO(pIOContext);
			continue;
		}

		if(dwBytes == 0)
		{
			if(pIOContext->oFile.is_open())
			{
				pIOContext->oFile.close();
				pIOContext->oFile.clear();
			}
			CloseIO(pIOContext);
			continue;
		}
		if(pIOContext->op == IOFileSize)
		{
			pIOContext->nBytesFileToRecv = atoi(pIOContext->Buffer);//要接收文件大小

			ZeroMemory(pIOContext->Buffer,DATA_BUFSIZE);//清空缓冲区

			pIOContext->nBytesInBuf = 0;// += dwBytes;
			pIOContext->wsaBuffer.buf = pIOContext->Buffer;
			pIOContext->wsaBuffer.len = DATA_BUFSIZE;//pIOContext->nBytesInBuf;

			ZeroMemory(pIOContext,sizeof(WSAOVERLAPPED));
			pIOContext->op = IORecvFileName;//接收文件名
			dwFlags = 0;
			nRet = WSARecv(pIOContext->hSocket,&(pIOContext->wsaBuffer),1,NULL,&dwFlags,pIOContext,NULL);
			int lastErr = WSAGetLastError();
			if(nRet == SOCKET_ERROR&&WSA_IO_PENDING != lastErr)
			{
				if(pIOContext->oFile.is_open())
				{
					pIOContext->oFile.close();
					pIOContext->oFile.clear();
				}
				CloseIO(pIOContext);
		    }
		}
		else if(pIOContext->op == IORecvFileName)
		{
			pIOContext->FileName = Char2CString(pIOContext->Buffer);//文件名

			CString ProjectName=pIOContext->FileName.Left(pIOContext->FileName.Find(L"."));

			//保存路径并创建文件
			strcat(pIOContext->FilePath,pIOContext->Buffer);	
			pIOContext->oFile.open(pIOContext->FilePath,ios::out|ios::binary);


			ZeroMemory(pIOContext->Buffer,DATA_BUFSIZE);//清空缓冲区

			pIOContext->nBytesInBuf = 0;
			pIOContext->wsaBuffer.buf = pIOContext->Buffer;
			pIOContext->wsaBuffer.len = DATA_BUFSIZE;//pIOContext->nBytesInBuf;

			ZeroMemory(pIOContext,sizeof(WSAOVERLAPPED));
			pIOContext->op = IORecvFile;  //开始接收文件
			dwFlags = 0;
			nRet = WSARecv(pIOContext->hSocket,&(pIOContext->wsaBuffer),1,NULL,&dwFlags,pIOContext,NULL);
			int lastErr = WSAGetLastError();
			if(nRet == SOCKET_ERROR&&WSA_IO_PENDING != lastErr)
			{
				if(pIOContext->oFile.is_open())
				{
					pIOContext->oFile.close();
					pIOContext->oFile.clear();
				}
				CloseIO(pIOContext);
		    }
		}
		else if(pIOContext->op == IORecvFile)//接收项目文件
		{
			//循环接收文件
			pIOContext->oFile.write(pIOContext->Buffer,dwBytes);
			pIOContext->nBytesFileReceived+=dwBytes;
			ZeroMemory(pIOContext->Buffer,DATA_BUFSIZE);//清空缓冲区

			if(pIOContext->nBytesFileReceived<pIOContext->nBytesFileToRecv)
			{
				//接收未完成
				ZeroMemory(pIOContext->Buffer,DATA_BUFSIZE);//清空缓冲区
				pIOContext->nBytesInBuf = 0;//
				pIOContext->wsaBuffer.buf = pIOContext->Buffer;
				pIOContext->wsaBuffer.len = DATA_BUFSIZE;//pIOContext->nBytesInBuf;

				ZeroMemory(pIOContext,sizeof(WSAOVERLAPPED));
				pIOContext->op = IORecvFile;//要求接收文件
				dwFlags = 0;
				nRet = WSARecv(pIOContext->hSocket,&(pIOContext->wsaBuffer),1,NULL,&dwFlags,pIOContext,NULL);
				int lastErr = WSAGetLastError();
				if(nRet == SOCKET_ERROR&&WSA_IO_PENDING != lastErr)
				{
					if(pIOContext->oFile.is_open())
					{
						pIOContext->oFile.close();
						pIOContext->oFile.clear();
					}
					CloseIO(pIOContext);
				}
			}
			else  
			{
				//接收完成
				pIOContext->oFile.close();
				pIOContext->oFile.clear();

				CString _currentDir;
				GetCurrentDirectory(MAX_PATH,_currentDir.GetBuffer(MAX_PATH));
				_currentDir.ReleaseBuffer(); 

				CString FilePath=_currentDir;
				FilePath+=L"\\";
				CString tempStr=Char2CString(pIOContext->FilePath);
				FilePath+=tempStr;

				CString DirUnpack =_currentDir;
				DirUnpack+=L"\\FinishedPro";

				CZipImplement ZipImplement;
				if(ZipImplement.Zip_UnPackFiles(FilePath,DirUnpack)==FALSE)
				{
					int err = GetLastError();
					//解压缩失败
					ZeroMemory(pIOContext->Buffer,DATA_BUFSIZE);//清空缓冲区
					memcpy(pIOContext->Buffer,"Failed",sizeof("Failed"));
					pIOContext->nBytesInBuf = DATA_BUFSIZE;//sizeof("Failed");
					pIOContext->wsaBuffer.buf = pIOContext->Buffer;
					pIOContext->wsaBuffer.len = DATA_BUFSIZE;//sizeof("Failed");

					ZeroMemory(pIOContext,sizeof(WSAOVERLAPPED));
					pIOContext->op = IOStatus;//要求接收文件名
					dwFlags = 0;
					nRet = WSASend(pIOContext->hSocket,&(pIOContext->wsaBuffer),1,NULL,dwFlags,pIOContext,NULL);
					int lastErr = WSAGetLastError();
					if(nRet == SOCKET_ERROR&&WSA_IO_PENDING != lastErr)
					{
						if(pIOContext->oFile.is_open())
						{
							pIOContext->oFile.close();
							pIOContext->oFile.clear();
						}
						CloseIO(pIOContext);
					}
				}
				else
				{
					//解压缩成功
					CString DirNew=L"FileRecv\\UnpackedZipFile\\";
					MoveFile(FilePath,DirNew);
					CString ProjectPath=DirUnpack;
					ProjectPath+=L"\\";
					ProjectPath+=pIOContext->FileName.Left(pIOContext->FileName.Find(L"."));
					//添加新接收的项目到列表
					CAnalasisDlg* pCAnalasisDlg = (CAnalasisDlg*)g_pCAnalasisDlg;
					if(pCAnalasisDlg->AddRecvPro(ProjectPath)!=0)
					{
						//添加列表失败
						ZeroMemory(pIOContext->Buffer,DATA_BUFSIZE);
						memcpy(pIOContext->Buffer,"Failed",sizeof("Failed"));
						pIOContext->nBytesInBuf = DATA_BUFSIZE;//sizeof("Failed");
						pIOContext->wsaBuffer.buf = pIOContext->Buffer;
						pIOContext->wsaBuffer.len = DATA_BUFSIZE;//sizeof("Failed");
						ZeroMemory(pIOContext,sizeof(WSAOVERLAPPED));
						pIOContext->op = IOStatus;//要求接收文件名
						dwFlags = 0;
						nRet = WSASend(pIOContext->hSocket,&(pIOContext->wsaBuffer),1,NULL,dwFlags,pIOContext,NULL);
						int lastErr = WSAGetLastError();
						if(nRet == SOCKET_ERROR&&WSA_IO_PENDING != lastErr)
						{
							if(pIOContext->oFile.is_open())
							{
								pIOContext->oFile.close();
								pIOContext->oFile.clear();
							}
							CloseIO(pIOContext);
						}
					}
					else
					{
						//添加列表成功
						ZeroMemory(pIOContext->Buffer,DATA_BUFSIZE);
						memcpy(pIOContext->Buffer,"success",sizeof("success"));
						pIOContext->nBytesInBuf = DATA_BUFSIZE;//sizeof("success");
						pIOContext->wsaBuffer.buf = pIOContext->Buffer;
						pIOContext->wsaBuffer.len = DATA_BUFSIZE;//sizeof("success");

						ZeroMemory(pIOContext,sizeof(WSAOVERLAPPED));
						pIOContext->op = IOStatus;
						dwFlags = 0;
						nRet = WSASend(pIOContext->hSocket,&(pIOContext->wsaBuffer),1,NULL,dwFlags,pIOContext,NULL);
						int lastErr = WSAGetLastError();
						if(nRet == SOCKET_ERROR&&WSA_IO_PENDING != lastErr)
						{
							if(pIOContext->oFile.is_open())
							{
								pIOContext->oFile.close();
								pIOContext->oFile.clear();
							}
							CloseIO(pIOContext);
						}
					}
				}
			}
		}
		else if(pIOContext->op == IOStatus)
		{
			//重新初始化
			//关闭连接
			if(pIOContext->oFile.is_open())
			{
				pIOContext->oFile.close();
				pIOContext->oFile.clear();
			}
			CloseIO(pIOContext);
		}
	}
	return 0;
}


bool CAnalasisDlg::AddRecvPro(CString proPath)
{
	CString ProjectPath=proPath;
	if (PathFileExists(ProjectPath))
	{
		//存在
		int nPos=0, nLastPos=-1;
		nPos = ProjectPath.Find(L"\\",nLastPos+1);
		while(nPos>=0)
		{
			nLastPos = nPos;
			nPos = ProjectPath.Find(L"\\",nLastPos+1);
			// 处理	
		}
		CString strTitle=ProjectPath.Right(ProjectPath.GetLength()-nLastPos-1);
		CString strPath=ProjectPath;

		//检测项目信息是否完整
		CFileFind finderIsFinished;

		CString strdir2=strPath;
		strdir2+=L"\\项目信息.xlsx";
		ProFinInfo proFinInfo;

		bool success2=finderIsFinished.FindFile(strdir2);
		if(success2)
		{
			//CArray<ProFinInfo,ProFinInfo> m_ary_FinishedPro;
			

			proFinInfo.m_FinishedProjectName = strTitle;//.Add(strTitle);
			proFinInfo.m_FinishedProjectPath = strPath;//Add(strPath);
			proFinInfo.m_FinishedProjectInfoPath = strdir2;//Add(strdir2);

			CExcelOperator 	ExcelOperator;
			ExcelOperator.LoadExcel(strdir2);

			//CString ProjectName;
			CString SenderName;
			CString SendDate;

			ExcelOperator.GetItem(10,1,L"Date",SendDate);
			ExcelOperator.GetItem(8,1,L"String",SenderName);

			SYSTEMTIME sys; 
			GetLocalTime(&sys); 
			CString sTime;
			sTime.Format(L"%d-%d-%d",sys.wYear,sys.wMonth,sys.wDay);

			//导入接收日期信息
			ExcelOperator.SetItem(12,1,sTime);
			proFinInfo.m_FinishedProjectSendDate=SendDate;//Add(SendDate);
			proFinInfo.m_FinishedProjectRecvDate=sTime;//Add(sTime);
			proFinInfo.m_FinishedProjectSenderName=SenderName;//Add(SenderName);

			//AllowChangelist=true;
			ExcelOperator.Save();
			ExcelOperator.ReleaseExcel();
		}			
		finderIsFinished.Close();

		strdir2=strPath;
		strdir2+=L"\\鉴定结果.xlsx";

		CExcelOperator 	ExcelOperator;
		ExcelOperator.LoadExcel(strdir2);
		CString PreResult;
		int i=1;
		while(1)
		{
			if(PreResult==L"")
			{
				break;
			}
			ExcelOperator.GetItem(i,1,L"String",PreResult);
		}
		ExcelOperator.GetItem(i+1,2,L"String",PreResult);
		ExcelOperator.ReleaseExcel();
		proFinInfo.m_FinishedProjectPreResult = PreResult;//Add(PreResult);
		
		EnterCriticalSection(&g_csFinishedProInfo);
		m_ary_FinishedPro.InsertAt(0,proFinInfo);
		//完成项目列表
		m_ListFinishedProject.InsertItem(0,m_ary_FinishedPro[0].m_FinishedProjectName);
		m_ListFinishedProject.SetItemText(0,1,m_ary_FinishedPro[0].m_FinishedProjectSendDate);
		m_ListFinishedProject.SetItemText(0,2,m_ary_FinishedPro[0].m_FinishedProjectSenderName);
		m_ListFinishedProject.SetItemText(0,3,m_ary_FinishedPro[0].m_FinishedProjectRecvDate);
		m_ListFinishedProject.SetItemText(0,4,m_ary_FinishedPro[0].m_FinishedProjectPreResult);
		LeaveCriticalSection(&g_csFinishedProInfo);
		return 0;
	}
	return 0;
}
//添加已接收项目到列表中
int CAnalasisDlg::AddFinishedPro(void)
{
	CFileFind finder; 
	//得到当前路径的绝对路径
	CString _currentDir;
	GetCurrentDirectory(MAX_PATH,_currentDir.GetBuffer(MAX_PATH));
	_currentDir.ReleaseBuffer(); 
	CString strDirFinishedAna =_currentDir;
	strDirFinishedAna +=L"\\FinishedPro\\*.*";
	BOOL bWorking = finder.FindFile(strDirFinishedAna);
	//添加待分析项目
	while (bWorking)  
	{
		bWorking = finder.FindNextFile();
		if(finder.IsDirectory() && "." != finder.GetFileName() && ".." != finder.GetFileName())//注意该句需要排除“.”“..”
		{
			CString strTitle=finder.GetFileName();
			CString strPath=finder.GetFilePath();

			CFileFind finderIsProject;

			CString strdir2=strPath;
			strdir2+="\\项目信息.xlsx";

			bool success2=finderIsProject.FindFile(strdir2);
			if(success2)
			{	
				ProFinInfo proFinInfo;
				proFinInfo.m_FinishedProjectName = strTitle;
				proFinInfo.m_FinishedProjectPath = strPath;
				proFinInfo.m_FinishedProjectInfoPath = strdir2;

				CExcelOperator 	ExcelOperator;
				ExcelOperator.LoadExcel(strdir2);

				//CString ProjectName;
				CString SenderName;
				CString SendDate;
				CString RecvDate;

				ExcelOperator.GetItem(10,1,L"Date",SendDate);
				ExcelOperator.GetItem(8,1,L"String",SenderName);
				ExcelOperator.GetItem(12,1,L"Date",RecvDate);

				proFinInfo.m_FinishedProjectSendDate=SendDate;
				proFinInfo.m_FinishedProjectRecvDate=RecvDate;
				proFinInfo.m_FinishedProjectSenderName=SenderName; 

				ExcelOperator.ReleaseExcel();
				CString strdir3=strPath;
				strdir3+=L"\\鉴定结果.xlsx";

				ExcelOperator.LoadExcel(strdir3);
				CString PreResult;
				int i=1;

				while(1)
				{
					ExcelOperator.GetItem(i,1,L"String",PreResult);
					if(PreResult==L"")
					{
						break;
					}
					i++;
				}
				ExcelOperator.GetItem(i+1,2,L"String",PreResult);
				ExcelOperator.ReleaseExcel();
				proFinInfo.m_FinishedProjectPreResult = PreResult;

				m_ary_FinishedPro.Add(proFinInfo);
			}	
			finderIsProject.Close();
		}
	}
	//在列表中显示
	for(int i=0;i<m_ary_FinishedPro.GetCount();i++)
	{
		m_ListFinishedProject.InsertItem(i,m_ary_FinishedPro[i].m_FinishedProjectName);
		m_ListFinishedProject.SetItemText(i,1,m_ary_FinishedPro[i].m_FinishedProjectSendDate);
		m_ListFinishedProject.SetItemText(i,2,m_ary_FinishedPro[i].m_FinishedProjectSenderName);
		m_ListFinishedProject.SetItemText(i,3,m_ary_FinishedPro[i].m_FinishedProjectRecvDate);
		m_ListFinishedProject.SetItemText(i,4,m_ary_FinishedPro[i].m_FinishedProjectPreResult);
	}
	return 0;
}
int CAnalasisDlg::UpdateFinishedProList(void)
{
	return 0;
}
void CAnalasisDlg::OnNMClickListFinishedproject(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	m_ListDetailResult.DeleteAllItems();
	//若没有选择
	if(!m_ListFinishedProject.GetSelectedCount())return;
	//若选中
	m_ListDetailResult.SetRedraw(false);

	int nItem=m_ListFinishedProject.GetSelectionMark();
	//获取选择的项目鉴定结果路径

	EnterCriticalSection(&g_csFinishedProInfo);
	CString nResultPath=m_ary_FinishedPro[nItem].m_FinishedProjectPath;
	CString nProjectName = m_ary_FinishedPro[nItem].m_FinishedProjectName;
	LeaveCriticalSection(&g_csFinishedProInfo);

	nResultPath+=L"\\鉴定结果.xlsx";
	CExcelOperator ExcelOperator;
	ExcelOperator.LoadExcel(nResultPath);

	CString specFileName;
	CString PredictResult;

	int j=0;
	ExcelOperator.GetItem(j+1,1,L"String",specFileName);
	ExcelOperator.GetItem(j+1,2,L"String",PredictResult);

	while(!specFileName.IsEmpty())
	{
		CString specFileTitle;
		specFileTitle=specFileName.Left(specFileName.Find('.'));
		m_ListDetailResult.InsertItem(j,nProjectName);
		m_ListDetailResult.SetItemText(j,1,specFileTitle);
		m_ListDetailResult.SetItemText(j,2,PredictResult);
		j++;
		ExcelOperator.GetItem(j+1,1,L"String",specFileName);
		ExcelOperator.GetItem(j+1,2,L"String",PredictResult);
	}

	ExcelOperator.ReleaseExcel();
	m_ListDetailResult.SetRedraw(true);
	return;
}
void CAnalasisDlg::OnBnClickedButtonImportfinishedproject()
{
	// TODO: 在此添加控件通知处理程序代码


}
void CAnalasisDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	// TODO: 在此处添加消息处理程序代码
	ReleaseResource();
}

void CAnalasisDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnClose();
}
void CAnalasisDlg::ReleaseResource(void)
{
	ExitAllThread=1;
	closesocket(g_hListenSocket);
	while(!(ExitThreadCheckConnect&ExitThreadReceiveResult&ExitThreadPackAndSendProject&(!g_ThreadRecvingCount)))
	{
		Sleep(100);
	}
	if(rsa_publickey)
	{
		RSA_free(rsa_publickey);
		rsa_publickey = NULL;
	}
	if(m_sock!=INVALID_SOCKET)
	{
		closesocket(m_sock);
		m_sock = INVALID_SOCKET;
	}
	DeleteCriticalSection(&g_csFinishedProInfo);
	DeleteCriticalSection(&g_csConns);
}
//打印页头
void CAnalasisDlg::PrintPageHeader(CDC* pDC, UINT nPageNumber, CPrintInfo* pInfo)
{
	CFont *pOldFont;
	CFont fontHeader;
	fontHeader.CreatePointFont(100,L"FixedSys",pDC);
	pOldFont=(CFont *)(pDC->SelectObject(&fontHeader));
	int nPosY,nPosX;
	nPosY=3*m_cyPrinter;
	nPosX=m_cxOffset;
	pDC->TextOut(nPosX,nPosY,L"白酒鉴定与溯源");
	CString str;
	str.Format(L"第 %d 页 / 共 %d 页",nPageNumber,pInfo->GetMaxPage());
	CSize size=pDC->GetTextExtent(str);
	nPosX=m_cxOffset+m_cxWidth-size.cx;
	pDC->TextOut(nPosX,nPosY,str);
	pDC->MoveTo(m_cxOffset,4*m_cyPrinter);
	pDC->LineTo(m_cxOffset+m_cxWidth,4*m_cyPrinter);
	pDC->SelectObject(pOldFont);
	fontHeader.DeleteObject();

}
void CAnalasisDlg::OnBnClickedButtonPrintresult()
{
	// TODO: 在此添加控件通知处理程序代码
	CClientView * p_CClientView = (CClientView *)GetParent();
	p_CClientView->m_pCallPrintDlg = this;
	p_CClientView->CallPrint();
}
void CAnalasisDlg::OnBnClickedButtonPrintpreview()
{
	// TODO: 在此添加控件通知处理程序代码
	CClientView * p_CClientView = (CClientView *)GetParent();
	p_CClientView->m_pCallPrintDlg = this;
	p_CClientView->CallPrintPreview();
}

afx_msg LRESULT CAnalasisDlg::OnBeginPrinting(WPARAM wParam, LPARAM lParam)
{
	//详细结果列表
	CDC* pDC			= (CDC*)wParam;
	CPrintInfo* pInfo	= (CPrintInfo *)lParam;
	if(m_fontPrinter.m_hObject==NULL)
		m_fontPrinter.CreatePointFont(120,L"FixedSys",pDC);
    TEXTMETRIC tm;
    CFont* pOldFont = pDC->SelectObject (&m_fontPrinter);
    pDC->GetTextMetrics (&tm);
    m_cyPrinter = tm.tmHeight + tm.tmExternalLeading;
    CSize size = pDC->GetTextExtent (_T ("-----------------------------------------------------------------------------------"), 81);
    pDC->SelectObject (pOldFont);
    m_cxWidth = size.cx;

    m_nLinesPerPage = (pDC->GetDeviceCaps (VERTRES) -
        (m_cyPrinter * (3 + (2 * PRINTMARGIN)))) / m_cyPrinter;

	m_MaxPage =  m_ListDetailResult.GetItemCount()/(m_nLinesPerPage-1);
	++m_MaxPage;

	pInfo->SetMaxPage (m_MaxPage); //设置最大打印张数

    m_cxOffset = (pDC->GetDeviceCaps (HORZRES) - size.cx) / 2;

	return 0;
}
afx_msg LRESULT CAnalasisDlg::OnEndPrinting(WPARAM wParam, LPARAM lParam)
{

	if(m_fontPrinter.m_hObject!=NULL)
		m_fontPrinter.DeleteObject ();
	return 0;
}
afx_msg LRESULT CAnalasisDlg::OnMyPrint(WPARAM wParam, LPARAM lParam)
{
	UINT gl_uNumOfPoints = 52;
	CDC* pDC			= (CDC*)wParam;
	CPrintInfo* pInfo	= (CPrintInfo *)lParam;
	int nPageNumber = pInfo->m_nCurPage;
	int i=0,j=0;
	CFont *pOldFont;
	CFont DataFont;
	DataFont.CreatePointFont(120,L"宋体",pDC);
	PrintPageHeader(pDC,pInfo->m_nCurPage,  pInfo);
	TCHAR *pszTitle[3]={L"项目名称",L"光谱文件名",L"鉴定结果"};
	pOldFont=pDC->SelectObject(&DataFont);
	
	int nMaxPageNumber = pInfo->GetMaxPage();

	int TotalPrintItemNumber = m_ListDetailResult.GetItemCount(); //总打印数目
	int RestPrintItemNumber = TotalPrintItemNumber - (nPageNumber-1)*(m_nLinesPerPage-1);//剩余打印数目
	int PrintItemNumber = 0; //当前页要打印的数目

	if(RestPrintItemNumber > m_nLinesPerPage - 1)
	{
		PrintItemNumber = m_nLinesPerPage - 1;
		pDC->Rectangle(m_cxOffset,(int)((2.5 + PRINTMARGIN)*m_cyPrinter),m_cxOffset+m_cxWidth,(PrintItemNumber+1+3 + PRINTMARGIN)*m_cyPrinter+20);		
	}
	else
	{
		PrintItemNumber =  RestPrintItemNumber;
		pDC->Rectangle(m_cxOffset,(int)((2.5 + PRINTMARGIN)*m_cyPrinter),m_cxOffset+m_cxWidth,(PrintItemNumber+1+3 + PRINTMARGIN)*m_cyPrinter+20);
	}
	pDC->MoveTo(m_cxOffset,(3.75 + PRINTMARGIN)*m_cyPrinter);
	pDC->LineTo(m_cxOffset+m_cxWidth,(3.75 + PRINTMARGIN)*m_cyPrinter);

	for(i=1;i<3;i++)
	{
		pDC->MoveTo(m_cxOffset+i*m_cxWidth/3,(int)((2.5 + PRINTMARGIN)*m_cyPrinter));
		pDC->LineTo(m_cxOffset+i*m_cxWidth/3,(3 +PrintItemNumber+1+PRINTMARGIN)*m_cyPrinter+20);
	}
	for(i=0;i<3;i++)
	{
		pDC->TextOut(m_cxOffset+50+i*m_cxWidth/3,(int)((2.8 + PRINTMARGIN)*m_cyPrinter),CString(pszTitle[i]));
	}

	for(i=0;i<PrintItemNumber;i++)
	{
		CString str[3];
		CString projectName = m_ListDetailResult.GetItemText((nPageNumber-1)*(m_nLinesPerPage-1)+i,0);
		CString SpecFileName = m_ListDetailResult.GetItemText((nPageNumber-1)*(m_nLinesPerPage-1)+i,1);
		CString PredictResult = m_ListDetailResult.GetItemText((nPageNumber-1)*(m_nLinesPerPage-1)+i,2);

		str[0].Format(L"%s",projectName);
		str[1].Format(L"%s",SpecFileName);
		str[2].Format(L"%s",PredictResult);
		for(j=0;j<3;j++)
		{
			pDC->TextOut(m_cxOffset+50+j*m_cxWidth/3,(4 + PRINTMARGIN+i)*m_cyPrinter+5,str[j]);		
		}
	}
	pDC->SelectObject (pOldFont);
	DataFont.DeleteObject();
	return 0;
}

