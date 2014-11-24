// AnalasisDlg.cpp : ʵ���ļ�

#include "stdafx.h"
#include "MainFrm.h"
#include "Client.h"
#include "AnalasisDlg.h"
#include "MainComponentsAnaDlg.h"
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
volatile bool ExitAllThread;

//��ֹ�߳�
volatile bool ExitThreadPackAndSendProject;
volatile bool ExitThreadCheckConnect;
//�߳���ֹ��־
volatile bool ThreadCheckConnectExited;
volatile bool ThreadReceiveResultExited;
volatile bool ThreadPackAndSendProjectExited;

//�ٽ���
CRITICAL_SECTION g_csFinishedProInfo;
CRITICAL_SECTION g_csConns;

//������������Դ�����Ŀ����������
ConnectionList g_conns;

CString g_CurrentDir;
//�����׽���
SOCKET g_hListenSocket;

CString GetLoginUserName()
{
	return UserName;
}

// CAnalasisDlg �Ի���


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
	DDX_Control(pDX, IDC_BUTTON_LASTPIC, m_btnLastPic);
	DDX_Control(pDX, IDC_BUTTON_NEXTPIC, m_btnNextPic);
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
	ON_BN_CLICKED(IDC_BUTTON_DELETEFINISHED, &CAnalasisDlg::OnBnClickedButtonDeletefinished)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_FINISHEDPROJECT, &CAnalasisDlg::OnNMDblclkListFinishedproject)
	ON_BN_CLICKED(IDC_BUTTON_COMPONENTSANALASIS, &CAnalasisDlg::OnBnClickedButtonComponentsanalasis)
END_MESSAGE_MAP()


// CAnalasisDlg ��Ϣ�������
void CAnalasisDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	if(!m_ary_PicturesPath.IsEmpty())
	{
		CRect rect;
		GetDlgItem(IDC_PICTURE)->GetWindowRect(rect);
		DisplayPic(m_ary_PicturesPath[m_PicIndex],this->GetDlgItem(IDC_PICTURE)->GetSafeHwnd(), rect.Width(), rect.Height());
	}

	GetDlgItem(IDC_STATIC_NEWPROJECT)->MoveWindow(0,0,m_rect.Width()*0.7,m_rect.Height()*0.5-5);
	GetDlgItem(IDC_LIST_TOANALASIS)->MoveWindow(10,20,m_rect.Width()*0.7-20,m_rect.Height()*0.5-5-60-10);

	GetDlgItem(IDC_BUTTON_IMPORTSPEC)->MoveWindow(10,m_rect.Height()*0.5-5-10-30,120,30);
	GetDlgItem(IDC_BUTTON_SENDFORANALASIS)->MoveWindow(140,m_rect.Height()*0.5-5-10-30,80,30);

	GetDlgItem(IDC_STATIC_DETAILRESULT)->MoveWindow(0,m_rect.Height()*0.5+5,m_rect.Width()*0.7,m_rect.Height()*0.5-5);
	GetDlgItem(IDC_LIST_ANALASISRESULT)->MoveWindow(10,m_rect.Height()*0.5+5+10+10,m_rect.Width()*0.7-20,m_rect.Height()*0.5-5-60-10);

	GetDlgItem(IDC_BUTTON_PRINTRESULT)->MoveWindow(10,m_rect.Height()-40,80,30);
	GetDlgItem(IDC_BUTTON_PRINTPREVIEW)->MoveWindow(100,m_rect.Height()-40,80,30);
	GetDlgItem(IDC_BUTTON_SPECCOMPARE)->MoveWindow(190,m_rect.Height()-40,100,30);
	GetDlgItem(IDC_BUTTON_COMPONENTSANALASIS)->MoveWindow(300,m_rect.Height()-40,100,30);

	GetDlgItem(IDC_STATIC_WINEPICTURE)->MoveWindow(m_rect.Width()*0.7+10,0,m_rect.Width()*0.3-20,m_rect.Height()*0.5-5);
	GetDlgItem(IDC_PICTURE)->MoveWindow(m_rect.Width()*0.7+10+10,20,m_rect.Width()*0.3-20-20,m_rect.Height()*0.5-5-60-10);
	GetDlgItem(IDC_BUTTON_LASTPIC)->MoveWindow(m_rect.Width()*0.7+20,m_rect.Height()*0.5-5-10-30,80,30);
	GetDlgItem(IDC_BUTTON_NEXTPIC)->MoveWindow(m_rect.Width()-10-80-10,m_rect.Height()*0.5-5-10-30,80,30);

	GetDlgItem(IDC_STATIC_FINISHEDPROJECT)->MoveWindow(m_rect.Width()*0.7+10,m_rect.Height()*0.5+5,m_rect.Width()*0.3-20,m_rect.Height()*0.5-5);
	GetDlgItem(IDC_LIST_FINISHEDPROJECT)->MoveWindow(m_rect.Width()*0.7+10+10,m_rect.Height()*0.5+5+10+10,m_rect.Width()*0.3-20-20,m_rect.Height()*0.5-5-60-10);

	GetDlgItem(IDC_BUTTON_DELETEFINISHED)->MoveWindow(m_rect.Width()*0.7+10+10,m_rect.Height()-40,80,30);
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
}

void CAnalasisDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	GetClientRect(&m_rect);
}


