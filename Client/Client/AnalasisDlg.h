#pragma once
#include "afxcmn.h"
#include "SpectrumCompareDlg.h"
#include "ClientView.h"
#include "ProjectSendProgressDlg.h"
#include <fstream>
using std::ofstream;
#include "CListCtrlEx.h"
#pragma comment(lib,"libeay32.lib")
#pragma comment(lib,"ssleay32.lib")
#include <openssl\rsa.h>
#include <openssl\aes.h>
#include "sqlite3.h"
#include "btnst.h"
// CAnalasisDlg 对话框


class CAnalasisDlg;
extern CAnalasisDlg *g_pCAnalasisDlg;


UINT ThreadReceiveResult(LPVOID lparam);

UINT ThreadCheckConnect(LPVOID lparam);

UINT ThreadPackAndSendProject(LPVOID lparam);

DWORD WINAPI ThreadRecv(LPVOID param);

CString GetLoginUserName();

struct ProFinInfo
{
	CString	 m_FinishedProjectName;
	CString	 m_FinishedProjectPath;
	CString  m_FinishedProjectInfoPath;
	CString	 m_FinishedProjectSendDate;
	CString	 m_FinishedProjectRecvDate;
	CString	 m_FinishedProjectSenderName;
	CString	 m_FinishedProjectPreResult;
};
//接收操作
typedef enum _IO_OPERATION
{
	IORecvFileName,//接收文件名
	IOFileSize,    //接收文件大小
	IORecvFile,    //接收项目文件
	IOStatus,      //发送接收完成处理
}IO_OPERATION;

typedef unsigned int (_stdcall *PTHREAD_START) (void*);

struct IOContext:WSAOVERLAPPED
{
	SOCKET hSocket;  //接收套接字
	int Port;       //端口号
	char Buffer[DATA_BUFSIZE]; //缓冲区
	int nBytesInBuf;           //缓冲区的字节数
	int nBytesFileReceived;    //已接收的字节数
	int nBytesFileToRecv;      //需要接收的总字节数
	CString FileName;          //接收的文件名称
	char FilePath[MAX_PATH];   //文件的保存路径
	ofstream oFile;            //文件句柄
	WSABUF wsaBuffer;          
	IO_OPERATION op;    //操作类型
	IOContext(SOCKET socket):hSocket(socket),nBytesInBuf(0),nBytesFileReceived(0),nBytesFileToRecv(0)
	{
		memcpy(FilePath,"Recv\\",sizeof("Recv\\"));
		ZeroMemory(Buffer,DATA_BUFSIZE);
		wsaBuffer.buf = Buffer;
		wsaBuffer.len = DATA_BUFSIZE;
		op = IOFileSize;  //初始化接收文件大小
		ZeroMemory(this,sizeof(WSAOVERLAPPED));
	}
};

typedef vector<IOContext *> ConnectionList;


class CAnalasisDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAnalasisDlg)

public:
	CAnalasisDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAnalasisDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_ANALASIS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrlEx m_ListInput;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	CRect m_rect;
	afx_msg void OnBnClickedButtonSpeccompare();
	CSpectrumCompareDlg *m_pDlg;
	afx_msg void OnButtonSpeccompare();
	afx_msg void OnBnClickedButtonImportspec();
	CString m_SpecsToAnaFolderPath;

	CArray<CString,CString> m_ary_PicturesPath;
	CArray<CString,CString> m_ary_PicturesName;

	CArray<CString,CString>	m_ary_SpecFileTitle;
	CArray<CString,CString>	m_ary_SpecFilePath;
	CArray<CString,CString>	m_ary_SpecFileName;
	int m_PicIndex;

	CArray<ProFinInfo,ProFinInfo> m_ary_FinishedPro;

	

	afx_msg void OnBnClickedButtonLastpic();
	afx_msg void OnBnClickedButtonNextpic();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedButtonSendforanalasis();

	SOCKET m_sock;
	
	CString m_UserName;

    CString m_ProjectName;
	CString m_WineName;
	CString m_AlcoholContent;
	CString m_Flavour;
	CString m_Brand;
	CString m_MailAddr;
	CString m_SenderName;
	CString m_AnaType;
	CProjectSendProgressDlg *pCProjectSendProgressDlg;
	CWinThread  *pCThreadWnd;
	CWinThread *pThreadReceiveResult;
//打印相关变量
public:
	CFont m_fontPrinter;
	UINT m_cyPrinter;
	UINT m_cxWidth;
	UINT m_nLinesPerPage;
	UINT m_cxOffset;
	UINT m_MaxPage;
protected:
	afx_msg LRESULT OnBeginshowsendprogress(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEndshowsendprogress(WPARAM wParam, LPARAM lParam);	
public:
	int EndCheckConnect(void);
	CListCtrlEx m_ListFinishedProject;
	CListCtrlEx m_ListDetailResult;
public:
	afx_msg LRESULT OnBegincheckconnect(WPARAM wParam, LPARAM lParam);
	int UpdateFinishedProList(void);
	int AddFinishedPro(void);
	bool AddRecvPro(CString proPath);
	afx_msg void OnDestroy();
	afx_msg void OnNMClickListFinishedproject(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonImportfinishedproject();
	afx_msg void OnBnClickedButtonPrintresult();
protected:
	afx_msg LRESULT OnBeginPrinting(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEndPrinting(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMyPrint(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnBnClickedButtonPrintpreview();
	void PrintPageHeader(CDC* pDC, UINT nPageNumber, CPrintInfo* pInfo);
	void ReleaseResource(void);
	void UpdateFinishedDB(void);
	int InitWind(void);
	afx_msg void OnClose();
	afx_msg void OnBnClickedButtonDeletefinished();
	afx_msg void OnNMDblclkListFinishedproject(NMHDR *pNMHDR, LRESULT *pResult);
//	CButtonST m_btnNextPic;
	CButtonST m_btnLastPic;
	CButtonST m_btnNextPic;
	afx_msg void OnBnClickedButtonComponentsanalasis();
};
