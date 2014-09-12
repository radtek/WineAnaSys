// MainUIDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DataServerCenter.h"
#include "MainFrm.h"
#include "MainUIDlg.h"
#include "afxdialogex.h"
#include  <afxpriv.h>
#include <shlwapi.h>
#pragma comment(lib,"Shlwapi.lib") //如果没有这行，会出现link错误
#include "PredictRecord.h"
#include "PCASettingDlg.h"
#include "AnalasisHistory.h"

volatile int OnLineUsersNum = 0;
volatile int g_SendingThreadCount = 0;
//结束线程
volatile bool ExitAllThread;

//暂停线程
volatile bool PauseThreadAutoPreProject;
volatile bool ThreadAutoPreProjectPaused;

//线程结束标记
volatile bool AnswerThreadExited;
volatile bool ThreadThreadDearWithClientConnectExited;
volatile bool ThreadAutoPreProjectExited;
volatile bool ThreadSendResultExited;

CWnd *m_pMainUIDlg;
//当前目录绝对值全局地址
CString g_CurrentDir;
ConnectionList g_conns;//用户集
//待发送队列
list<SendInfo> g_waiteForSendList;
//监听套接字
SOCKET g_hListenSocket = INVALID_SOCKET;
//临界区
CRITICAL_SECTION g_csConns;    //添加删除或修改用户连接
CRITICAL_SECTION g_csListWaitPre; //添加删除或修改待分析项目list
CRITICAL_SECTION g_csListBeingPre; //修改分析中项目list
CRITICAL_SECTION g_csListFinishedAutoPredict;//添加删除或修改已完成分析项目list
CRITICAL_SECTION g_cswaiteForSendList;//添加删除或修改待发送项目list
CRITICAL_SECTION g_csProListShow;//更新待分析项目、已完成分析项目、待发送项目列表显示

// CMainUIDlg 对话框

IMPLEMENT_DYNAMIC(CMainUIDlg, CDialogEx)

CMainUIDlg::CMainUIDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMainUIDlg::IDD, pParent)
{
	pDlg=NULL;
}

CMainUIDlg::~CMainUIDlg()
{

}

void CMainUIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ONLINEUSER, m_ListOnlineUsers);
	DDX_Control(pDX, IDC_LIST_WAITEFORPREDICT, m_ListProjectWaitForPredict);
	DDX_Control(pDX, IDC_LIST_BEINGPREDICT, m_ListProjectBeingPredict);
	DDX_Control(pDX, IDC_LIST_FINISHEDPREDICT, m_ListFinishedAutoPredict);
	DDX_Control(pDX, IDC_LIST_PREDICTRESULTDETAILINFO, m_ListProPreResultDetail);
}


BEGIN_MESSAGE_MAP(CMainUIDlg, CDialogEx)
	ON_WM_SIZE()
	ON_NOTIFY(NM_CLICK, IDC_LIST_FINISHEDPREDICT, &CMainUIDlg::OnNMClickListFinishedpredict)
	ON_BN_CLICKED(IDC_BUTTON_MANUALCOMPARE, &CMainUIDlg::OnBnClickedButtonManualcompare)
	ON_WM_PAINT()
	ON_MESSAGE(WM_SENDRESULTCONDITION, &CMainUIDlg::OnSendresultcondition)
	ON_WM_NCDESTROY()
	ON_MESSAGE(WM_CONTINUEPREDICTPROJECT, &CMainUIDlg::OnContinuepredictproject)
	ON_MESSAGE(WM_PAUSEPREDICTPROJECT, &CMainUIDlg::OnPausepredictproject)
	ON_BN_CLICKED(IDC_BUTTON_PREDICTEDRECORD, &CMainUIDlg::OnBnClickedButtonPredictedrecord)
	ON_MESSAGE(WM_ADDONEPROJECT, &CMainUIDlg::OnAddoneproject)
END_MESSAGE_MAP()

// CMainUIDlg 消息处理程序
CWnd *IOContext::pWnd = NULL;

BOOL CMainUIDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	WSAData wsaData;
	int nCode;
	if((nCode = WSAStartup(MAKEWORD(2,2),&wsaData)) != 0)
	{
		//cout<<"WSAStartup error:"<<WSAGetLastError()<<endl;
		return 0;
	}
	//初始化当前目录的绝对值
	
	GetCurrentDirectory(MAX_PATH,g_CurrentDir.GetBuffer(MAX_PATH));
	g_CurrentDir.ReleaseBuffer();

	IOContext::pWnd = this;	
	InitializeCriticalSection(&g_csConns);
	InitializeCriticalSection(&g_csListFinishedAutoPredict);
	InitializeCriticalSection(&g_cswaiteForSendList);
	InitializeCriticalSection(&g_csListWaitPre);
	InitializeCriticalSection(&g_csListBeingPre);
	InitializeCriticalSection(&g_csProListShow);
	m_pMainUIDlg = this;

	ExitAllThread=false;
	PauseThreadAutoPreProject = false;
	ThreadAutoPreProjectPaused = false;

	AnswerThreadExited=1;
	ThreadThreadDearWithClientConnectExited=1;
	ThreadAutoPreProjectExited=1;
	ThreadSendResultExited=1;

	m_ListOnlineUsers.InsertColumn(0,L"在线用户", LVCFMT_LEFT, 200 );
	m_ListOnlineUsers.InsertColumn(1,L"IP地址", LVCFMT_LEFT, 200 );


	m_ListProjectWaitForPredict.InsertColumn(0,L"项目名称", LVCFMT_LEFT, 100 );
	m_ListProjectWaitForPredict.InsertColumn(1,L"送检用户", LVCFMT_LEFT, 100 );
	m_ListProjectWaitForPredict.InsertColumn(2,L"送检日期", LVCFMT_LEFT, 100 );
	m_ListProjectWaitForPredict.InsertColumn(3,L"送检类型", LVCFMT_LEFT, 100 );

	m_ListProjectBeingPredict.InsertColumn(0,L"项目名称", LVCFMT_LEFT, 100 );
	m_ListProjectBeingPredict.InsertColumn(1,L"送检用户", LVCFMT_LEFT, 100 );
	m_ListProjectBeingPredict.InsertColumn(2,L"送检日期", LVCFMT_LEFT, 100 );
	m_ListProjectBeingPredict.InsertColumn(3,L"送检类型", LVCFMT_LEFT, 100 );

	m_ListFinishedAutoPredict.InsertColumn(0,L"项目名称", LVCFMT_LEFT, 100 );
	m_ListFinishedAutoPredict.InsertColumn(1,L"送检用户", LVCFMT_LEFT, 100 );
	m_ListFinishedAutoPredict.InsertColumn(2,L"送检日期", LVCFMT_LEFT, 100 );
	m_ListFinishedAutoPredict.InsertColumn(3,L"送检类型", LVCFMT_LEFT, 100 );
	m_ListFinishedAutoPredict.InsertColumn(4,L"检测结果", LVCFMT_LEFT, 100 );
	m_ListFinishedAutoPredict.InsertColumn(5,L"回送情况", LVCFMT_LEFT, 100 );

	m_ListProPreResultDetail.InsertColumn(0,L"项目名称", LVCFMT_LEFT, 150 );
	m_ListProPreResultDetail.InsertColumn(1,L"光谱文件", LVCFMT_LEFT, 150 );
	m_ListProPreResultDetail.InsertColumn(2,L"检测结果", LVCFMT_LEFT, 150 );


	DWORD dwStyle = m_ListOnlineUsers.GetExtendedStyle(); //获取当前扩展样式
	dwStyle |= LVS_EX_FULLROWSELECT; //选中某行使整行高亮（report风格时）
	dwStyle |= LVS_EX_GRIDLINES; //网格线（report风格时）

	m_ListOnlineUsers.SetExtendedStyle(dwStyle);           //设置扩展风格
	m_ListProjectWaitForPredict.SetExtendedStyle(dwStyle); //设置扩展风格
	m_ListProjectBeingPredict.SetExtendedStyle(dwStyle);   //设置扩展风格
	m_ListFinishedAutoPredict.SetExtendedStyle(dwStyle);   //设置扩展风格
	m_ListProPreResultDetail.SetExtendedStyle(dwStyle);    //设置扩展风格

	AddAllProject();

	AfxBeginThread(ThreadDearWithClientConnect,this);
	pThreadAutoPreProject=AfxBeginThread(ThreadAutoPreProject,this);
	pThreadCheckSendCondition=AfxBeginThread(ThreadCheckSendCondition,this);
	pThreadSendResult=AfxBeginThread(ThreadSendResult,this);

	GetClientRect(&m_rect);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

//采用非阻塞模式处理客户端的通信
SOCKET BindListen()
{
	SOCKET sd = socket(AF_INET,SOCK_STREAM,0);
	if(sd == INVALID_SOCKET)
	{
		int lastErr = WSAGetLastError();
		cout<<"create listen socket error :"<<WSAGetLastError()<<endl;
		return INVALID_SOCKET;
	}
	sockaddr_in saListen;
	saListen.sin_family = AF_INET;
	saListen.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

	saListen.sin_port = htons(PORT);

	if(SOCKET_ERROR == bind(sd,(sockaddr *)&saListen,sizeof(saListen)))
	{
		int lastErr = WSAGetLastError();
		cout<<"bind error : "<<WSAGetLastError()<<endl;
		closesocket(sd);
		return INVALID_SOCKET;
	}
	if(listen(sd,5) == SOCKET_ERROR)
	{
		int lastErr = WSAGetLastError();
		cout<<"listen error : "<<WSAGetLastError()<<endl;
		closesocket(sd);
		return INVALID_SOCKET;
	}

	return sd;
}