BOOL CAnalasisDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	ExitAllThread=0;
	ThreadCheckConnectExited=1;
	ThreadReceiveResultExited=1;
	ThreadPackAndSendProjectExited=1;

	g_hListenSocket = INVALID_SOCKET;
	g_pCAnalasisDlg = this;

	InitializeCriticalSection(&g_csFinishedProInfo);
	InitializeCriticalSection(&g_csConns);
	

	m_ListInput.InsertColumn(0,L"�׾ƹ�����",LVCFMT_LEFT,200);
	m_ListInput.InsertColumn(1,L"�׾���",LVCFMT_LEFT,200);
	m_ListInput.InsertColumn(2,L"�׾���Ϣ",LVCFMT_LEFT,200);

	DWORD dwStyle = m_ListInput.GetExtendedStyle(); //��ȡ��ǰ��չ��ʽ
	dwStyle |= LVS_EX_FULLROWSELECT;                //ѡ��ĳ��ʹ���и�����report���ʱ��
	dwStyle |= LVS_EX_GRIDLINES;                    //�����ߣ�report���ʱ��
	m_ListInput.SetExtendedStyle(dwStyle);          //������չ���

	//�����Ŀ�б�
	m_ListFinishedProject.InsertColumn(0,L"��Ŀ����",LVCFMT_LEFT,100);
	m_ListFinishedProject.InsertColumn(1,L"��Ŀ��������",LVCFMT_LEFT,100);
	m_ListFinishedProject.InsertColumn(2,L"������",LVCFMT_LEFT,100);
	m_ListFinishedProject.InsertColumn(3,L"�����������",LVCFMT_LEFT,100);
	m_ListFinishedProject.InsertColumn(4,L"��Դ���",LVCFMT_LEFT,100);


	dwStyle = m_ListFinishedProject.GetExtendedStyle(); //��ȡ��ǰ��չ��ʽ
	dwStyle |= LVS_EX_FULLROWSELECT; //ѡ��ĳ��ʹ���и�����report���ʱ��
	dwStyle |= LVS_EX_GRIDLINES; //�����ߣ�report���ʱ��
	m_ListFinishedProject.SetExtendedStyle(dwStyle); //������չ���

	//��ϸ����б�
	m_ListDetailResult.InsertColumn(0,L"��Ŀ����",LVCFMT_LEFT,200);
	m_ListDetailResult.InsertColumn(1,L"�����ļ���",LVCFMT_LEFT,200);
	m_ListDetailResult.InsertColumn(2,L"��Դ���",LVCFMT_LEFT,200);
	dwStyle = m_ListDetailResult.GetExtendedStyle(); //��ȡ��ǰ��չ��ʽ
	dwStyle |= LVS_EX_FULLROWSELECT; //ѡ��ĳ��ʹ���и�����report���ʱ��
	dwStyle |= LVS_EX_GRIDLINES; //�����ߣ�report���ʱ��
	m_ListDetailResult.SetExtendedStyle(dwStyle); //������չ���


	m_ListInput.SetHeaderBackColor(168,193,221,2);           //������չ���
	m_ListInput.SetRowHeigt(28);              //������չ���
	m_ListInput.SetHeaderHeight(1);           //������չ���

	m_ListFinishedProject.SetHeaderBackColor(168,193,221,2);           //������չ���
	m_ListFinishedProject.SetRowHeigt(28);              //������չ���
	m_ListFinishedProject.SetHeaderHeight(1);           //������չ���

	m_ListDetailResult.SetHeaderBackColor(168,193,221,2);           //������չ���
	m_ListDetailResult.SetRowHeigt(28);              //������չ���
	m_ListDetailResult.SetHeaderHeight(1);           //������չ���

	AddFinishedPro();
	//�������ܽ���߳�
	pThreadReceiveResult=AfxBeginThread(ThreadReceiveResult,this);
	//��ȡCClientViewָ��
	CClientView *pCClientView=(CClientView*) this->GetParent();
	m_sock=pCClientView->m_sClient;
	UserName = m_UserName;
	::SendMessage(::AfxGetApp()->GetMainWnd()->m_hWnd,WM_SHOWUSERNAME,NULL,NULL);
	OnBegincheckconnect(NULL,NULL);
	::SendMessage(AfxGetApp()->GetMainWnd()->m_hWnd,WM_SHOWUSERNAME,0,0);

	m_btnLastPic.SetIcon(IDI_ICON_PREV); 
	m_btnLastPic.DrawTransparent(TRUE);

	m_btnNextPic.SetIcon(IDI_ICON_NEXT); 
	m_btnNextPic.DrawTransparent(TRUE);
	m_btnNextPic.SetAlign(CButtonST::ST_ALIGN_HORIZ_RIGHT);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

int CAnalasisDlg::InitWind(void)
{
	//��ȡCClientViewָ��
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
	ExitThreadCheckConnect=1;
	while(!ThreadCheckConnectExited)
	{
		Sleep(10);
	}
	pCThreadWnd = NULL;
	return 0;
}

UINT ThreadCheckConnect(LPVOID lparam)
{
	CClientView *pCClientView=(CClientView*) lparam;
	char Buf[DATA_BUFSIZE] = {0};
	int count = 0;	
	ThreadCheckConnectExited = 0;
	ExitThreadCheckConnect = 0;
	while(!ExitThreadCheckConnect)
	{
		if(ExitAllThread)
		{	
			ThreadCheckConnectExited=1;
			return -2;
		}
		while(!ExitThreadCheckConnect)
		{
			if(ExitAllThread)
			{	
				ThreadCheckConnectExited=1;
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
					//���µ�¼
					MessageBox(pCClientView->m_hWnd,L"����������ӳ�ʱ�������µ�¼��",L"����ʧ��",MB_ICONERROR);
					pCClientView->isDlgLogin=true;
					pCClientView->isDlgAnalasis=false;
					pCClientView->isSpecCompareDlg=false;
					::PostMessage(pCClientView->m_hWnd,WM_UISELECT,NULL,NULL);
					ThreadCheckConnectExited=1;
					return -1;
				}
			}
			break;
		}
		Sleep(1000);
	}
	ThreadCheckConnectExited=1;
	return 0;
}


void CAnalasisDlg::OnBnClickedButtonSpeccompare()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_ListFinishedProject.GetSelectedCount()==0)
	{
		MessageBox(L"��ѡ��һ����Ŀ��",L"�˹���������",MB_ICONWARNING);
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
	// TODO: �ڴ���������������
	OnBnClickedButtonSpeccompare();
}


