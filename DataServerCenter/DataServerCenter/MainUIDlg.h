#pragma once


// CMainUIDlg 对话框
#include "Matrix.h"
#include "UserManage.h"
#include <algorithm>
#include <map>
#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")
#include "DataServerCenterDoc.h"
#include "DataServerCenterView.h"
#include "WineTypeIndex.h"
#include "CListCtrlEx.h"
#include "PredictedRecordDlg.h"
#include <iostream>
#include <fstream>
#include "afxcmn.h"
#include <openssl\aes.h>
#include <openssl\rsa.h>
#include <openssl\bn.h>
#pragma comment(lib,"libeay32.lib")
#pragma comment(lib,"ssleay32.lib")
#include "sqlite3.h"
#pragma comment(lib,"sqlite3.lib")
#include <vector>
#include "afxwin.h"
using std::map;
using std::ofstream;
using std::ifstream;
using std::ios;
using std::find;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define PORT 8000
#define DATA_BUFSIZE 12288

#define  RECV_POSTED 0
#define  SEND_POSTED 1

#define WM_USERLOUGOOUT WM_USER+100
#define WM_UPDATELIST WM_USER+106
#define WM_SENDRESULTCONDITION WM_USER+108

#define WM_CONTINUEPREDICTPROJECT WM_USER+112
#define WM_PAUSEPREDICTPROJECT WM_USER+113

#define WM_ADDONEPROJECT WM_USER+114
#define WM_SENDMAILTOUSER WM_USER+115

#define MAX_THREAD_NUM 32
#define EXIT_THREAD 2012
#define RSA_KEY_LEN 1024
typedef struct
{
	SOCKET Socket;
	CWnd *pWnd;
	CString UserName;
}SockStruct;

struct SendInfo
{
	long User_Port;
	CString  User_IP;
	CString UserName;
	CString PackDir;
	CString FileName;
	CString ResultFilePath;
	CString ComponnentConcentrationFilePath;
	CString MailAddr;
	SOCKET m_sSend;

	SendInfo(const SendInfo &sendInfo):
	    User_Port(sendInfo.User_Port),
		User_IP(sendInfo.User_IP),
		UserName(sendInfo.UserName),
		PackDir(sendInfo.PackDir),
		FileName(sendInfo.FileName),
		ResultFilePath(sendInfo.ResultFilePath),
		ComponnentConcentrationFilePath(sendInfo.ComponnentConcentrationFilePath),
		MailAddr(sendInfo.MailAddr),
		m_sSend(sendInfo.m_sSend)
		{
		}
	SendInfo():
		User_Port(-1),
		User_IP(L""),
		UserName(L""),
		PackDir(L""),
		FileName(L""),
		ResultFilePath(L""),
		ComponnentConcentrationFilePath(L""),
		MailAddr(L"")
		{
			m_sSend = INVALID_SOCKET;
		}
 ~SendInfo()
 {
	 if(m_sSend!=INVALID_SOCKET)
		 closesocket(m_sSend);
 }
};
//管理用户连接线程
UINT ThreadDearWithClientConnect(LPVOID lpParam);
//管理用户线程
//UINT AnswerThread(LPVOID lparam);
//检测发送情况线程
UINT ThreadCheckSendCondition(LPVOID lpParam);
//自动溯源线程
UINT ThreadAutoPreProject(LPVOID lpParam);
VOID NTAPI PredictCallback(PTP_CALLBACK_INSTANCE pInstance,PVOID pvContext);//溯源异步函数
//发送检测结果线程
UINT ThreadSendResult(LPVOID lpParam);
UINT ThreadSend(LPVOID pContext);
//完成线程
DWORD WINAPI WorkThread(LPVOID param);
typedef enum _IO_OPERATION
{
	IORSA,         //发送rsa公钥
	IOUserInfo,    //接收用户信息
	IORecvType,    //接收类型（测试连接和接收文件）
	IORecvFileName,//接收文件名
	IOFileSize,    //接收文件大小
	IORecvFile,    //接收项目文件
	IOStatus,      //发送服务状态
}IO_OPERATION;

typedef unsigned int (_stdcall *PTHREAD_START) (void*);

struct IOContext:WSAOVERLAPPED
{
	static CWnd *pWnd;          //保存窗口指针
	SOCKET hSocket;
	int Port; //端口号
	char Buffer[DATA_BUFSIZE];
	int nBytesInBuf;     //缓冲区的字节数
	int nBytesFileReceived;  //已接收的字节数
	int nBytesFileToRecv;    //期望接收的字节数
	CString FileName;      //接收的文件名称
	char FilePath[MAX_PATH];
	ofstream oFile;         //文件句柄
	CString UserName;    //用户名
	CString UserIP;      //用户IP
	//利用客户端发来的随机数产生的加解密密钥
	char keyEncode[255];  //加密密钥
	char keyDecode[255];  //解密密钥
	RSA *rsa_key;