SOCKET AcceptConnection(SOCKET sdListen,sockaddr_in &saRemote)
{	
	int nSize = sizeof(saRemote);
	SOCKET sa = accept(sdListen,(sockaddr *)&saRemote,&nSize);
	if(INVALID_SOCKET == sa)
	{
		cout<<"accept error :"<<WSAGetLastError()<<endl;
	}
	return sa;
}
void AddToList(IOContext *pIOContext)
{
	EnterCriticalSection(&g_csConns);
	ConnectionList::iterator it = g_conns.begin();
	while(it!=g_conns.end())
	{
		if((*it)->UserName == pIOContext->UserName)
			break;
	}
	if(it != g_conns.end())
	{
		closesocket((*it)->hSocket);
		//删除旧用户
		CMainUIDlg* pCMainUIDlg = (CMainUIDlg*)(*it)->pWnd;
		LVFINDINFO info;
		int nIndex;
		info.flags = LVFI_PARTIAL|LVFI_STRING;
		info.psz = (*it)->UserName;
		// Delete all of the items that begin with the string.
		while ((nIndex = pCMainUIDlg->m_ListOnlineUsers.FindItem(&info)) != -1)
		{
			pCMainUIDlg->m_ListOnlineUsers.DeleteItem(nIndex);
		}
		delete *it;
		g_conns.erase(it);

	}
	g_conns.push_back(pIOContext);
	LeaveCriticalSection(&g_csConns);
}