void CAnalasisDlg::OnBnClickedButtonImportspec()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	BROWSEINFO bi;
	char Buffer[MAX_PATH];
	bi.hwndOwner = this->m_hWnd;
	bi.pidlRoot =NULL;
	bi.pszDisplayName =(LPWSTR)Buffer;
	bi.lpszTitle = L"ѡ��������ݼ��ļ���·��";
	bi.ulFlags =BIF_USENEWUI ; 
	bi.lpfn = NULL;
	bi.lParam = 0;
	bi.iImage=IDR_MAINFRAME;
	//��ʼ����ڲ���bi����
	LPITEMIDLIST pIDList = SHBrowseForFolder(&bi);

	if(pIDList==NULL)
	{	
		return;
	}
	SHGetPathFromIDList(pIDList,(LPWSTR)Buffer);
	//ȡ���ļ���·����Buffer��
	m_SpecsToAnaFolderPath.Format(L"%s",Buffer);
	// free memory used      
	IMalloc * imalloc = 0;
	if (SUCCEEDED(SHGetMalloc(&imalloc)))
	{
		imalloc->Free(pIDList);
		imalloc->Release();
	}
	//���ļ����е���������ݼ������Ϣ
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
		// �����õ��������ļ�����
		CFileFind finder; 
		BOOL bWorking = finder.FindFile(strDir);

		m_ary_SpecFilePath.RemoveAll();
		m_ary_SpecFileName.RemoveAll();
		m_ary_SpecFileTitle.RemoveAll();
		m_ary_PicturesPath.RemoveAll();
		m_ary_SpecFileTitle.RemoveAll();
		while (bWorking)  
		{
			bWorking = finder.FindNextFile();
			CString str=finder.GetFileName();
			if(str.Right(4)==L".CSV"||str.Right(4)==L".csv" &&L"." !=str &&L".." != str)//ע��þ���Ҫ�ų���.����..��
			{
				CString filename;
				CString fileTitle;

				m_ary_SpecFilePath.Add(finder.GetFilePath());
				m_ary_SpecFileName.Add(finder.GetFileName());
				//��ȡ�ļ���(��������׺)  
				//����CString��Left(int count)��ȡCString�д�����������count���ַ�  
				//fileName.GetLength()-4�е�4��ʾ".csv"�ĸ��ַ�  
				m_ary_SpecFileTitle.Add(finder.GetFileTitle());//���ļ���(��������׺)��ӵ�������  
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
			MessageBox(L"���ļ�����û�й涨��ʽ�Ĺ������ݣ�������ѡ��",L"����",MB_ICONWARNING);
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
			MessageBox(L"��������̫��",L"����",MB_ICONWARNING);
			return;
		}
		m_PicIndex=0;
		SortCArray(m_ary_SpecFileTitle);

		for(int i=0;i<m_ary_SpecFileTitle.GetCount();i++)
		{
			CString Wineinfo=L"";
			Wineinfo.Format(L"%s��,%s,%s",m_AlcoholContent,m_Flavour,m_Brand);
			m_ListInput.InsertItem(i,m_ary_SpecFileTitle[i]);
			m_ListInput.SetItemText(i,1,m_WineName);
			m_ListInput.SetItemText(i,2,Wineinfo);
		}

	}
}