	WSABUF wsaBuffer;   
	IO_OPERATION op;    //操作类型
	IOContext(SOCKET socket):hSocket(socket),nBytesInBuf(0),nBytesFileReceived(0),nBytesFileToRecv(0),UserName(L""),rsa_key(NULL)
	{
		memcpy(FilePath,"FileRecv\\",sizeof("FileRecv\\"));
		//利用RSA算法生成1024位的密钥对
		rsa_key = RSA_new();
		rsa_key = RSA_generate_key(RSA_KEY_LEN,0x10001,NULL,NULL);

		ZeroMemory(Buffer,DATA_BUFSIZE);
		int len = strlen(BN_bn2dec(rsa_key->n));
		memcpy(Buffer,BN_bn2dec(rsa_key->n),strlen(BN_bn2dec(rsa_key->n)));// 保存公钥

		wsaBuffer.buf = Buffer;
		wsaBuffer.len = DATA_BUFSIZE;
		op = IORSA;  //初始化发送RSA公钥
		ZeroMemory(this,sizeof(WSAOVERLAPPED));
	}
	~IOContext()
	{
		if(rsa_key)
		{
			RSA_free(rsa_key);
			rsa_key = NULL;
		}
	}
};
typedef vector<IOContext *> ConnectionList;

extern ConnectionList g_conns;

struct PreProInfo
{
	CString projectNameBeingPre;
	CString projectInfoPathBeingPre;
	CString projectPathBeingPre;
	CString projectUserNameBeingPre; 
	CString projectRecvDateBeingPre; 
	CString projectPredictTypeBeingPre;
	CString UserMailAddr;
};
struct proFinInfo
{
	CString  projectFinishedName;       //已完成项目名list
	CString  projectFinishedPath;       //已完成项目路径list
	CString  projectFinishedInfoPath;   //已完成项目信息list
	CString  projectFinishedUserName;   //用户名
	CString  projectFinishedRecvDate;   //
	CString  projectFinishedPredictType;
	CString  projectFinishedPreResult;
	CString  projectFinishedSendBack;     //已完成项目回送情况
	CString  UserMailAddr;
};

struct proWaitPreInfo
{
	CString projectName;     //待溯源项目名list
	CString projectInfoPath; //待溯源项目信息list
	CString projectPath;     //待溯源项目路径list
	CString projectUserName; //用户名list
	CString projectRecvDate; //
	CString projectPredictType;
	CString UserMailAddr;
};
class CMainUIDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMainUIDlg)
public:
	CMainUIDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMainUIDlg();
// 对话框数据
	enum { IDD = IDD_DIALOG_MAINUI };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	//溯源中项目
	std::list<PreProInfo> projectBeingPreList;
	//待溯源项目
	std::list<proWaitPreInfo>  projectWaitePreList;
	//已完成项目
	std::list<proFinInfo>  projectFinishedList;
	CArray<CString,CString> m_specName;
	CWinThread *pThreadCheckSendCondition; 
	CWinThread *pThreadSendResult;
	CWinThread *pThreadAutoPreProject;

	CWnd *pDlg;
protected:
	afx_msg LRESULT OnUserlougoout(WPARAM wParam, LPARAM lParam);
public:
	CListCtrlEx m_ListOnlineUsers;
public:
	CListCtrlEx m_ListProjectWaitForPredict; //
	CListCtrlEx m_ListProjectBeingPredict;
	CListCtrlEx m_ListFinishedAutoPredict;
	CListCtrlEx m_ListProPreResultDetail;

	int AddAllProject();
	int AddOneProject(CString ProjectPath);
	int UpdateProjectList(void);
	
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CRect m_rect;

	afx_msg void OnNMClickListFinishedpredict(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonManualcompare();
	afx_msg void OnPaint();
protected:
	afx_msg LRESULT OnUpdatelist(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnBnClickedButtonCeshi();
protected:
	afx_msg LRESULT OnSendresultcondition(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnNcDestroy();
	int DoFinishedSend(SendInfo &sendinfo);
protected:
	afx_msg LRESULT OnContinuepredictproject(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPausepredictproject(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnBnClickedButtonPredictedrecord();
protected:
	afx_msg LRESULT OnAddoneproject(WPARAM wParam, LPARAM lParam);
public:
	void UpdateOlUserList(void);
    void UpdateFinishedDB(void);
	afx_msg void OnBnClickedButtonDeletefinishedproject();
protected:
	afx_msg LRESULT OnSendmailtouser(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnNMDblclkListFinishedpredict(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClickListFinishedpredict(NMHDR *pNMHDR, LRESULT *pResult);
	void SetListctrl();
	CButton m_btnComponentAna;
	afx_msg void OnBnClickedButtonComponentana();
};