void RemoveFromList(IOContext *pIOContext)
{
	EnterCriticalSection(&g_csConns);
	//删除用户列表中相关的项
	CMainUIDlg* pCMainUIDlg = (CMainUIDlg*)pIOContext->pWnd;
	LVFINDINFO info;
	int nIndex;
	info.flags = LVFI_PARTIAL|LVFI_STRING;
	info.psz = pIOContext->UserName;
	// Delete all of the items that begin with the string.
	while ((nIndex = pCMainUIDlg->m_ListOnlineUsers.FindItem(&info)) != -1)
	{
		pCMainUIDlg->m_ListOnlineUsers.DeleteItem(nIndex);
	}

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
UINT ThreadDearWithClientConnect(LPVOID lpParam)
{
	ThreadThreadDearWithClientConnectExited=0;
	CMainUIDlg *pCMainUIDlg=(CMainUIDlg *)lpParam;

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

		//AfxBeginThread(,WorkThread,NULL,0,(PTHREAD_START),hIOCP,0,,&dwThreadID);
		HANDLE hThread = (HANDLE)_beginthreadex(NULL,0,(PTHREAD_START)WorkThread,hIOCP,0,&dwThreadID);
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
		pIOContext->UserIP = Char2CString(inet_ntoa(saRemote.sin_addr)); //获取用户IP
		DWORD dwFlags = 0;
		//发送公钥
		int nRet = WSASend(hAcceptSocket,
			&(pIOContext->wsaBuffer),
			1,
			NULL,
			dwFlags,
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
			PostQueuedCompletionStatus(hIOCP,0,EXIT_THREAD,NULL);
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
		g_hListenSocket = INVALID_SOCKET;
	}
	ThreadThreadDearWithClientConnectExited=1;
	return 0;
}
DWORD WINAPI WorkThread(LPVOID param)
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

		if(bOK && ulKey == EXIT_THREAD)
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
			int n = g_conns.size();
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
			int n = g_conns.size();
			continue;
		}
		if(pIOContext->op == IORSA)
		{
			ZeroMemory(pIOContext->Buffer,DATA_BUFSIZE);//清空缓冲区

			pIOContext->nBytesInBuf = 0;// += dwBytes;
			pIOContext->wsaBuffer.buf = pIOContext->Buffer;
			pIOContext->wsaBuffer.len = DATA_BUFSIZE;//pIOContext->nBytesInBuf;

			ZeroMemory(pIOContext,sizeof(WSAOVERLAPPED));
			pIOContext->op = IOUserInfo;//要求接收用户信息
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
		else if(pIOContext->op == IOUserInfo)
		{
			//前32位是用户名，中32位是密码，后32位是端口号
			unsigned char userInfoEncode[DATA_BUFSIZE] = {0};
			unsigned char userInfoDecode[DATA_BUFSIZE] = {0};
			memcpy(userInfoEncode,pIOContext->Buffer,DATA_BUFSIZE);
			//解密
			int inputlen=RSA_size(pIOContext->rsa_key);
			int ret = RSA_private_decrypt(inputlen,userInfoEncode,userInfoDecode, pIOContext->rsa_key, RSA_NO_PADDING);
			if (ret < 0)
			{
				CloseIO(pIOContext);
				continue;
			}
			CString UserInfo = Char2CString((char*)userInfoDecode);

			int pos1 = 0,pos2 = 0;
			pos2 = UserInfo.Find(L",",pos1);
			if(pos2 == -1)
			{
				CloseIO(pIOContext);
				continue;
			}
			pIOContext->UserName = UserInfo.Mid(pos1,pos2-pos1);
			pos1 = pos2+1;
			pos2 =  UserInfo.Find(L",",pos1);
			if(pos2 == -1)
			{
				CloseIO(pIOContext);
				continue;
			}
			CString PassWord = UserInfo.Mid(pos1,pos2-pos1);
			if(pos2 == -1)
			{
				CloseIO(pIOContext);
				continue;
			}
			pIOContext->Port = _wtoi(UserInfo.Mid(pos2+1,UserInfo.GetLength()-pos2-1));

			//检查用户名及密码是否有误
			CUserManage UserManage;
			//
			if(!UserManage.Open())
			{
				//连接数据库失败
				CloseIO(pIOContext);
				continue;
			}
			UserManage.m_strFilter.Format(L"UserName = '%s'",pIOContext->UserName);
			UserManage.Requery();
			if(0 == UserManage.GetRecordCount())
			{
				//用户名不存在
				ZeroMemory(pIOContext->Buffer,DATA_BUFSIZE);//清空缓冲区
				memcpy(pIOContext->Buffer,"UserNonExist",sizeof("UserNonExist"));
				pIOContext->nBytesInBuf = sizeof("UserNonExist");
				pIOContext->wsaBuffer.buf = pIOContext->Buffer;
				pIOContext->wsaBuffer.len = sizeof("UserNonExist");

				ZeroMemory(pIOContext,sizeof(WSAOVERLAPPED));
				pIOContext->op = IORecvType;//接收连接状态，用户收到消息后会自己断开连接
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
				continue;
			}
			UserManage.m_strFilter.Format(L"UserName = '%s' AND PassWord = '%s'",pIOContext->UserName,PassWord);
			UserManage.Requery();
			if(0 == UserManage.GetRecordCount())
			{
				//密码错误
				ZeroMemory(pIOContext->Buffer,DATA_BUFSIZE);//清空缓冲区
				memcpy(pIOContext->Buffer,"IncorrectPassWord",sizeof("IncorrectPassWord"));
				pIOContext->nBytesInBuf = sizeof("IncorrectPassWord");
				pIOContext->wsaBuffer.buf = pIOContext->Buffer;
				pIOContext->wsaBuffer.len = sizeof("IncorrectPassWord");

				ZeroMemory(pIOContext,sizeof(WSAOVERLAPPED));
				pIOContext->op = IORecvType;//接收连接状态，用户收到消息后会自己断开连接
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
				continue;
			}
			//验证成功
			ZeroMemory(pIOContext->Buffer,DATA_BUFSIZE);//清空缓冲区
			pIOContext->nBytesInBuf = 0;
			pIOContext->wsaBuffer.buf = pIOContext->Buffer;
			pIOContext->wsaBuffer.len = DATA_BUFSIZE;
			AddToList(pIOContext);   //列表添加新用户
			CMainUIDlg *pCMainUIDlg = (CMainUIDlg*)pIOContext->pWnd;
			EnterCriticalSection(&g_csListFinishedAutoPredict);
			int n = pCMainUIDlg->m_ListFinishedAutoPredict.GetItemCount();
			for (int i=0;i<n; i++)
			{
				CString username = pCMainUIDlg->m_ListFinishedAutoPredict.GetItemText(i,1);
				CString sendcondition = pCMainUIDlg->m_ListFinishedAutoPredict.GetItemText(i,5);
				if (username.Compare(pIOContext->UserName) == 0&&sendcondition.Compare(L"用户处于离线状态")==0)
				{
					pCMainUIDlg->m_ListFinishedAutoPredict.SetItemText(i,5,L"准备发送");
				}
			}
			LeaveCriticalSection(&g_csListFinishedAutoPredict);

			//刷新在线用户列表...OnLineUsersNum
			int Last=pCMainUIDlg->m_ListOnlineUsers.GetItemCount();
			pCMainUIDlg->m_ListOnlineUsers.InsertItem(Last,pIOContext->UserName);
			pCMainUIDlg->m_ListOnlineUsers.SetItemText(Last,1,pIOContext->UserIP);
			//发送登录成功消息
			ZeroMemory(pIOContext->Buffer,DATA_BUFSIZE);//清空缓冲区
			memcpy(pIOContext->Buffer,"LoginSuccess",sizeof("LoginSuccess"));
			pIOContext->nBytesInBuf = sizeof("LoginSuccess");
			pIOContext->wsaBuffer.buf = pIOContext->Buffer;
			pIOContext->wsaBuffer.len = sizeof("LoginSuccess");
			ZeroMemory(pIOContext,sizeof(WSAOVERLAPPED));
			pIOContext->op = IOStatus;//IO状态切换
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
		else if(pIOContext->op == IORecvType)
		{
			if(strcmp(pIOContext->Buffer,"Connect") == 0)//测试连接状态
			{
				ZeroMemory(pIOContext->Buffer,DATA_BUFSIZE);//清空缓冲区
				pIOContext->nBytesInBuf = 0;//
				pIOContext->wsaBuffer.buf = pIOContext->Buffer;
				pIOContext->wsaBuffer.len = DATA_BUFSIZE;//pIOContext->nBytesInBuf;
				ZeroMemory(pIOContext,sizeof(WSAOVERLAPPED));
				pIOContext->op = IORecvType;  // 继续接收服务类型
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
			else if(strcmp(pIOContext->Buffer,"TransmitProjet") == 0)//发送项目文件
			{

				ZeroMemory(pIOContext->Buffer,DATA_BUFSIZE);//清空缓冲区
				pIOContext->nBytesInBuf = 0;// += dwBytes;
				pIOContext->wsaBuffer.buf = pIOContext->Buffer;
				pIOContext->wsaBuffer.len = DATA_BUFSIZE;//pIOContext->nBytesInBuf;
				ZeroMemory(pIOContext,sizeof(WSAOVERLAPPED));
				pIOContext->op = IORecvFileName;  // 开始准备接收项目文件
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
		}
		else if(pIOContext->op == IORecvFileName)
		{
			pIOContext->FileName = Char2CString(pIOContext->Buffer);//文件名

			CString ProjectName=pIOContext->FileName.Left(pIOContext->FileName.Find(L"."));

			//保存路径并创建文件
			strcat(pIOContext->FilePath,pIOContext->Buffer);	
			pIOContext->oFile.open(pIOContext->FilePath,ios::out|ios::binary);
			ZeroMemory(pIOContext->Buffer,DATA_BUFSIZE);//清空缓冲区

			pIOContext->nBytesInBuf = 0;// += dwBytes;
			pIOContext->wsaBuffer.buf = pIOContext->Buffer;
			pIOContext->wsaBuffer.len = DATA_BUFSIZE;//pIOContext->nBytesInBuf;

			ZeroMemory(pIOContext,sizeof(WSAOVERLAPPED));
			pIOContext->op = IOFileSize;  //要求接收文件大小
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
		else if(pIOContext->op == IOFileSize)
		{
			pIOContext->nBytesFileToRecv = atoi(pIOContext->Buffer);//要接收文件大小

			ZeroMemory(pIOContext->Buffer,DATA_BUFSIZE);//清空缓冲区

			pIOContext->nBytesInBuf = 0;// += dwBytes;
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
				pIOContext->nBytesInBuf = 0;// += dwBytes;
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

				CString FilePath=g_CurrentDir;
				FilePath+=L"\\";
				CString tempStr=Char2CString(pIOContext->FilePath);
				FilePath+=tempStr;

				CString DirUnpack =g_CurrentDir;
				DirUnpack+=L"\\Project\\WaitForAutoAnalasis";

				CZipImplement ZipImplement;
				if(ZipImplement.Zip_UnPackFiles(FilePath,DirUnpack)==FALSE)
				{
					//解压缩失败
					ZeroMemory(pIOContext->Buffer,DATA_BUFSIZE);//清空缓冲区
					memcpy(pIOContext->Buffer,"Failed",sizeof("Failed"));
					pIOContext->nBytesInBuf = sizeof("Failed");// += dwBytes;
					pIOContext->wsaBuffer.buf = pIOContext->Buffer;
					pIOContext->wsaBuffer.len = sizeof("Failed");//pIOContext->nBytesInBuf;

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
					CString DirNew=L"FileRecv\\UnpackedZipFile\\";
					MoveFile(FilePath,DirNew);
					CString ProjectPath=DirUnpack;
					ProjectPath+=L"\\";
					ProjectPath+=pIOContext->FileName.Left(pIOContext->FileName.Find(L"."));

					//添加新接收的项目到待分析列表
					CMainUIDlg* pCMainUIDlg = (CMainUIDlg*)pIOContext->pWnd;

					if(pCMainUIDlg->AddOneProject(ProjectPath)!=0)
					{
						//失败
						ZeroMemory(pIOContext->Buffer,DATA_BUFSIZE);//清空缓冲区
						memcpy(pIOContext->Buffer,"Failed",sizeof("Failed"));
						pIOContext->nBytesInBuf = sizeof("Failed");
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
						//成功
						ZeroMemory(pIOContext->Buffer,DATA_BUFSIZE);//清空缓冲区
						memcpy(pIOContext->Buffer,"Success",sizeof("Success"));
						pIOContext->nBytesInBuf = sizeof("Success");// += dwBytes;
						pIOContext->wsaBuffer.buf = pIOContext->Buffer;
						pIOContext->wsaBuffer.len = sizeof("Success");//pIOContext->nBytesInBuf;

						ZeroMemory(pIOContext,sizeof(WSAOVERLAPPED));
						pIOContext->op = IOStatus;//IO状态切换
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
			ZeroMemory(pIOContext->Buffer,DATA_BUFSIZE);//清空缓冲区
			pIOContext->nBytesInBuf = 0;// += dwBytes;
			pIOContext->wsaBuffer.buf = pIOContext->Buffer;
			pIOContext->wsaBuffer.len = DATA_BUFSIZE;//pIOContext->nBytesInBuf;
			memcpy(pIOContext->FilePath,"FileRecv\\",sizeof("FileRecv\\"));
			pIOContext->nBytesFileReceived = 0;
			pIOContext->nBytesFileToRecv = 0;

			ZeroMemory(pIOContext,sizeof(WSAOVERLAPPED));
			pIOContext->op = IORecvType;//继续开始接收服务类型
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
	}
	return 0;
}
afx_msg LRESULT CMainUIDlg::OnUserlougoout(WPARAM wParam, LPARAM lParam)
{

	return 0;
}

//自动鉴定线程，采用线程池技术，实现同时对多个项目进行鉴定
UINT ThreadAutoPreProject(LPVOID lpParam)
{
	AfxOleInit();
	ThreadAutoPreProjectExited = false;
	ThreadAutoPreProjectPaused = false;
	CMainUIDlg* pCMainUIDlg=(CMainUIDlg*) lpParam;

	// 创建线程池
	PTP_POOL pThreadpool = CreateThreadpool(NULL);    
	// 设置线程池线程数量上下限
	SetThreadpoolThreadMinimum(pThreadpool, 2);
	SetThreadpoolThreadMaximum(pThreadpool, 10);
	// 初始化“回调函数环境”结构
	TP_CALLBACK_ENVIRON tcbe;
	InitializeThreadpoolEnvironment(&tcbe);
	// 将该回调函数环境结构与线程池相关联
	SetThreadpoolCallbackPool(&tcbe, pThreadpool);
	// 创建清理组
	PTP_CLEANUP_GROUP pTpcg= CreateThreadpoolCleanupGroup();
	// 将回调函数环境结构与清理组关联起来
	SetThreadpoolCallbackCleanupGroup(&tcbe, pTpcg, NULL);

	while(!ExitAllThread)
	{
		if(ExitAllThread)
		{
			break;
		}
		while(PauseThreadAutoPreProject)
		{
			Sleep(100);
			ThreadAutoPreProjectPaused = 1;
		}
		ThreadAutoPreProjectPaused = false;

		//项目列表中是否存在待分析项目list<proWaitPreInfo>  projectWaitePreList;
		if(!(pCMainUIDlg->projectWaitePreList.empty()))
		{	
			PreProInfo *pPreProInfo = new PreProInfo();

			EnterCriticalSection(&g_csListWaitPre);
			list<proWaitPreInfo>::iterator itproWaitPreInfo = pCMainUIDlg->projectWaitePreList.begin();
			pPreProInfo->projectNameBeingPre= itproWaitPreInfo->projectName;//*iterName;
			pPreProInfo->projectInfoPathBeingPre= itproWaitPreInfo->projectInfoPath;//*iterInfoPath;
			pPreProInfo->projectPathBeingPre= itproWaitPreInfo->projectPath;//*iterPath;
			pPreProInfo->projectUserNameBeingPre= itproWaitPreInfo->projectUserName;//*iterUserName;
			pPreProInfo->projectRecvDateBeingPre= itproWaitPreInfo->projectRecvDate;//*iterRecvDate;
			pPreProInfo->projectPredictTypeBeingPre= itproWaitPreInfo->projectPredictType;//*iterPredictType;
			pCMainUIDlg->projectWaitePreList.pop_front();
			LeaveCriticalSection(&g_csListWaitPre);

			EnterCriticalSection(&g_csListBeingPre);
			pCMainUIDlg->projectBeingPreList.push_back(*pPreProInfo);
			LeaveCriticalSection(&g_csListBeingPre);

			//更新显示
			EnterCriticalSection(&g_csProListShow);
			pCMainUIDlg->UpdateProjectList();
			LeaveCriticalSection(&g_csProListShow);

			//加入线程池
			TrySubmitThreadpoolCallback(PredictCallback,(PVOID)pPreProInfo,&tcbe);
			Sleep(500);
		}
		else
		{
			Sleep(100);
		}		
	}

	// 等待所有项完成
	CloseThreadpoolCleanupGroupMembers(pTpcg, FALSE, NULL);

	// 关闭各个项
	CloseThreadpoolCleanupGroup(pTpcg);    // 关闭线程池清理组
	DestroyThreadpoolEnvironment(&tcbe);    // 删除回调函数环境结构
	CloseThreadpool(pThreadpool);    // 关闭线程池

	ThreadAutoPreProjectExited=1;
	return 0;
}
//鉴定过程函数
VOID NTAPI PredictCallback(PTP_CALLBACK_INSTANCE pInstance,PVOID pvContext)
{
	CoInitialize(NULL);
	CMainUIDlg* pMainUIDlg = (CMainUIDlg*)m_pMainUIDlg;
	PreProInfo *pPreProInfo = (PreProInfo*)pvContext;
	//开始鉴定
	CString ProjectPath=pPreProInfo->projectPathBeingPre;

	//光谱文件名
	CArray<CString,CString> specTitle;

	ProjectPath+="\\*.*";
	//找出项目中的光谱文件
	CArray<CString,CString> specPathArray;

	CFileFind finder; 
	BOOL bWorking = finder.FindFile(ProjectPath);

	while (bWorking)  
	{
		if(ExitAllThread)
		{
			ThreadAutoPreProjectExited=1;
			return;
		}
		while(PauseThreadAutoPreProject)
		{
			Sleep(100);
			ThreadAutoPreProjectPaused = 1;
		}
		ThreadAutoPreProjectPaused = false;


		bWorking = finder.FindNextFile();
		CString str=finder.GetFileName();
		if(str.Right(4)==L".CSV"||str.Right(4)==L".csv" &&L"." !=str &&L".." != str)//注意该句需要排除“.”“..”
		{
			CString filename;
			CString fileTitle;
			specPathArray.Add(finder.GetFilePath());
			specTitle.Add(finder.GetFileTitle());
		}

	}
	finder.Close();

	//将光谱路径按自然顺序排列
	SortCArrayByFirst(specTitle,specPathArray);
	//存放鉴定结果
	CMatrix Results(specTitle.GetCount(),1);

	for(int i=0;i<specTitle.GetCount();i++)
	{
		if(ExitAllThread)
		{
			delete pPreProInfo;
			pPreProInfo = NULL;
			return ;
		}
		while(PauseThreadAutoPreProject)
		{
			Sleep(100);
			ThreadAutoPreProjectPaused = 1;
		}
		ThreadAutoPreProjectPaused = false;

		Results.SetItem(i+1,1,PredictProject(pPreProInfo->projectPathBeingPre,specTitle[i]));
	}

	//得到所有不同的鉴定结果
	CMatrix ResultsTypes=Results.GetDistinct();

	//得到每种鉴定结果的数量
	CMatrix ResultsCount(1,ResultsTypes.m_column,0);
	for(int i=1;i<=ResultsTypes.m_column;i++)
	{
		ResultsCount.SetItem(1,i,Results.Count(ResultsTypes(1,i)));
	}
	CMatrixIndex Maxindex;

	//找到鉴定结果最多的结果
	ResultsCount.FindMax(Maxindex);

	double reliability=(ResultsCount(Maxindex.row,Maxindex.column)/(ResultsCount.Sum()))*100;

	//生成鉴定结果文件，excel存放
	CString ResultExcelmodelPath=g_CurrentDir;
	ResultExcelmodelPath+=L"\\";
	ResultExcelmodelPath+=L"Project\\鉴定结果.xlsx";

	CExcelOperator 	ExcelOperator;
	int retn=ExcelOperator.LoadExcel(ResultExcelmodelPath);

	CWineTypeIndex WineTypeIndex;
	WineTypeIndex.Open();

	int i=0;
	for(;i<specTitle.GetCount();i++)
	{
		CString specName=specTitle[i];
		specName+=L".CSV";
		WineTypeIndex.m_strFilter.Format(L"TypeIndex = '%d'",(int)(Results(i+1,1)));
		WineTypeIndex.Requery();
		ExcelOperator.SetItem(i+1,1,specName);
		ExcelOperator.SetItem(i+1,2,WineTypeIndex.m_WineName);
	}

	//最终鉴定结果
	WineTypeIndex.m_strFilter.Format(L"TypeIndex = '%d'",
		(int)(ResultsTypes(Maxindex.row,Maxindex.column)));
	WineTypeIndex.Requery();

	ExcelOperator.SetItem(i+2,1,_T("鉴定结果"));
	ExcelOperator.SetItem(i+2,2,WineTypeIndex.m_WineName);

	CString szReliability;
	szReliability.Format(L"%f",reliability);
	szReliability+=L"%";
	ExcelOperator.SetItem(i+3,1,_T("可靠度"));
	ExcelOperator.SetItem(i+3,2,szReliability);
	//处理分析结果...
	CString ResultPath=pPreProInfo->projectPathBeingPre;
	ResultPath+="\\鉴定结果.xlsx";
	ExcelOperator.SaveExcel(ResultPath);
	ExcelOperator.ReleaseExcel();
	//转移完成自动分析的项目
	CString FinishedPath=g_CurrentDir;
	FinishedPath+=L"\\Project\\AnalasisFinished\\";
	FinishedPath+=pPreProInfo->projectNameBeingPre;
	MoveFolder(pPreProInfo->projectPathBeingPre,FinishedPath);

	CString projectFinishedInfoPath=g_CurrentDir;
	projectFinishedInfoPath+=L"\\Project\\AnalasisFinished\\";
	projectFinishedInfoPath+=pPreProInfo->projectNameBeingPre;
	projectFinishedInfoPath+=L"\\项目信息.xlsx";
	//更新list
	proFinInfo projectFinished;
	projectFinished.projectFinishedName = pPreProInfo->projectNameBeingPre;
	projectFinished.projectFinishedInfoPath = projectFinishedInfoPath;
	projectFinished.projectFinishedPath = FinishedPath;
	projectFinished.projectFinishedUserName = pPreProInfo->projectUserNameBeingPre;
	projectFinished.projectFinishedRecvDate = pPreProInfo->projectRecvDateBeingPre;
	projectFinished.projectFinishedSendBack = L"未完成";
	projectFinished.projectFinishedPredictType = pPreProInfo->projectPredictTypeBeingPre;
	projectFinished.projectFinishedPreResult = WineTypeIndex.m_WineName;

	EnterCriticalSection(&g_csListFinishedAutoPredict);
	pMainUIDlg->projectFinishedList.push_back(projectFinished);
	LeaveCriticalSection(&g_csListFinishedAutoPredict);

	//添加已完成记录到库中
	CPredictRecord PredictRecord;
	if(!PredictRecord.Open())
	{
		//打开数据库失败
		MessageBox(pMainUIDlg->m_hWnd,L"打开数据库失败",L"添加已完成项目记录失败",MB_ICONWARNING);
		return;
	}

	SYSTEMTIME sys; 
	GetLocalTime(&sys); 
	CString sTime;
	sTime.Format(L"%d-%d-%d",sys.wYear,sys.wMonth,sys.wDay);

	ExcelOperator.LoadExcel(projectFinishedInfoPath);

	CString RecvDate;
	ExcelOperator.GetItem(10,1,L"Date",RecvDate);

	PredictRecord.AddNew();
	PredictRecord.m_ProjectName=pPreProInfo->projectNameBeingPre;
	PredictRecord.m_SpecNum.Format(L"%d",specTitle.GetCount());
	PredictRecord.m_RecvDate=RecvDate;
	PredictRecord.m_PredictResult=WineTypeIndex.m_WineName;
	PredictRecord.m_Reliablility.Format(L"%f",reliability);
	PredictRecord.m_PredictDate=sTime;
	PredictRecord.Update();

	WineTypeIndex.Close();

	//更新列表显示
	EnterCriticalSection(&g_csListBeingPre);
	list<PreProInfo>::iterator iterprojectBeingPreList = pMainUIDlg->projectBeingPreList.begin();
	while(iterprojectBeingPreList!= pMainUIDlg->projectBeingPreList.end())
	{
		if(iterprojectBeingPreList->projectNameBeingPre == pPreProInfo->projectNameBeingPre)
			break;
		++iterprojectBeingPreList;
	}
	pMainUIDlg->projectBeingPreList.erase(iterprojectBeingPreList);
	LeaveCriticalSection(&g_csListBeingPre);

	delete pPreProInfo;
	pPreProInfo = NULL;

	EnterCriticalSection(&g_csProListShow);
	pMainUIDlg->UpdateProjectList();
	LeaveCriticalSection(&g_csProListShow);
	Sleep(100);

}

//UpdateProjectList之前请确保没有对list进行更新
int CMainUIDlg::UpdateProjectList(void)
{
	EnterCriticalSection(&g_csListWaitPre);
	list<proWaitPreInfo>::iterator itprojectWaitePreList = projectWaitePreList.begin();
	int i=0;
	//更新待鉴定项目列表
	m_ListProjectWaitForPredict.DeleteAllItems();
	while(itprojectWaitePreList!=projectWaitePreList.end())
	{
		m_ListProjectWaitForPredict.InsertItem(i,itprojectWaitePreList->projectName);
		m_ListProjectWaitForPredict.SetItemText(i,1,itprojectWaitePreList->projectUserName);
		m_ListProjectWaitForPredict.SetItemText(i,2,itprojectWaitePreList->projectRecvDate);
		m_ListProjectWaitForPredict.SetItemText(i,3,itprojectWaitePreList->projectPredictType);
		++itprojectWaitePreList;
		i++;
	}
	LeaveCriticalSection(&g_csListWaitPre);

	//更新鉴定中的项目列表
	i=0;
	EnterCriticalSection(&g_csListBeingPre);
	list<PreProInfo>::iterator itBeingPreList = projectBeingPreList.begin();	
	m_ListProjectBeingPredict.DeleteAllItems();
	while(itBeingPreList!=projectBeingPreList.end())
	{
		m_ListProjectBeingPredict.InsertItem(i,itBeingPreList->projectNameBeingPre);
		m_ListProjectBeingPredict.SetItemText(i,1,itBeingPreList->projectUserNameBeingPre);
		m_ListProjectBeingPredict.SetItemText(i,2,itBeingPreList->projectRecvDateBeingPre);
		m_ListProjectBeingPredict.SetItemText(i,3,itBeingPreList->projectPredictTypeBeingPre);

		itBeingPreList++;
		i++;
	}
	LeaveCriticalSection(&g_csListBeingPre);
	//更新已自动鉴定完成的项目列表
	i=0;
	EnterCriticalSection(&g_csListFinishedAutoPredict);
	list<proFinInfo>::iterator itprojectFinishedList = projectFinishedList.begin();	
	m_ListFinishedAutoPredict.DeleteAllItems();
	while(itprojectFinishedList!=projectFinishedList.end())
	{
		m_ListFinishedAutoPredict.InsertItem(i,itprojectFinishedList->projectFinishedName);
		m_ListFinishedAutoPredict.SetItemText(i,1,itprojectFinishedList->projectFinishedUserName);
		m_ListFinishedAutoPredict.SetItemText(i,2,itprojectFinishedList->projectFinishedRecvDate);
		m_ListFinishedAutoPredict.SetItemText(i,3,itprojectFinishedList->projectFinishedPredictType);
		m_ListFinishedAutoPredict.SetItemText(i,4,itprojectFinishedList->projectFinishedPreResult);
		m_ListFinishedAutoPredict.SetItemText(i,5,itprojectFinishedList->projectFinishedSendBack);
		itprojectFinishedList++;
		i++;
	}
	LeaveCriticalSection(&g_csListFinishedAutoPredict);
	return 0;
}
//初始化时添加待鉴定项目与已分析项目
int CMainUIDlg::AddAllProject()
{
	// 遍历得到所有子文件夹名
	CFileFind finder; 
	CString strDirWaiteAna =g_CurrentDir;
	strDirWaiteAna+=L"\\Project\\WaitForAutoAnalasis\\*.*";

	CString strDirFinishedAna =g_CurrentDir;
	strDirFinishedAna +=L"\\Project\\AnalasisFinished\\*.*";

	BOOL bWorking = finder.FindFile(strDirWaiteAna);
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

			proWaitPreInfo projectWaitPreInfo;
			BOOL success2=finderIsProject.FindFile(strdir2);
			if(success2)
			{
				projectWaitPreInfo.projectName = strTitle;
				projectWaitPreInfo.projectInfoPath = strdir2;
				projectWaitPreInfo.projectPath = strPath;

				CExcelOperator 	ExcelOperator;
				ExcelOperator.LoadExcel(strdir2);

				CString ProjectName;
				CString UserName;
				CString RecvDate;
				CString PredictType;

				ExcelOperator.GetItem(2,1,L"String",UserName);
				ExcelOperator.GetItem(10,1,L"Date",RecvDate);
				ExcelOperator.GetItem(9,1,L"String",PredictType);

				projectWaitPreInfo.projectUserName = UserName;             //用户名list
				projectWaitPreInfo.projectRecvDate = RecvDate;                 //
				projectWaitPreInfo.projectPredictType = PredictType;     

				projectWaitePreList.push_back(projectWaitPreInfo);
				ExcelOperator.ReleaseExcel();
			}
		}
	}
	//添加已分析项目
	bWorking = finder.FindFile(strDirFinishedAna);
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
			proFinInfo projectFinished;

			BOOL success2=finderIsProject.FindFile(strdir2);
			if(success2)
			{
				projectFinished.projectFinishedName = strTitle;
				projectFinished.projectFinishedInfoPath = strdir2;
				projectFinished.projectFinishedPath = strPath;

				CExcelOperator 	ExcelOperator;
				ExcelOperator.LoadExcel(strdir2);

				CString ProjectName;
				CString UserName;
				CString RecvDate;
				CString PredictType;
				CString SendBack;

				ExcelOperator.GetItem(2,1,L"String",UserName);
				ExcelOperator.GetItem(10,1,L"Date",RecvDate);
				ExcelOperator.GetItem(9,1,L"String",PredictType);
				ExcelOperator.GetItem(11,1,L"String",SendBack);
				projectFinished.projectFinishedUserName = UserName;             //用户名list
				projectFinished.projectFinishedRecvDate = RecvDate;                 //
				projectFinished.projectFinishedPredictType = PredictType;  

				if(SendBack.IsEmpty())
				{
					SendBack=L"未完成";
				}
				projectFinished.projectFinishedSendBack= SendBack; 
				ExcelOperator.ReleaseExcel();
			}	
			//添加鉴定结果
			CString strdir3=strPath;
			strdir3+="\\鉴定结果.xlsx";

			BOOL success3=finderIsProject.FindFile(strdir3);
			if(success3)
			{
				CExcelOperator 	ExcelOperator;
				ExcelOperator.LoadExcel(strdir3);

				int i=1;
				CString temp;
				ExcelOperator.GetItem(i,1,L"String",temp);

				while(!temp.IsEmpty())
				{
					i++;
					ExcelOperator.GetItem(i,1,L"String",temp);
				}
				CString Preresult;
				ExcelOperator.GetItem(i+1,2,L"String",Preresult);			
				projectFinished.projectFinishedPreResult= Preresult;
				projectFinishedList.push_back(projectFinished);
				ExcelOperator.ReleaseExcel();
			}
		}
	}
	finder.Close();
	UpdateProjectList();
	return 0;
}


int CMainUIDlg::AddOneProject(CString ProjectPath)
{
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
		CFileFind finderIsProject;
		CString strdir2=strPath;
		strdir2+="\\项目信息.xlsx";
		BOOL success2=finderIsProject.FindFile(strdir2);
		if(success2)
		{
			proWaitPreInfo projectWaitPreInfo;

			projectWaitPreInfo.projectName = strTitle;
			projectWaitPreInfo.projectInfoPath = strdir2;
			projectWaitPreInfo.projectPath = strPath;

			CExcelOperator 	ExcelOperator;
			ExcelOperator.LoadExcel(strdir2);

			CString ProjectName;
			CString UserName;
			CString RecvDate;
			CString PredictType;

			ExcelOperator.GetItem(2,1,L"String",UserName);
			ExcelOperator.GetItem(10,1,L"Date",RecvDate);
			ExcelOperator.GetItem(9,1,L"String",PredictType);

			projectWaitPreInfo.projectUserName = UserName;             
			projectWaitPreInfo.projectRecvDate = RecvDate;             
			projectWaitPreInfo.projectPredictType = PredictType;  

			EnterCriticalSection(&g_csListWaitPre);
			projectWaitePreList.push_back(projectWaitPreInfo);
			LeaveCriticalSection(&g_csListWaitPre);
			ExcelOperator.ReleaseExcel();
		}			
		finderIsProject.Close();
	    //更新列表
		EnterCriticalSection(&g_csProListShow);
		UpdateProjectList();
		LeaveCriticalSection(&g_csProListShow);
		return 0;
	}
	return -1;
}

//更新列表显示


afx_msg LRESULT CMainUIDlg::OnUpdatelist(WPARAM wParam, LPARAM lParam)
{
	UpdateProjectList();
	return 0;
}
void CMainUIDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	GetDlgItem(IDC_STATIC1)->MoveWindow(0,0,m_rect.Width()/2 - 100,m_rect.Height()*0.3);
	GetDlgItem(IDC_LIST_ONLINEUSER)->MoveWindow(10,20,m_rect.Width()/2 - 120,m_rect.Height()*0.3-30);
	GetDlgItem(IDC_STATIC2)->MoveWindow(0,m_rect.Height()*0.3+5,m_rect.Width()/2 - 100,m_rect.Height()*0.5);
	GetDlgItem(IDC_LIST_WAITEFORPREDICT)->MoveWindow(10,m_rect.Height()*0.3+25,m_rect.Width()/2 - 120,m_rect.Height()*0.5-30);
	GetDlgItem(IDC_STATIC3)->MoveWindow(0,m_rect.Height()*0.8+10,m_rect.Width()/2 - 100,m_rect.Height()*0.2-10);
	GetDlgItem(IDC_LIST_BEINGPREDICT)->MoveWindow(10,m_rect.Height()*0.8+30,m_rect.Width()/2 - 120,m_rect.Height()*0.2-40);
	GetDlgItem(IDC_STATIC4)->MoveWindow(m_rect.Width()/2 - 90,0,m_rect.Width()/2 + 90,m_rect.Height()*0.4);
	GetDlgItem(IDC_LIST_FINISHEDPREDICT)->MoveWindow(m_rect.Width()/2 - 80,20,m_rect.Width()/2 + 70,m_rect.Height()*0.4-30-30);

	int buttonWidth = (m_rect.Width()/2 + 70-30)/4;
	GetDlgItem(IDC_BUTTON_DELETEFINISHEDPROJECT)->MoveWindow(m_rect.Width()/2 - 80,m_rect.Height()*0.4-40,buttonWidth,30);
	GetDlgItem(IDC_BUTTON_MANUALCOMPARE)->MoveWindow(m_rect.Width()/2 - 80+buttonWidth+10,m_rect.Height()*0.4-40,buttonWidth,30);
	GetDlgItem(IDC_BUTTON_PREDICTEDRECORD)->MoveWindow(m_rect.Width()/2 - 80+2*(buttonWidth+10),m_rect.Height()*0.4-40,buttonWidth,30);

	GetDlgItem(IDC_STATIC5)->MoveWindow(m_rect.Width()/2 - 90,m_rect.Height()*0.4+10,m_rect.Width()/2 + 90,m_rect.Height()*0.6-10);
	GetDlgItem(IDC_LIST_PREDICTRESULTDETAILINFO)->MoveWindow(m_rect.Width()/2 - 90+10,m_rect.Height()*0.4+10+20,m_rect.Width()/2 + 90-20,m_rect.Height()*0.6-10-30);
	// 不为绘图消息调用 CDialogEx::OnPaint()
}
void CMainUIDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	// TODO: 在此处添加消息处理程序代码
	GetClientRect(&m_rect);//将变化后的对话框大小设为旧大小 
}

