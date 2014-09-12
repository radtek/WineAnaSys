#pragma once


// CMainUIDlg �Ի���
#include "Matrix.h"
#include "UserManage.h"
#include <algorithm>
#include <map>
#include <WinSock2.h>
#pragma coment(lib,"ws2_32.lib")
#include "DataServerCenterDoc.h"
#include "DataServerCenterView.h"
#include "WineTypeIndex.h"
#include "PredictedRecordDlg.h"
#include <iostream>
#include <fstream>
#include "afxcmn.h"
#include <openssl\aes.h>
#include <openssl\rsa.h>
#include <openssl\bn.h>
#pragma comment(lib,"libeay32.lib")
#pragma comment(lib,"ssleay32.lib")

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
	SOCKET m_sSend;

	SendInfo(const SendInfo &sendInfo):
	    User_Port(sendInfo.User_Port),
		User_IP(sendInfo.User_IP),
		UserName(sendInfo.UserName),
		PackDir(sendInfo.PackDir),
		FileName(sendInfo.FileName),
		m_sSend(sendInfo.m_sSend)
		{
		}
	SendInfo():
		User_Port(-1),
		User_IP(L""),
		UserName(L""),
		PackDir(L""),
		FileName(L"")
		{
			m_sSend = INVALID_SOCKET;
		}
 ~SendInfo()
 {
	 if(m_sSend!=INVALID_SOCKET)
		 closesocket(m_sSend);
 }
};
//�����û������߳�
UINT ThreadDearWithClientConnect(LPVOID lpParam);
//�����û��߳�
//UINT AnswerThread(LPVOID lparam);
//��ⷢ������߳�
UINT ThreadCheckSendCondition(LPVOID lpParam);
//�Զ������߳�
UINT ThreadAutoPreProject(LPVOID lpParam);
VOID NTAPI PredictCallback(PTP_CALLBACK_INSTANCE pInstance,PVOID pvContext);//�����첽����
//���ͼ�����߳�
UINT ThreadSendResult(LPVOID lpParam);
UINT ThreadSend(LPVOID pContext);
//����߳�
DWORD WINAPI WorkThread(LPVOID param);
typedef enum _IO_OPERATION
{
	IORSA,         //����rsa��Կ
	IOUserInfo,    //�����û���Ϣ
	IORecvType,    //�������ͣ��������Ӻͽ����ļ���
	IORecvFileName,//�����ļ���
	IOFileSize,    //�����ļ���С
	IORecvFile,    //������Ŀ�ļ�
	IOStatus,      //���ͷ���״̬
}IO_OPERATION;

typedef unsigned int (_stdcall *PTHREAD_START) (void*);

struct IOContext:WSAOVERLAPPED
{
	static CWnd *pWnd;          //���洰��ָ��
	SOCKET hSocket;
	int Port; //�˿ں�
	char Buffer[DATA_BUFSIZE];
	int nBytesInBuf;     //���������ֽ���
	int nBytesFileReceived;  //�ѽ��յ��ֽ���
	int nBytesFileToRecv;    //�������յ��ֽ���
	CString FileName;      //���յ��ļ�����
	char FilePath[MAX_PATH];
	ofstream oFile;         //�ļ����
	CString UserName;    //�û���
	CString UserIP;      //�û�IP
	//���ÿͻ��˷���������������ļӽ�����Կ
	char keyEncode[255];  //������Կ
	char keyDecode[255];  //������Կ
	RSA *rsa_key;

	WSABUF wsaBuffer;   
	IO_OPERATION op;    //��������
	IOContext(SOCKET socket):hSocket(socket),nBytesInBuf(0),nBytesFileReceived(0),nBytesFileToRecv(0),UserName(L""),rsa_key(NULL)
	{
		memcpy(FilePath,"FileRecv\\",sizeof("FileRecv\\"));
		//����RSA�㷨����1024λ����Կ��
		rsa_key = RSA_new();
		rsa_key = RSA_generate_key(RSA_KEY_LEN,0x10001,NULL,NULL);

		ZeroMemory(Buffer,DATA_BUFSIZE);
		int len = strlen(BN_bn2dec(rsa_key->n));
		memcpy(Buffer,BN_bn2dec(rsa_key->n),strlen(BN_bn2dec(rsa_key->n)));// ���湫Կ

		wsaBuffer.buf = Buffer;
		wsaBuffer.len = DATA_BUFSIZE;
		op = IORSA;  //��ʼ������RSA��Կ
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
};
struct proFinInfo
{
	CString  projectFinishedName;       //�������Ŀ��list
	CString  projectFinishedInfoPath;   //�������Ŀ��Ϣlist
	CString  projectFinishedPath;       //�������Ŀ·��list
	CString  projectFinishedUserName;   //�û���list
	CString  projectFinishedRecvDate;   //
	CString  projectFinishedPredictType;
	CString  projectFinishedPreResult;
	CString  projectFinishedSendBack;     //�������Ŀ�������
};

struct proWaitPreInfo
{
	CString  projectName;     //��������Ŀ��list
	CString  projectInfoPath; //��������Ŀ��Ϣlist
	CString  projectPath;     //��������Ŀ·��list
	CString  projectUserName; //�û���list
	CString  projectRecvDate; //
	CString  projectPredictType;
};
class CMainUIDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMainUIDlg)
public:
	CMainUIDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMainUIDlg();
// �Ի�������
	enum { IDD = IDD_DIALOG_MAINUI };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	//��������Ŀ
	list<PreProInfo> projectBeingPreList;
	//��������Ŀ
	list<proWaitPreInfo>  projectWaitePreList;
	//�������Ŀ
	list<proFinInfo>  projectFinishedList;
	CArray<CString,CString> m_specName;
	CWinThread *pThreadCheckSendCondition; 
	CWinThread *pThreadSendResult;
	CWinThread *pThreadAutoPreProject;

	CWnd *pDlg;
protected:
	afx_msg LRESULT OnUserlougoout(WPARAM wParam, LPARAM lParam);
public:
	CListCtrl m_ListOnlineUsers;
public:
	CListCtrl m_ListProjectWaitForPredict; //
	CListCtrl m_ListProjectBeingPredict;
	CListCtrl m_ListFinishedAutoPredict;
	CListCtrl m_ListProPreResultDetail;

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
//	afx_msg void OnButtonUsermanage();
};