void CAnalasisDlg::OnBnClickedButtonLastpic()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_ary_SpecFilePath.GetCount() == 0)
	{
		MessageBox(L"���ȵ�����ף�",L"������Ŀ",MB_ICONWARNING);
		return;
	}

	CProjectInfoWriteDlg ProjectInfoWriteDlg;
	ProjectInfoWriteDlg.m_WineName=m_WineName;
	ProjectInfoWriteDlg.m_AlcoholContent=m_AlcoholContent;
	ProjectInfoWriteDlg.m_Flavour=m_Flavour;
	ProjectInfoWriteDlg.m_Brand=m_Brand;

	if(IDOK==ProjectInfoWriteDlg.DoModal())
	{
		//�����Ŀ
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
			//�����������Ŀ
			AfxBeginThread(ThreadPackAndSendProject,this);
		}
	}
}
//����������Ŀ���ȴ���
afx_msg LRESULT CAnalasisDlg::OnBeginshowsendprogress(WPARAM wParam, LPARAM lParam)
{
	pCProjectSendProgressDlg=new CProjectSendProgressDlg;

	pCProjectSendProgressDlg->m_CurrentProgress=1;
	pCProjectSendProgressDlg->m_MaxRange=0;
	pCProjectSendProgressDlg->m_SendState=L"����׼����Ŀ";
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

//���������Ŀ
UINT ThreadPackAndSendProject(LPVOID lparam)
{
	AfxOleInit();
	bool bSendSuccess = false;
	ThreadPackAndSendProjectExited=0;

	//��ȡCAnalasisDlgָ��
	CAnalasisDlg *pCAnalasisDlg=(CAnalasisDlg*) lparam;

	//��ȡCClientViewָ��
	CClientView *pCClientView=(CClientView*) pCAnalasisDlg->GetParent();

	::SendMessage(pCAnalasisDlg->m_hWnd,WM_BEGINSHOWSENDPROGRESS,0,0);

	//����ѹ����
	CString strFolder;
	strFolder.Format(L"tempFile\\%s",pCAnalasisDlg->m_ProjectName);
	CreateDirectory(strFolder,NULL);

	CString strDir=strFolder;
	strDir+=L"\\";

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
	projectInfoPath+=L"��Ŀ��Ϣ.xlsx";
	
	//��Ŀ��Ϣ��񱸷ݾ���·��
	CString strprojectInfoBackupFullPath=g_CurrentDir;
	strprojectInfoBackupFullPath+="\\tempFile\\��Ŀ��Ϣ.xlsx";

	//��ȡ��Ŀ�ļ��о���·��
	CString strFolderFullPath=g_CurrentDir;		
	strFolderFullPath+="\\tempFile\\";
	strFolderFullPath+=pCAnalasisDlg->m_ProjectName;

	//�����ɵ���Ŀ��Ϣ������·��
	CString strprojectInfoFullPath=strFolderFullPath;
	strprojectInfoFullPath+="\\��Ŀ��Ϣ.xlsx";

	CExcelOperator ExcelOperator;
	ExcelOperator.LoadExcel(strprojectInfoBackupFullPath);
	ExcelOperator.SetItem(1,1,pCAnalasisDlg->m_ProjectName);

	//�û���
	ExcelOperator.SetItem(2,1,pCAnalasisDlg->m_UserName);
	ExcelOperator.SetItem(3,1,pCAnalasisDlg->m_WineName);
	ExcelOperator.SetItem(4,1,pCAnalasisDlg->m_AlcoholContent);
	ExcelOperator.SetItem(5,1,pCAnalasisDlg->m_Flavour);
	ExcelOperator.SetItem(6,1,pCAnalasisDlg->m_Brand);
	ExcelOperator.SetItem(7,1,pCAnalasisDlg->m_MailAddr);
	ExcelOperator.SetItem(8,1,pCAnalasisDlg->m_SenderName);
	ExcelOperator.SetItem(9,1,pCAnalasisDlg->m_AnaType);

	//����
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
	pCAnalasisDlg->pCProjectSendProgressDlg->m_SendState=L"�����";
	pCAnalasisDlg->pCProjectSendProgressDlg->m_SendPercent=L"0%";
	pCAnalasisDlg->pCProjectSendProgressDlg->m_ProjectSize=L"0 kb";
	pCAnalasisDlg->pCProjectSendProgressDlg->m_SendedSize=L"0 kb";

	::SendMessageA(pCAnalasisDlg->pCProjectSendProgressDlg->m_hWnd,WM_SHOWSENDPROGRESS,NULL,NULL);

	//ѹ��
	int retn=zipObj.Zip_PackFiles(strFolderFullPath,zipFilePath);

	bool fla=DeleteFolder(strFolderFullPath);

	//��Ŀ����
	CString projectName=pCAnalasisDlg->m_ProjectName;
	//�Ӻ�׺
	projectName+=".zip";
	int count = 0;//������

	//���棨ÿ�η��͵Ĵ�С��
	char buf[DATA_BUFSIZE];
	long filelen;
	int len;

	SOCKET m_sock=pCAnalasisDlg->m_sock;

	ifstream infile(zipFilePath,ios::in|ios::binary);
	infile.seekg(0,ios::end);
	filelen=infile.tellg();
	infile.seekg(0,ios::beg);
	//��ʼ������Ŀ
	//�ر����Ӽ���߳�
	pCAnalasisDlg->EndCheckConnect();
	
	memset(buf,0,DATA_BUFSIZE);
	memcpy(buf,"TransmitProjet",strlen("TransmitProjet"));

	while(true)
	{
		if(ExitAllThread|ExitThreadPackAndSendProject)
		{
			infile.close();
			closesocket(m_sock);
			goto Release;
		}
		int retVal=send(m_sock,buf,DATA_BUFSIZE,0);
		if(SOCKET_ERROR==retVal)
		{
			int err=WSAGetLastError();
			if(err==WSAEWOULDBLOCK&&count<=50)
			{
				Sleep(100);
				++count;
				continue;
			}
			else
			{
				infile.close();
				closesocket(m_sock);				
				goto Release;
			}
		}
		break;
	}
	Sleep(100);
	//�����ļ���
	char *ProName=CString2Char(projectName);
	memset(buf,0,DATA_BUFSIZE);
	memcpy(buf,ProName,strlen(ProName));

	count = 0;
	while(true)
	{
		if(ExitAllThread|ExitThreadPackAndSendProject)
		{
			infile.close();
			closesocket(m_sock);
			goto Release;
		}

		int len=strlen(ProName);
		int retVal=send(m_sock,buf,DATA_BUFSIZE,0);
		if(SOCKET_ERROR==retVal)
		{
			int err=WSAGetLastError();
			if(err==WSAEWOULDBLOCK&&count<=50)
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
				goto Release;
			}
		}
		break;
	}	
	Sleep(100);
	free(ProName);
	ProName = NULL;	
	//��Ҫ���͵��ļ����ȷ��͸��ͻ���
	memset(buf,0,DATA_BUFSIZE);
	sprintf(buf,"%ld",filelen);
	count = 0;
	while(true)
	{
		if(ExitAllThread|ExitThreadPackAndSendProject)
		{
			infile.close();
			DeleteFile(zipFilePath);
			closesocket(m_sock);
			goto Release;
		}
		int retVal=send(m_sock,buf,DATA_BUFSIZE,NULL);
		if(SOCKET_ERROR==retVal)
		{
			int err=WSAGetLastError();
			if(err==WSAEWOULDBLOCK&&count<=50)
			{
				count++;
				Sleep(100);
				continue;
			}
			else
			{
				//���Ӵ���
				infile.close();
				DeleteFile(zipFilePath);
				closesocket(m_sock);				
				goto Release;
			}
		}
		break;
	}
	Sleep(100);
	char sSendedSize[100];
	int nFile=1;

	pCAnalasisDlg->pCProjectSendProgressDlg->m_CurrentProgress=1;
	pCAnalasisDlg->pCProjectSendProgressDlg->m_MaxRange=filelen/DATA_BUFSIZE+1;
	pCAnalasisDlg->pCProjectSendProgressDlg->m_SendState=L"����";
	pCAnalasisDlg->pCProjectSendProgressDlg->m_SendPercent=L"0%";
	pCAnalasisDlg->pCProjectSendProgressDlg->m_ProjectSize.Format(L"�ܴ�С��%ld kb",filelen%1024==0?filelen/1024:filelen/1024+1);
	pCAnalasisDlg->pCProjectSendProgressDlg->m_SendedSize=L"�ѷ��ͣ�0 kb";

	::SendMessageA(pCAnalasisDlg->pCProjectSendProgressDlg->m_hWnd,WM_SHOWSENDPROGRESS,NULL,NULL);

	int SendedSize=0;
	while(!infile.eof())
	{
		if(ExitAllThread|ExitThreadPackAndSendProject)
		{
			infile.close();
			closesocket(m_sock);
			goto Release;
		}

		sprintf(sSendedSize,"%d",SendedSize);
		pCAnalasisDlg->pCProjectSendProgressDlg->m_CurrentProgress=nFile;
		float Percent=((float)SendedSize/filelen)*100;
		pCAnalasisDlg->pCProjectSendProgressDlg->m_SendPercent.Format(L"%d",(int)Percent);
		pCAnalasisDlg->pCProjectSendProgressDlg->m_SendPercent+=L"%";
		pCAnalasisDlg->pCProjectSendProgressDlg->m_SendedSize.Format(L"�ѷ��ͣ�%d kb",(SendedSize%1024==0?((int)SendedSize/1024):((int)(SendedSize/1024+1))));
		::SendMessageA(pCAnalasisDlg->pCProjectSendProgressDlg->m_hWnd,WM_SHOWSENDPROGRESS,NULL,NULL);
		//����
		memset(buf,0,DATA_BUFSIZE);
		infile.read(buf,DATA_BUFSIZE);
		len=static_cast<int>(infile.gcount());	
		count = 0;
		while(true)
		{
			if(ExitAllThread|ExitThreadPackAndSendProject)
			{
				infile.close();
				closesocket(m_sock);
				goto Release;
			}
			int retVal=send(m_sock,buf,len,0);
			if(SOCKET_ERROR==retVal)
			{
				int err=WSAGetLastError();
				if(err==WSAEWOULDBLOCK&&count<=50)
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
					goto Release;
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
	pCAnalasisDlg->pCProjectSendProgressDlg->m_SendedSize.Format(L"�ѷ��ͣ�%d kb",(SendedSize%1024==0?((int)SendedSize/1024):((int)(SendedSize/1024+1))));
	::SendMessageA(pCAnalasisDlg->pCProjectSendProgressDlg->m_hWnd,WM_SHOWSENDPROGRESS,NULL,NULL);
	infile.close();
	Sleep(200);
	count = 0;
	while(true)
	{
		if(ExitAllThread|ExitThreadPackAndSendProject)
		{
			closesocket(m_sock);
			goto Release;
		}
		ZeroMemory(buf,DATA_BUFSIZE);
		int retVal=recv(m_sock,buf,DATA_BUFSIZE,0);
		if(SOCKET_ERROR==retVal)
		{
			int err=WSAGetLastError();
			if(err==WSAEWOULDBLOCK&&count<=500)
			{
				count++;
				Sleep(100);
				continue;
			}
			else if(err == WSAETIMEDOUT || err == WSAENETDOWN)
			{	
				goto Release;
			}
		}
		break;
	}
	if(strcmp(buf,"Success")==0)
	{
		bSendSuccess = true;
		::SendMessage(pCAnalasisDlg->m_hWnd,WM_ENDSHOWSENDPROGRESS,0,0);
		Sleep(200);	
	}
Release:
	//ɾ���ѷ�����Ŀ
	::SendMessage(pCAnalasisDlg->m_hWnd,WM_ENDSHOWSENDPROGRESS,0,0);
	if(bSendSuccess)
	{
		MessageBox(pCAnalasisDlg->m_hWnd,L"��Ŀ���ͳɹ��������ĵȴ���Դ���",L"���ͳɹ�",MB_ICONINFORMATION);
	}
	else
	{
		MessageBox(pCAnalasisDlg->m_hWnd,L"��Ŀ����ʧ�ܣ������·���",L"����ʧ��",MB_ICONERROR);
	}
	::SendMessage(pCAnalasisDlg->m_hWnd,WM_BEGINCHECKCONNECT,NULL,NULL);
	AfxOleTerm(FALSE);
	ThreadPackAndSendProjectExited=1;
	return 0;
}
//���÷�����ģʽ����ͻ��˵�ͨ��
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
	//ɾ���û��б�����ص���
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
	ThreadReceiveResultExited=0;
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
		//�����ļ���С
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
	}
	ThreadReceiveResultExited=1;
	return 0;
}

DWORD WINAPI ThreadRecv(LPVOID param)
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
			pIOContext->nBytesFileToRecv = atoi(pIOContext->Buffer);//Ҫ�����ļ���С
			ZeroMemory(pIOContext->Buffer,DATA_BUFSIZE);//��ջ�����

			pIOContext->nBytesInBuf = 0;
			pIOContext->wsaBuffer.buf = pIOContext->Buffer;
			pIOContext->wsaBuffer.len = DATA_BUFSIZE;

			ZeroMemory(pIOContext,sizeof(WSAOVERLAPPED));
			pIOContext->op = IORecvFileName;//�����ļ���
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
			pIOContext->FileName = Char2CString(pIOContext->Buffer);//�ļ���
			CString ProjectName=pIOContext->FileName.Left(pIOContext->FileName.Find(L"."));

			//����·���������ļ�
			strcat(pIOContext->FilePath,pIOContext->Buffer);	
			pIOContext->oFile.open(pIOContext->FilePath,ios::out|ios::binary);
			ZeroMemory(pIOContext->Buffer,DATA_BUFSIZE);//��ջ�����
			pIOContext->nBytesInBuf = 0;
			pIOContext->wsaBuffer.buf = pIOContext->Buffer;
			pIOContext->wsaBuffer.len = DATA_BUFSIZE;

			ZeroMemory(pIOContext,sizeof(WSAOVERLAPPED));
			pIOContext->op = IORecvFile;  //��ʼ�����ļ�
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
		else if(pIOContext->op == IORecvFile)//������Ŀ�ļ�
		{
			//ѭ�������ļ�
			pIOContext->oFile.write(pIOContext->Buffer,dwBytes);
			pIOContext->nBytesFileReceived+=dwBytes;
			ZeroMemory(pIOContext->Buffer,DATA_BUFSIZE);//��ջ�����

			if(pIOContext->nBytesFileReceived<pIOContext->nBytesFileToRecv)
			{
				//����δ���
				ZeroMemory(pIOContext->Buffer,DATA_BUFSIZE);//��ջ�����
				pIOContext->nBytesInBuf = 0;
				pIOContext->wsaBuffer.buf = pIOContext->Buffer;
				pIOContext->wsaBuffer.len = DATA_BUFSIZE;

				ZeroMemory(pIOContext,sizeof(WSAOVERLAPPED));
				pIOContext->op = IORecvFile;//Ҫ������ļ�
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
				//�������
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
					//��ѹ��ʧ��
					ZeroMemory(pIOContext->Buffer,DATA_BUFSIZE);//��ջ�����
					memcpy(pIOContext->Buffer,"Failed",sizeof("Failed"));
					pIOContext->nBytesInBuf = DATA_BUFSIZE;
					pIOContext->wsaBuffer.buf = pIOContext->Buffer;
					pIOContext->wsaBuffer.len = DATA_BUFSIZE;

					ZeroMemory(pIOContext,sizeof(WSAOVERLAPPED));
					pIOContext->op = IOStatus;//Ҫ������ļ���
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
					//��ѹ���ɹ�
					CString DirNew=L"FileRecv\\UnpackedZipFile\\";
					MoveFile(FilePath,DirNew);
					CString ProjectPath=DirUnpack;
					ProjectPath+=L"\\";
					ProjectPath+=pIOContext->FileName.Left(pIOContext->FileName.Find(L"."));
					//����½��յ���Ŀ���б�
					CAnalasisDlg* pCAnalasisDlg = (CAnalasisDlg*)g_pCAnalasisDlg;
					if(pCAnalasisDlg->AddRecvPro(ProjectPath)!=0)
					{
						//����б�ʧ��
						ZeroMemory(pIOContext->Buffer,DATA_BUFSIZE);
						memcpy(pIOContext->Buffer,"Failed",sizeof("Failed"));
						pIOContext->nBytesInBuf = DATA_BUFSIZE;
						pIOContext->wsaBuffer.buf = pIOContext->Buffer;
						pIOContext->wsaBuffer.len = DATA_BUFSIZE;
						ZeroMemory(pIOContext,sizeof(WSAOVERLAPPED));
						pIOContext->op = IOStatus;//Ҫ������ļ���
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
						//����б�ɹ�
						ZeroMemory(pIOContext->Buffer,DATA_BUFSIZE);
						memcpy(pIOContext->Buffer,"success",sizeof("success"));
						pIOContext->nBytesInBuf = DATA_BUFSIZE;
						pIOContext->wsaBuffer.buf = pIOContext->Buffer;
						pIOContext->wsaBuffer.len = DATA_BUFSIZE;

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
			//���³�ʼ��
			//�ر�����
			if(pIOContext->oFile.is_open())
			{
				pIOContext->oFile.close();
				pIOContext->oFile.clear();
			}
			CloseIO(pIOContext);
		}
	}
	CoUninitialize();
	return 0;
}


bool CAnalasisDlg::AddRecvPro(CString proPath)
{
	CString ProjectPath=proPath;
	if (PathFileExists(ProjectPath))
	{
		//����
		int nPos=0, nLastPos=-1;
		nPos = ProjectPath.Find(L"\\",nLastPos+1);
		while(nPos>=0)
		{
			nLastPos = nPos;
			nPos = ProjectPath.Find(L"\\",nLastPos+1);
		}
		CString strTitle=ProjectPath.Right(ProjectPath.GetLength()-nLastPos-1);
		CString strPath=ProjectPath;

		//�����Ŀ��Ϣ�Ƿ�����
		CFileFind finderIsFinished;

		CString strdir2=strPath;
		strdir2+=L"\\��Ŀ��Ϣ.xlsx";
		ProFinInfo proFinInfo;

		if(PathFileExists(strdir2))
		{	
			proFinInfo.m_FinishedProjectName = strTitle;
			proFinInfo.m_FinishedProjectPath = strPath;
			proFinInfo.m_FinishedProjectInfoPath = strdir2;

			CExcelOperator 	ExcelOperator;
			ExcelOperator.LoadExcel(strdir2);

			CString SenderName;
			CString SendDate;

			ExcelOperator.GetItem(10,1,L"Date",SendDate);
			ExcelOperator.GetItem(8,1,L"String",SenderName);

			SYSTEMTIME sys; 
			GetLocalTime(&sys); 
			CString sTime;
			sTime.Format(L"%d-%d-%d",sys.wYear,sys.wMonth,sys.wDay);

			//�������������Ϣ
			ExcelOperator.SetItem(12,1,sTime);
			proFinInfo.m_FinishedProjectSendDate=SendDate;
			proFinInfo.m_FinishedProjectRecvDate=sTime;
			proFinInfo.m_FinishedProjectSenderName=SenderName;

			ExcelOperator.Save();
			ExcelOperator.ReleaseExcel();
		}			

		strdir2=strPath;
		strdir2+=L"\\��Դ���.xlsx";

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

		proFinInfo.m_FinishedProjectPreResult = PreResult;
		
		EnterCriticalSection(&g_csFinishedProInfo);
		m_ary_FinishedPro.InsertAt(0,proFinInfo);
		//�����Ŀ�б�
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
//����ѽ�����Ŀ���б���
int CAnalasisDlg::AddFinishedPro(void)
{
	CFileFind finder; 
	//�õ���ǰ·���ľ���·��
	CString _currentDir;
	GetCurrentDirectory(MAX_PATH,_currentDir.GetBuffer(MAX_PATH));
	_currentDir.ReleaseBuffer(); 
	CString strDirFinishedAna =_currentDir;
	strDirFinishedAna +=L"\\FinishedPro\\*.*";

	sqlite3* pFinishedProjectInfodb = NULL;
	//�����ݿ⣬�������򴴽�
	if(PathFileExists(L"User\\FinishedProjectInfo.db3"))
	{
		if(sqlite3_open("User\\FinishedProjectInfo.db3", &pFinishedProjectInfodb) != 0)
		{
			return FALSE;
		}
		//�����û���Ϣ����Ѵ����򴴽�ʧ�ܣ�������Ϣ������szErrmsg��
		char sqlcmd[500] = "CREATE TABLE FinishedProjectInfo(\
						   ProjectName string PRIMARY KEY,\
						   FinishedProjectPath,\
						   FinishedProjectInfoPath,\
						   SendDate string,\
						   RecvDate string,\
						   Sender string,\
						   Result string)";

		char *pErrMsg = NULL;
		sqlite3_exec(pFinishedProjectInfodb,sqlcmd,0,0,&pErrMsg);
		//��ѯ�û�����
		char *zsql = "SELECT * FROM FinishedProjectInfo";
		char **szResult = NULL;
		int nRow = 0;
		int nColumn = 0;
		sqlite3_get_table(pFinishedProjectInfodb,zsql,&szResult,&nRow,&nColumn,&pErrMsg);
		if(nRow)
		{
			ProFinInfo proFinInfo;

			for(int i = 1;i<=nRow;++i)
			{
				proFinInfo.m_FinishedProjectName = Char2CString(szResult[i*nColumn]);
				proFinInfo.m_FinishedProjectPath = Char2CString(szResult[i*nColumn+1]);
				proFinInfo.m_FinishedProjectInfoPath = Char2CString(szResult[i*nColumn+2]);
				proFinInfo.m_FinishedProjectSendDate = Char2CString(szResult[i*nColumn+3]);
				proFinInfo.m_FinishedProjectRecvDate = Char2CString(szResult[i*nColumn+4]);
				proFinInfo.m_FinishedProjectSenderName = Char2CString(szResult[i*nColumn+5]);
				proFinInfo.m_FinishedProjectPreResult = Char2CString(szResult[i*nColumn+6]);
				m_ary_FinishedPro.Add(proFinInfo);
			}		
		}	
		sqlite3_close(pFinishedProjectInfodb);
	}
	BOOL bWorking = finder.FindFile(strDirFinishedAna);
	//��Ӵ�������Ŀ
	while (bWorking)  
	{
		bWorking = finder.FindNextFile();
		if(finder.IsDirectory() && "." != finder.GetFileName() && ".." != finder.GetFileName())//ע��þ���Ҫ�ų���.����..��
		{
			CString strTitle=finder.GetFileName();
			CString strPath=finder.GetFilePath();
			int i = 0;
			for(;i<m_ary_FinishedPro.GetCount();++i)
			{
				if(m_ary_FinishedPro[i].m_FinishedProjectPath == strPath)
				{
					break;
				}
			}
			if(i>=m_ary_FinishedPro.GetCount())
			{
				CFileFind finderIsProject;
				CString strdir2=strPath;
				strdir2+="\\��Ŀ��Ϣ.xlsx";

				CString strdir3=strPath;
				strdir3+=L"\\��Դ���.xlsx";

				bool success2 = finderIsProject.FindFile(strdir2);
				bool success3 = finderIsProject.FindFile(strdir3);
				if(success2&&success3)
				{	
					ProFinInfo proFinInfo;
					proFinInfo.m_FinishedProjectName = strTitle;
					proFinInfo.m_FinishedProjectPath = strPath;
					proFinInfo.m_FinishedProjectInfoPath = strdir2;

					CExcelOperator 	ExcelOperator;
					ExcelOperator.LoadExcel(strdir2);

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
	}

	//���б�����ʾ
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}

void CAnalasisDlg::OnNMDblclkListFinishedproject(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	m_ListDetailResult.DeleteAllItems();
	//��û��ѡ��
	if(!m_ListFinishedProject.GetSelectedCount())return;
	//��ѡ��
	m_ListDetailResult.SetRedraw(false);

	int nItem=m_ListFinishedProject.GetSelectionMark();
	//��ȡѡ�����Ŀ��Դ���·��
	EnterCriticalSection(&g_csFinishedProInfo);
	CString nResultPath=m_ary_FinishedPro[nItem].m_FinishedProjectPath;
	CString nProjectName = m_ary_FinishedPro[nItem].m_FinishedProjectName;

	nResultPath+=L"\\��Դ���.xlsx";
	if(PathFileExists(nResultPath))
	{
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
	}
	else
	{
		if(IDOK == MessageBox(L"ѡ�е���Ŀ�����ڻ��ļ�����٣��Ƿ�ɾ������Ŀ��",L"��ȡ��Ŀ��Ϣ����",MB_OKCANCEL|MB_ICONERROR))
		{
			m_ary_FinishedPro.RemoveAt(nItem);
			m_ListFinishedProject.DeleteItem(nItem);
		}
	}
	m_ListDetailResult.SetRedraw(true);
	LeaveCriticalSection(&g_csFinishedProInfo);
	return;
}

void CAnalasisDlg::OnBnClickedButtonImportfinishedproject()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
void CAnalasisDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	// TODO: �ڴ˴������Ϣ����������
	//�����������Ŀ�б�����
	UpdateFinishedDB();
	ReleaseResource();
}
void CAnalasisDlg::UpdateFinishedDB()
{
	//��ȡ֮ǰ������û���Ϣ������еĻ�,����sqlite���ݿ�
	sqlite3* pFinishedProjectInfodb = NULL;

	//�����ݿ⣬�������򴴽�
	if(sqlite3_open("User\\FinishedProjectInfo.db3", &pFinishedProjectInfodb) != 0)
	{
		MessageBox(L"���������Ŀ����ʧ��",L"����ʧ��",MB_ICONERROR|MB_OK);
		return;
	}
	//�����û���Ϣ����Ѵ����򴴽�ʧ�ܣ�������Ϣ������szErrmsg��
	char sqlcmd[500] = "CREATE TABLE FinishedProjectInfo(\
				     ProjectName string PRIMARY KEY,\
					 FinishedProjectPath string,\
					 FinishedProjectInfoPath string,\
					 SendDate string,\
					 RecvDate string,\
					 Sender string,\
					 Result string)";

	char *pErrMsg = NULL;
	sqlite3_exec(pFinishedProjectInfodb,sqlcmd,0,0,&pErrMsg);

	//ɾ����������
	sqlite3_exec(pFinishedProjectInfodb,"DELETE from FinishedProjectInfo",0,0,&pErrMsg);

	int FinishedProCount = m_ary_FinishedPro.GetCount();
	int i = 0;
	while(i < FinishedProCount)
	{
		memset(sqlcmd,0,500);
		char *pFinishedProjectName = CString2Char(m_ary_FinishedPro[i].m_FinishedProjectName);
		char *pFinishedProjectPath = CString2Char(m_ary_FinishedPro[i].m_FinishedProjectPath);
		char *pFinishedProjectInfoPath = CString2Char(m_ary_FinishedPro[i].m_FinishedProjectInfoPath);
		char *pFinishedProjectSendDate = CString2Char(m_ary_FinishedPro[i].m_FinishedProjectSendDate);
		char *pFinishedProjectSenderName = CString2Char(m_ary_FinishedPro[i].m_FinishedProjectSenderName);
		char *pFinishedProjectRecvDate = CString2Char(m_ary_FinishedPro[i].m_FinishedProjectRecvDate);
		char *pFinishedProjectPreResult = CString2Char(m_ary_FinishedPro[i].m_FinishedProjectPreResult);

		_snprintf(sqlcmd,500,"INSERT INTO FinishedProjectInfo(ProjectName,FinishedProjectPath,FinishedProjectInfoPath,SendDate,RecvDate,Sender,Result)\
			                               values('%s','%s','%s','%s','%s','%s','%s')"
			,pFinishedProjectName
			,pFinishedProjectPath
			,pFinishedProjectInfoPath
			,pFinishedProjectSendDate
			,pFinishedProjectRecvDate
		    ,pFinishedProjectSenderName  
		    ,pFinishedProjectPreResult);

		free(pFinishedProjectName);
		free(pFinishedProjectPath);
		free(pFinishedProjectInfoPath);
		free(pFinishedProjectSendDate);
		free(pFinishedProjectSenderName);
		free(pFinishedProjectRecvDate);
		free(pFinishedProjectPreResult);
		sqlite3_exec(pFinishedProjectInfodb,sqlcmd,0,0,&pErrMsg);
		++i;
	}
}


void CAnalasisDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CDialogEx::OnClose();
}
void CAnalasisDlg::ReleaseResource(void)
{
	ExitAllThread=1;
	closesocket(g_hListenSocket);
	while(!(ThreadCheckConnectExited&ThreadReceiveResultExited&ThreadPackAndSendProjectExited&(!g_ThreadRecvingCount)))
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
	Sleep(100);
}
//��ӡҳͷ
void CAnalasisDlg::PrintPageHeader(CDC* pDC, UINT nPageNumber, CPrintInfo* pInfo)
{
	CFont *pOldFont;
	CFont fontHeader;
	fontHeader.CreatePointFont(100,L"FixedSys",pDC);
	pOldFont=(CFont *)(pDC->SelectObject(&fontHeader));
	int nPosY,nPosX;
	nPosY=3*m_cyPrinter;
	nPosX=m_cxOffset;
	pDC->TextOut(nPosX,nPosY,L"�׾Ƽ�������Դ");
	CString str;
	str.Format(L"�� %d ҳ / �� %d ҳ",nPageNumber,pInfo->GetMaxPage());
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CClientView * p_CClientView = (CClientView *)GetParent();
	p_CClientView->m_pCallPrintDlg = this;
	p_CClientView->CallPrint();
}
void CAnalasisDlg::OnBnClickedButtonPrintpreview()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CClientView * p_CClientView = (CClientView *)GetParent();
	p_CClientView->m_pCallPrintDlg = this;
	p_CClientView->CallPrintPreview();
}

afx_msg LRESULT CAnalasisDlg::OnBeginPrinting(WPARAM wParam, LPARAM lParam)
{
	//��ϸ����б�
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

    m_nLinesPerPage = (pDC->GetDeviceCaps(VERTRES) - (m_cyPrinter * (3 + (2 * PRINTMARGIN)))) / m_cyPrinter;

	m_MaxPage =  m_ListDetailResult.GetItemCount()/(m_nLinesPerPage-1);
	++m_MaxPage;

	pInfo->SetMaxPage (m_MaxPage); //��������ӡ����

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
	DataFont.CreatePointFont(120,L"����",pDC);
	PrintPageHeader(pDC,pInfo->m_nCurPage,  pInfo);
	TCHAR *pszTitle[3]={L"��Ŀ����",L"�����ļ���",L"��Դ���"};
	pOldFont=pDC->SelectObject(&DataFont);
	
	int nMaxPageNumber = pInfo->GetMaxPage();

	int TotalPrintItemNumber = m_ListDetailResult.GetItemCount(); //�ܴ�ӡ��Ŀ
	int RestPrintItemNumber = TotalPrintItemNumber - (nPageNumber-1)*(m_nLinesPerPage-1);//ʣ���ӡ��Ŀ
	int PrintItemNumber = 0; //��ǰҳҪ��ӡ����Ŀ

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

void CAnalasisDlg::OnBnClickedButtonDeletefinished()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_ListFinishedProject.GetSelectedCount()==0)
	{
		MessageBox(L"��ѡ��һ����Ŀɾ��",L"ɾ����Ŀ",MB_OK|MB_ICONWARNING);
	}

	int nIndex=m_ListFinishedProject.GetSelectionMark();

	EnterCriticalSection(&g_csFinishedProInfo);
	//��ȡѡ�����Ŀ��Դ���·��
	if(PathFileExists(m_ary_FinishedPro[nIndex].m_FinishedProjectPath))
	{
		DeleteFolder(m_ary_FinishedPro[nIndex].m_FinishedProjectPath);
	}
	m_ary_FinishedPro.RemoveAt(nIndex);
	m_ListFinishedProject.DeleteItem(nIndex);
	LeaveCriticalSection(&g_csFinishedProInfo);
	m_ListDetailResult.DeleteAllItems();
}

void CAnalasisDlg::OnBnClickedButtonComponentsanalasis()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_ListFinishedProject.GetSelectedCount()==0)
	{
		MessageBox(L"��ѡ��һ����Ŀ��",L"�������ʷ���",MB_ICONWARNING);
		return;
	}
	int nIndex=m_ListFinishedProject.GetSelectionMark();
	EnterCriticalSection(&g_csFinishedProInfo);

	CMainComponentsAnaDlg MainComponentsAnaDlg;
	MainComponentsAnaDlg.m_projectName=m_ary_FinishedPro[nIndex].m_FinishedProjectName;//projectFinishedName;
	MainComponentsAnaDlg.m_projectPath=m_ary_FinishedPro[nIndex].m_FinishedProjectPath;//->projectFinishedPath;
	LeaveCriticalSection(&g_csFinishedProInfo);

	MainComponentsAnaDlg.DoModal();
}