//单击
void CMainUIDlg::OnNMClickListFinishedpredict(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	m_ListProPreResultDetail.DeleteAllItems();
	if(m_ListFinishedAutoPredict.GetSelectedCount()==0)
	{
		return;
	}
	int nIndex=m_ListFinishedAutoPredict.GetSelectionMark();

	EnterCriticalSection(&g_csListFinishedAutoPredict);
	list<proFinInfo>::iterator itprojectFinishedList = projectFinishedList.begin();
	for(int i=0;i<nIndex;i++)
	{
		++itprojectFinishedList;
	}
	//获取选择的项目鉴定结果路径
	CString nResultPath= itprojectFinishedList->projectFinishedPath;
	CString projectName = itprojectFinishedList->projectFinishedName;
	LeaveCriticalSection(&g_csListFinishedAutoPredict);

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
		m_ListProPreResultDetail.InsertItem(j,projectName);
		m_ListProPreResultDetail.SetItemText(j,1,specFileTitle);
		m_ListProPreResultDetail.SetItemText(j,2,PredictResult);
		j++;
		ExcelOperator.GetItem(j+1,1,L"String",specFileName);
		ExcelOperator.GetItem(j+1,2,L"String",PredictResult);
	}

	ExcelOperator.ReleaseExcel();

	return;
}


void CMainUIDlg::OnBnClickedButtonManualcompare()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_ListFinishedAutoPredict.GetSelectedCount()==0)
	{
		MessageBox(L"请选择一个项目！",L"人工辅助分析",MB_ICONWARNING);
		return;
	}
	int nIndex;
	nIndex=m_ListFinishedAutoPredict.GetSelectionMark();
	EnterCriticalSection(&g_csListFinishedAutoPredict);
	list<proFinInfo>::iterator itproFinInfo = projectFinishedList.begin();
	for(int i=0;i<nIndex;i++)
	{
		++itproFinInfo;
	}
	CSpectrumCompareDlg SpectrumCompareDlg;
	SpectrumCompareDlg.m_projectName=itproFinInfo->projectFinishedName;
	SpectrumCompareDlg.m_projectPath=itproFinInfo->projectFinishedPath;
	LeaveCriticalSection(&g_csListFinishedAutoPredict);
	SpectrumCompareDlg.DoModal();
}

UINT ThreadCheckSendCondition(LPVOID lpParam)
{
	CMainUIDlg *pCMainUIDlg=(CMainUIDlg*)lpParam;

	while(!ExitAllThread)
	{
		EnterCriticalSection(&g_csListFinishedAutoPredict);//进入临界区
		//查找是否存在待发送项目
		list<proFinInfo>::iterator itproFinInfo = pCMainUIDlg->projectFinishedList.begin();
		while(itproFinInfo!=pCMainUIDlg->projectFinishedList.end()
			&&(itproFinInfo->projectFinishedSendBack)!=L"未完成"
			&&(itproFinInfo->projectFinishedSendBack)!=L"用户接收失败")
		{
			++itproFinInfo;
		}
		//不存在待发送项目
		if(itproFinInfo==pCMainUIDlg->projectFinishedList.end())
		{
			LeaveCriticalSection(&g_csListFinishedAutoPredict);
			Sleep(1000);
			continue;
		}
		//存在待发送项目,修改参数,更新显示,加入待发送队列
		itproFinInfo->projectFinishedSendBack = L"准备发送";
		CString FinishedPathList = itproFinInfo->projectFinishedPath;
		CString FinishedName = itproFinInfo->projectFinishedName;
		CString UserName = itproFinInfo->projectFinishedUserName;
		LeaveCriticalSection(&g_csListFinishedAutoPredict); //离开临界区

		//获取打包地址
		CString PackDir=g_CurrentDir;
		PackDir+=L"\\Project\\zipFile\\";	
		PackDir+=FinishedName;
		PackDir+=L".zip";
		//若存在同名文件，则删除
		if(PathFileExists(PackDir))
		{
			DeleteFile(PackDir);
		}
		//打包
		CZipImplement ZipImplement;
		ZipImplement.Zip_PackFiles(FinishedPathList,PackDir);
		SendInfo sendinfo;
		sendinfo.PackDir=PackDir;
		sendinfo.FileName=FinishedName;
		sendinfo.FileName+=L".zip";
		sendinfo.UserName=UserName;

		EnterCriticalSection(&g_cswaiteForSendList);
		g_waiteForSendList.push_back(sendinfo);
		LeaveCriticalSection(&g_cswaiteForSendList);

	}
	return 0;
}
afx_msg LRESULT CMainUIDlg::OnSendresultcondition(WPARAM wParam, LPARAM lParam)
{
	int nFlag=wParam;
	char *p=(char*)lParam;
	CString projectName=Char2CString(p);
	delete[] p;
	p=NULL;
	switch (nFlag)
	{
	case 1:
		{
			EnterCriticalSection(&g_csListFinishedAutoPredict);

			LVFINDINFO info;
			int nIndex;
			info.flags = LVFI_PARTIAL|LVFI_STRING;
			info.psz = projectName;
			if ((nIndex = m_ListFinishedAutoPredict.FindItem(&info)) != -1)
			{
				if(m_ListFinishedAutoPredict.GetItemText(nIndex,5)!=L"用户处于离线状态")
					m_ListFinishedAutoPredict.SetItemText(nIndex,5,L"用户处于离线状态");
			}
			LeaveCriticalSection(&g_csListFinishedAutoPredict);
			break;
		}
	case 2:
		{

			EnterCriticalSection(&g_csListFinishedAutoPredict);

			LVFINDINFO info;
			int nIndex;
			info.flags = LVFI_PARTIAL|LVFI_STRING;
			info.psz = projectName;
			if ((nIndex = m_ListFinishedAutoPredict.FindItem(&info)) != -1)
			{
				if(m_ListFinishedAutoPredict.GetItemText(nIndex,5)!=L"用户接收失败")
					m_ListFinishedAutoPredict.SetItemText(nIndex,5,L"用户接收失败");
			}
			LeaveCriticalSection(&g_csListFinishedAutoPredict);
			break;
		}

	case 3:
		{
			EnterCriticalSection(&g_csListFinishedAutoPredict);
			LVFINDINFO info;
			int nIndex;
			info.flags = LVFI_PARTIAL|LVFI_STRING;
			info.psz = projectName;
			if ((nIndex = m_ListFinishedAutoPredict.FindItem(&info)) != -1)
			{
				if(m_ListFinishedAutoPredict.GetItemText(nIndex,5)!=L"发送中")
					m_ListFinishedAutoPredict.SetItemText(nIndex,5,L"发送中");
			}
			LeaveCriticalSection(&g_csListFinishedAutoPredict);
			break;
		}
	case 4:
		{
			EnterCriticalSection(&g_csListFinishedAutoPredict);
			LVFINDINFO info;
			int nIndex;
			info.flags = LVFI_PARTIAL|LVFI_STRING;
			info.psz = projectName;
			if ((nIndex = m_ListFinishedAutoPredict.FindItem(&info)) != -1)
			{
				if(m_ListFinishedAutoPredict.GetItemText(nIndex,5)!=L"用户尚未准备完全")
					m_ListFinishedAutoPredict.SetItemText(nIndex,5,L"用户尚未准备完全");
			}
			LeaveCriticalSection(&g_csListFinishedAutoPredict);
			break;
		}
	case 5:
		{
			EnterCriticalSection(&g_csListFinishedAutoPredict);
			LVFINDINFO info;
			int nIndex;
			info.flags = LVFI_PARTIAL|LVFI_STRING;
			info.psz = projectName;
			if ((nIndex = m_ListFinishedAutoPredict.FindItem(&info)) != -1)
			{
				if(m_ListFinishedAutoPredict.GetItemText(nIndex,5)!=L"即将发送")
					m_ListFinishedAutoPredict.SetItemText(nIndex,5,L"即将发送");
			}
			LeaveCriticalSection(&g_csListFinishedAutoPredict);
			break;
		}
	case 6:
		{
			EnterCriticalSection(&g_csListFinishedAutoPredict);
			LVFINDINFO info;
			int nIndex;
			info.flags = LVFI_PARTIAL|LVFI_STRING;
			info.psz = projectName;
			if ((nIndex = m_ListFinishedAutoPredict.FindItem(&info)) != -1)
			{
				if(m_ListFinishedAutoPredict.GetItemText(nIndex,5)!=L"等待发送")
					m_ListFinishedAutoPredict.SetItemText(nIndex,5,L"等待发送");
			}
			LeaveCriticalSection(&g_csListFinishedAutoPredict);
			break;
		}
	case 7:
		{
			EnterCriticalSection(&g_csListFinishedAutoPredict);
			LVFINDINFO info;
			int nIndex;
			info.flags = LVFI_PARTIAL|LVFI_STRING;
			info.psz = projectName;
			if ((nIndex = m_ListFinishedAutoPredict.FindItem(&info)) != -1)
			{
				if(m_ListFinishedAutoPredict.GetItemText(nIndex,5)!=L"已完成")
					m_ListFinishedAutoPredict.SetItemText(nIndex,5,L"已完成");
			}
			LeaveCriticalSection(&g_csListFinishedAutoPredict);
			break;
		}
	}
	return 0;
}
UINT ThreadSendResult(LPVOID lpParam)
{
	AfxOleInit();
	CMainUIDlg *pCMainUIDlg=(CMainUIDlg*)lpParam;
	ThreadSendResultExited=0;
	while(!ExitAllThread)
	{
		if(g_waiteForSendList.empty())
		{
			Sleep(100);
			continue;
		}
		EnterCriticalSection(&g_cswaiteForSendList);
		SendInfo *pSendInfo = new SendInfo(*g_waiteForSendList.begin());
		g_waiteForSendList.pop_front();
		LeaveCriticalSection(&g_cswaiteForSendList);
		//检查用户是否在线
		EnterCriticalSection(&g_csConns);
		ConnectionList::iterator it = g_conns.begin();
		while(it != g_conns.end())
		{
			if((*it)->UserName == pSendInfo->UserName)
				break;
			else
				++it;
		}
		LeaveCriticalSection(&g_csConns);
		//用户不在线
		if(it == g_conns.end())
		{
			//将该项目置于队尾
			EnterCriticalSection(&g_cswaiteForSendList);
			g_waiteForSendList.push_back(*pSendInfo);
			LeaveCriticalSection(&g_cswaiteForSendList);
			//发送消息,提示用户不在线
			char *pprojectName=CString2Char(pSendInfo->FileName.Left(pSendInfo->FileName.Find(L".")));
			::SendMessage(pCMainUIDlg->m_hWnd,WM_SENDRESULTCONDITION,1,(LPARAM)pprojectName);
			delete pSendInfo;
			pSendInfo = NULL;		
			Sleep(100);
			continue;
		}
		//用户在线，准备发送
		pSendInfo->User_IP = (*it)->UserIP;
		pSendInfo->User_Port = (*it)->Port;
		char *s_ip = CString2Char(pSendInfo->User_IP);
		//准备发送
		::SendMessage(pCMainUIDlg->m_hWnd,WM_SENDRESULTCONDITION,5,(LPARAM)CString2Char(pSendInfo->FileName.Left(pSendInfo->FileName.Find(L"."))));	

		sockaddr_in serveraddr;
		serveraddr.sin_family=AF_INET;  //协议
		serveraddr.sin_port=htons(pSendInfo->User_Port);//服务器进程端口
		serveraddr.sin_addr.s_addr=inet_addr(s_ip);//服务器进程的IP
		if(s_ip)
		{
			free(s_ip);
			s_ip = NULL;
		}
		//创建临时发送套接字
		SOCKET m_sSend=socket(AF_INET,SOCK_STREAM,0);

		int retval=connect(m_sSend,(sockaddr*)&serveraddr,sizeof(serveraddr));
		if(retval == SOCKET_ERROR)
		{
			//连接失败
			//将该项目置于队尾
			closesocket(m_sSend);
			m_sSend = INVALID_SOCKET;
			EnterCriticalSection(&g_cswaiteForSendList);
			g_waiteForSendList.push_back(*pSendInfo);
			LeaveCriticalSection(&g_cswaiteForSendList);
			//发送消息,提示用户不在线
			char *pprojectName=CString2Char(pSendInfo->FileName.Left(pSendInfo->FileName.Find(L".")));
			::SendMessage(pCMainUIDlg->m_hWnd,WM_SENDRESULTCONDITION,4,(LPARAM)pprojectName);
			delete pSendInfo;
			pSendInfo = NULL;		
			Sleep(100);
			continue;
		}
		//如果连接成功，开始传输数据
		//设为非阻塞套接字
		int mode=1;
		if(SOCKET_ERROR==ioctlsocket(m_sSend,FIONBIO,(u_long FAR*) &mode))
		{
			//将该项目置于队尾
			closesocket(m_sSend);
			m_sSend = INVALID_SOCKET;
			EnterCriticalSection(&g_cswaiteForSendList);
			g_waiteForSendList.push_back(*pSendInfo);
			LeaveCriticalSection(&g_cswaiteForSendList);
			//发送消息,提示用户不在线
			char *pprojectName=CString2Char(pSendInfo->FileName.Left(pSendInfo->FileName.Find(L".")));
			::SendMessage(pCMainUIDlg->m_hWnd,WM_SENDRESULTCONDITION,4,(LPARAM)pprojectName);
			delete pSendInfo;
			pSendInfo = NULL;		
			Sleep(100);
			continue;
		}
		if(m_sSend!=INVALID_SOCKET)
		{
			pSendInfo->m_sSend = m_sSend;
			if(g_SendingThreadCount<4)
			    AfxBeginThread(ThreadSend,(LPVOID)pSendInfo);
			else
			{
				EnterCriticalSection(&g_cswaiteForSendList);
				g_waiteForSendList.push_back(*pSendInfo);
				LeaveCriticalSection(&g_cswaiteForSendList);
				//发送消息,提示用户不在线
				char *pprojectName=CString2Char(pSendInfo->FileName.Left(pSendInfo->FileName.Find(L".")));
				::SendMessage(pCMainUIDlg->m_hWnd,WM_SENDRESULTCONDITION,4,(LPARAM)pprojectName);
				delete pSendInfo;
				pSendInfo = NULL;		
				Sleep(100);
			}

		}
	}
	ThreadSendResultExited=1;
	return 0;
}

UINT ThreadSend(LPVOID pContext)
{
	AfxOleInit();
	++g_SendingThreadCount;	
	int SendState = 3;
	CMainUIDlg *pCMainUIDlg = (CMainUIDlg *)m_pMainUIDlg;
	SendInfo sendinfo((*(SendInfo *)pContext));	
	SOCKET m_sSend = sendinfo.m_sSend;
	delete pContext;
	pContext = NULL;
	int count = 0;
	ifstream infile;	
	long port=sendinfo.User_Port;
	CString cs_ip=sendinfo.User_IP;
	char *s_ip=CString2Char(cs_ip);
	char *pprojectName = NULL;
	//开始发送
	//发送消息，发送中
	pprojectName=CString2Char(sendinfo.FileName.Left(sendinfo.FileName.Find(L".")));
	::SendMessage(pCMainUIDlg->m_hWnd,WM_SENDRESULTCONDITION,SendState,(LPARAM)pprojectName);
	char buf[DATA_BUFSIZE];	
	//打开文件	
	infile.open(sendinfo.PackDir,ios::in|ios::binary);
	infile.seekg(0,ios::end);
	std::streamoff filelen=infile.tellg();
	infile.seekg(0,ios::beg);

	memset(buf,0,DATA_BUFSIZE);
	sprintf(buf,"%ld",filelen);
	while(true)
	{
		if(ExitAllThread)
		{
			goto Release;
		}
		int retVal=send(m_sSend,buf,DATA_BUFSIZE,NULL);
		if(SOCKET_ERROR==retVal)
		{
			int err=WSAGetLastError();
			if(err==WSAEWOULDBLOCK&&count<=300)
			{
				Sleep(100);
				++count;
				continue;
			}
			else
			{
				//发送失败处理
				EnterCriticalSection(&g_cswaiteForSendList);
				g_waiteForSendList.push_back(sendinfo);
				LeaveCriticalSection(&g_cswaiteForSendList);
				pprojectName=CString2Char(sendinfo.FileName.Left(sendinfo.FileName.Find(L".")));
				SendState = 2;
				goto Release;
			}
		}
		break;
	}
	Sleep(200);
	//发送文件名
	char *ProName=CString2Char(sendinfo.FileName);
	memset(buf,0,DATA_BUFSIZE);
	memcpy(buf,ProName,strlen(ProName));
	while(true)
	{
		if(ExitAllThread)
		{
			goto Release;
		}
		int retVal=send(m_sSend,buf,DATA_BUFSIZE,0);
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
				free(ProName);
				ProName =NULL;
				EnterCriticalSection(&g_cswaiteForSendList);
				g_waiteForSendList.push_back(sendinfo);
				LeaveCriticalSection(&g_cswaiteForSendList);
				pprojectName=CString2Char(sendinfo.FileName.Left(sendinfo.FileName.Find(L".")));
				SendState = 2;
				goto Release;			
			}
		}
		break;
	}
	free(ProName);
	ProName = NULL;
	Sleep(200);
	while(!infile.eof())
	{
		if(ExitAllThread)
		{
			goto Release;
		}
		//发送
		memset(buf,0,DATA_BUFSIZE);
		infile.read(buf,DATA_BUFSIZE);
		std::streamsize len=infile.gcount();
		count = 0;
		while(true)
		{
			if(ExitAllThread)
			{
				goto Release;
			}
			int retVal=send(m_sSend,buf,len,0);
			if(SOCKET_ERROR==retVal)
			{
				int err=WSAGetLastError();
				if(err==WSAEWOULDBLOCK&&count<300)
				{
					Sleep(100);
					++count;
					continue;
				}
				else
				{
					EnterCriticalSection(&g_cswaiteForSendList);
					g_waiteForSendList.push_back(sendinfo);
					LeaveCriticalSection(&g_cswaiteForSendList);
					pprojectName=CString2Char(sendinfo.FileName.Left(sendinfo.FileName.Find(L".")));
					SendState = 2;
					goto Release;
				}
			}
			break;
		}
		Sleep(200);
	}
	//发送完毕
	infile.close();
	infile.clear();
	//接收确认信息
	while(true)
	{
		if(ExitAllThread)
		{
			goto Release;
		}
		ZeroMemory(buf,DATA_BUFSIZE);
		int retVal=recv(m_sSend,buf,DATA_BUFSIZE,0);
		if(SOCKET_ERROR==retVal)
		{
			int err=WSAGetLastError();
			if(err==WSAEWOULDBLOCK)
			{
				Sleep(100);
				continue;
			}
			else if(err == WSAETIMEDOUT || err == WSAENETDOWN)
			{
				//发送失败处理
				EnterCriticalSection(&g_cswaiteForSendList);
				g_waiteForSendList.push_back(sendinfo);
				LeaveCriticalSection(&g_cswaiteForSendList);
				pprojectName=CString2Char(sendinfo.FileName.Left(sendinfo.FileName.Find(L".")));
				SendState = 2;
				goto Release;
			}
		}
		break;
	}
	if(strcmp(buf,"success")==0)
	{
		pCMainUIDlg->DoFinishedSend(sendinfo);
		if(infile.is_open())
		{
			infile.close();
			infile.clear();
		}
		//删除zip文件
		DeleteFile(sendinfo.PackDir);
		SendState = 7;
		pprojectName=CString2Char(sendinfo.FileName.Left(sendinfo.FileName.Find(L".")));
	}
	else
	{
		//发送失败，执行相应操作
		EnterCriticalSection(&g_cswaiteForSendList);
		g_waiteForSendList.push_back(sendinfo);
		LeaveCriticalSection(&g_cswaiteForSendList);
		pprojectName=CString2Char(sendinfo.FileName.Left(sendinfo.FileName.Find(L".")));
		SendState = 2;
		goto Release;
	}
//资源释放
Release:
	closesocket(m_sSend);
	m_sSend = INVALID_SOCKET;
	if(infile.is_open())
	{
		infile.close();
		infile.clear();
	}
	--g_SendingThreadCount; //更新发送线程计数
	::SendMessage(pCMainUIDlg->m_hWnd,WM_SENDRESULTCONDITION,SendState,(LPARAM)pprojectName);
	return 0;
}

int CMainUIDlg::DoFinishedSend(SendInfo &sendinfo)
{
	CString Filetitle=sendinfo.FileName.Left(sendinfo.FileName.Find(L"."));
	CString proInfoDir=g_CurrentDir;
	proInfoDir+=L"\\Project\\AnalasisFinished\\";
	proInfoDir+=Filetitle;
	proInfoDir+=L"\\项目信息.xlsx";
	CExcelOperator ExcelOperator;
	if(ExcelOperator.LoadExcel(proInfoDir)==0)
	{
		MessageBox(L"打开信息列表失败",L"打开文件",MB_ICONWARNING);
		return -1;
	}
	ExcelOperator.SetItem(11,1,L"已完成");
	//获取时间
	SYSTEMTIME sys; 
	GetLocalTime(&sys); 
	CString szDate;
	szDate.Format(L"%d-%d-%d",sys.wYear,sys.wMonth,sys.wDay);//.GetMonth(),cTime.GetDay());
	ExcelOperator.SetItem(12,1,szDate);
	ExcelOperator.Save();
	ExcelOperator.ReleaseExcel();
	return 0;
}
void CMainUIDlg::OnBnClickedButtonCeshi()
{
	// TODO: 在此添加控件通知处理程序代码
	CPCASettingDlg PCASettingDlg;
	PCASettingDlg.DoModal();
}
//销毁窗口前结束线程
void CMainUIDlg::OnNcDestroy()
{
	CDialogEx::OnNcDestroy();
	ExitAllThread=1;
	//关闭监听
	closesocket(g_hListenSocket);
	g_hListenSocket = INVALID_SOCKET;
	while(!(ThreadThreadDearWithClientConnectExited&ThreadAutoPreProjectExited&ThreadSendResultExited&(!g_SendingThreadCount)))
	{
		Sleep(10);
	}
	DeleteCriticalSection(&g_csConns);
	DeleteCriticalSection(&g_csListFinishedAutoPredict);
	DeleteCriticalSection(&g_csListWaitPre);
	DeleteCriticalSection(&g_csListBeingPre);
	DeleteCriticalSection(&g_cswaiteForSendList);
	DeleteCriticalSection(&g_csProListShow);
	WSACleanup();
	// TODO: 在此处添加消息处理程序代码
}
afx_msg LRESULT CMainUIDlg::OnContinuepredictproject(WPARAM wParam, LPARAM lParam)
{
	PauseThreadAutoPreProject = false;
	while(ThreadAutoPreProjectPaused)Sleep(10);
	return 0;
}
afx_msg LRESULT CMainUIDlg::OnPausepredictproject(WPARAM wParam, LPARAM lParam)
{
	PauseThreadAutoPreProject = true;
	while(!ThreadAutoPreProjectPaused)Sleep(10);
	return 0;
}
void CMainUIDlg::OnBnClickedButtonPredictedrecord()
{
	// TODO: 在此添加控件通知处理程序代码
	CPredictedRecordDlg PredictedRecordDlg;
	PredictedRecordDlg.DoModal();
}
afx_msg LRESULT CMainUIDlg::OnAddoneproject(WPARAM wParam, LPARAM lParam)
{
	char *pProjectPath = (char*)lParam;
	CString ProjectPath = Char2CString(pProjectPath);
	free(pProjectPath);
	this->AddOneProject(ProjectPath);	
	return 0;
}
//更新在线用户列表
void CMainUIDlg::UpdateOlUserList(void)
{
	EnterCriticalSection(&g_csConns);
	m_ListOnlineUsers.DeleteAllItems();
	ConnectionList::iterator it = g_conns.begin();
	while(it!=g_conns.end())
	{
		m_ListOnlineUsers.InsertItem(m_ListOnlineUsers.GetItemCount(),(*it)->UserName);
		m_ListOnlineUsers.SetItemText(m_ListOnlineUsers.GetItemCount(),1,(*it)->UserIP);
		++it;
	}
	LeaveCriticalSection(&g_csConns);
}

