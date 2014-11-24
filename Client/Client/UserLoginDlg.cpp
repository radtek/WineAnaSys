// UserLoginDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Client.h"
#include "UserLoginDlg.h"
#include "afxdialogex.h"
#include "ClientView.h"
#include "UserManage.h"
#include "ZipImplement.h"
#include "AnalasisDlg.h"
#include <openssl\bn.h>
#include "sqlite3.h"
#include "AnalasisDlg.h"
// CUserLoginDlg 对话框
//extern SOCKET sClient;
CLoginingDlg *pCLoginingDlg = NULL;

extern RSA *rsa_publickey;
extern CString g_CurrentDir;
UINT ThreadCheckConnect(LPVOID lparam);
IMPLEMENT_DYNAMIC(CUserLoginDlg, CDialogEx)

CUserLoginDlg::CUserLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUserLoginDlg::IDD, pParent)
	, m_UserName(_T(""))
	, m_PassWord(_T(""))
	, m_CheckPW(FALSE)
	,m_sock(INVALID_SOCKET)
{
	m_pParent=NULL;
}

CUserLoginDlg::~CUserLoginDlg()
{
}

void CUserLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_USERNAME, m_UserName);
	DDX_Text(pDX, IDC_EDIT_PWD, m_PassWord);
	DDX_Check(pDX, IDC_CHECK_SavePW, m_CheckPW);
}


BEGIN_MESSAGE_MAP(CUserLoginDlg, CDialogEx)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDEXIT, &CUserLoginDlg::OnBnClickedExit)
	ON_BN_CLICKED(IDOK, &CUserLoginDlg::OnBnClickedOk)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CUserLoginDlg 消息处理程序
void CUserLoginDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	// TODO: 在此处添加消息处理程序代码
}


BOOL CUserLoginDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	HICON m_hIcon;
	m_hIcon=AfxGetApp()->LoadIcon(IDR_MAINFRAME);//修改对话框的图标
	SetIcon(m_hIcon,TRUE);


	// TODO:  在此添加额外的初始化
	ModifyStyleEx(0,WS_EX_APPWINDOW);
	GetClientRect(&m_rect);
	SetBackgroundColor(RGB(247,252,255));
	//初始化全局变量g_CurrentDir(当前目录绝对路径)
	GetCurrentDirectory(MAX_PATH,g_CurrentDir.GetBuffer(MAX_PATH));
	g_CurrentDir.ReleaseBuffer(); 
	//提取之前保存的用户信息，如果有的话,利用sqlite数据库
	sqlite3* UserInfodb = NULL;
	//打开数据库，不存在则创建
	if(sqlite3_open("User\\UserInfo.db3", &UserInfodb) != 0)
	{
		MessageBox(L"打开用户信息列表失败",L"初始化",MB_ICONERROR);
		return FALSE;
	}
	//创建用户信息表格，已存在则创建失败，错误信息保存在szErrmsg中
	char *sqlcmd = "CREATE TABLE UserInfo(\
				     UserName string PRIMARY KEY,\
					 PassWord string )";

	char *szErrmsg = NULL;
	sqlite3_exec(UserInfodb,sqlcmd,0,0,&szErrmsg);
	//查询用户数据
	char *zsql = "SELECT * FROM UserInfo";
	char **szResult = NULL;
	int nRow = 0;
	int nColumn = 0;
	sqlite3_get_table(UserInfodb,zsql,&szResult,&nRow,&nColumn,&szErrmsg);

	char username[MAX_USERNAMELEN] = {0};
	char passwordEncodeHex[500] = {0};
	char *passwordEncode = NULL;
	memset(username,0,MAX_USERNAMELEN);
	memset(passwordEncodeHex,0,500);
	int l = strlen(szResult[3]);
	if(nRow)
	{
		memcpy(username,szResult[2],strlen(szResult[2]));
		memcpy(passwordEncodeHex,szResult[3],strlen(szResult[3]));
	}

	m_UserIniFilePath = g_CurrentDir;
	m_UserIniFilePath+=L"\\config\\config.ini";
	m_UserName = Char2CString(username);
	m_CheckPW=GetPrivateProfileInt(L"UserSetting",L"SavePassword",0,m_UserIniFilePath);
	if(strlen(passwordEncodeHex))
	{
		//如果密码不为空，则解密
		char *userKey = "1234567887654321"; // 原始密钥128位，16字节
		CString sDataLen;
		GetPrivateProfileString(L"UserSetting",L"PasswordEnCodeLen",L"",sDataLen.GetBuffer(10),10,m_UserIniFilePath);
		int datalen = _wtoi(sDataLen);
		char keyDecode[256]; //解密密钥
		char *PassWordDecoded = NULL;
		//char *dataSrc = NULL; //补齐的数据
		//将转为16进制串的密码转为初始状态

		int L=strlen(passwordEncodeHex);
		passwordEncode = (char*)malloc(sizeof(char)*(L));
		memset(passwordEncode,0,L);
		int  x;
		for(int i=0;i<L/2;i++) 
		{		
			sscanf(passwordEncodeHex+i*2,"%02X",&x);
			passwordEncode[i]=(char)x;//(x&0xFFu);
		}
		unsigned char in[16];
		unsigned char out[16];
		PassWordDecoded = (char*)malloc(sizeof(char)*datalen);
		if(!PassWordDecoded)
		{
			return -1;
		}
		memset(PassWordDecoded,0,datalen);
		//设置解密密钥
		if(0!=AES_set_decrypt_key((const unsigned char *)userKey,128,(AES_KEY *)keyDecode))
		{
			return -1;
		}
		//循环加密，以16字节为单位
		int count = datalen/16;
		//解密
		for(int i = 0;i<count;++i)
		{
			memset(in, 0, 16);             
			memset(out, 0, 16); 	
			memcpy(in,passwordEncode+i*16,16);
			AES_ecb_encrypt(in,out,(AES_KEY *)keyDecode,AES_DECRYPT);
			memcpy(PassWordDecoded+i*16,out,16);
		}
		//初始化密码
		m_PassWord = Char2CString(PassWordDecoded);
		//释放资源
		free(passwordEncode);
		passwordEncode = NULL;
		free(PassWordDecoded);
		PassWordDecoded = NULL;
	}
	//关闭数据库
	sqlite3_close(UserInfodb);
	UserInfodb = NULL;	
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CUserLoginDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	//保存用户信息
	//保存密码选项
	CString szCheckPW;
	szCheckPW.Format(L"%d",m_CheckPW);
	WritePrivateProfileString(L"UserSetting",L"SavePassword",szCheckPW,m_UserIniFilePath);


	sqlite3* pUserInfodb = NULL;
	char *pErrMsg;
	//打开数据库，不存在则创建
	if(sqlite3_open("User\\UserInfo.db3", &pUserInfodb) != 0)
	{
		MessageBox(L"打开用户信息列表失败",L"初始化",MB_ICONERROR);
		return;
	}

	char *pUserName = CString2Char(m_UserName);

	char UserName[MAX_USERNAMELEN] = {0};
	char *PassWordEncode = NULL;
	memcpy(UserName,pUserName,strlen(pUserName));
	//释放资源
	free(pUserName);
	pUserName = NULL;
	//删除已有数据
	sqlite3_exec(pUserInfodb,"DELETE from UserInfo",0,0,&pErrMsg);
	if(m_CheckPW)
	{
		//保存密码
		char *pPassWord = CString2Char(m_PassWord);
		char PassWord[MAX_PASSWORDLEN];
		memset(PassWord,0,MAX_PASSWORDLEN);
		memcpy(PassWord,pPassWord,strlen(pPassWord));
		free(pPassWord);
		char *userKey = "1234567887654321"; // 原始密钥128位，16字节
		unsigned char keyEncode[256] = {0};   // 加密密钥
		unsigned char keyDecode[256] = {0}; //解密密钥

		char *PassWordSrc = NULL; //补齐的数据
		unsigned char in[16];
		unsigned char out[16];
		int datalen = strlen(PassWord);
		int data_rest = datalen%16;
		//如果除不尽，则补齐16位
		datalen+=16-data_rest;
		PassWordSrc = (char*)malloc(sizeof(char)*datalen);
		if(!PassWordSrc)
		{
			return;
		}
		PassWordEncode = (char*)malloc(sizeof(char)*datalen);
		if(!PassWordEncode)
		{
			return;
		}
		//补齐之后的明文
		memset(PassWordSrc,0,datalen);
		memcpy(PassWordSrc,PassWord,strlen(PassWord));
		//设置加密密钥
		if(0!=AES_set_encrypt_key((const unsigned char *)userKey,128,(AES_KEY *)keyEncode))
		{
			return;
		}
		//循环加密，以16字节为单位
		int count = datalen/16;
		for(int i = 0;i<count;++i)
		{
			memset(in, 0, 16);             
			memset(out, 0, 16); 
			memcpy(in,PassWordSrc+i*16,16);
			AES_ecb_encrypt(in,out,(AES_KEY *)keyEncode,AES_ENCRYPT);
			memcpy(PassWordEncode+i*16,out,16);
		}
		//保存密码长度
		CString szPasswordEnCodeLen;
		szPasswordEnCodeLen.Format(L"%d",datalen);
		WritePrivateProfileString(L"UserSetting",L"PasswordEnCodeLen",szPasswordEnCodeLen,m_UserIniFilePath);
		free(PassWordSrc);
		PassWordSrc = NULL;
	}
    //插入新用户
	//将加密后的密码转为16进制
	char PassWordEncodeHex[500];
	memset(PassWordEncodeHex,0,500);
	for (int i = 0; i < strlen(PassWordEncode); i++)
	{
		_snprintf(PassWordEncodeHex +2*i,500-2*i,"%02X",(unsigned char)PassWordEncode[i]);
	}
	char sqlcmd[500];
	_snprintf(sqlcmd,500,"INSERT INTO UserInfo(UserName,PassWord)values('%s','%s')",UserName,PassWordEncodeHex);
	sqlite3_exec(pUserInfodb,sqlcmd,0,0,&pErrMsg);
	sqlite3_close(pUserInfodb);

	if(!ConnectServer())
	{
		return;
	}
	CDialogEx::OnOK();
}
//连接服务器
bool CUserLoginDlg::ConnectServer()
{
	
	//用户信息
	unsigned char UserInfo[DATA_BUFSIZE] = {0};
	unsigned char UserInfoEncode[DATA_BUFSIZE] = {0};

	char szPort[32] = {0};
	char *pUserName = CString2Char(m_UserName);
	char *pPassWord = CString2Char(m_PassWord);
	itoa(CLIENT_RECV_PORT,szPort,10);

	memcpy(UserInfo,pUserName,strlen(pUserName));
	memset(UserInfo+strlen(pUserName),',',1);
	memcpy(UserInfo+strlen((char*)UserInfo),pPassWord,strlen(pPassWord));
	memset(UserInfo+strlen((char*)UserInfo),',',1);
	memcpy(UserInfo+strlen((char*)UserInfo),szPort,strlen(szPort));

	//连接服务器

	//服务器地址
	sockaddr_in serveraddr;
	serveraddr.sin_family=AF_INET;  //协议
	serveraddr.sin_port=htons(SERVER_PROT);//服务器进程端口
	serveraddr.sin_addr.s_addr=inet_addr("127.0.0.1");//服务器进程的IP

	int ret = 0;
	do
	{
		m_sock=socket(AF_INET,SOCK_STREAM,0);
		int retval=connect(m_sock,(sockaddr*)&serveraddr,sizeof(serveraddr));
		if(SOCKET_ERROR==retval)
		{
			int err = GetLastError();
			MessageBox(L"与服务器连接失败！",L"登录失败",MB_ICONERROR);
			closesocket(m_sock);
			return false;
		}
		//设置为非阻塞模式
		int mode=1;
		if(SOCKET_ERROR==ioctlsocket(m_sock,FIONBIO,(u_long FAR*) &mode))
		{
			//WSACleanup();
			closesocket(m_sock);
			MessageBox(L"与服务器连接失败！",L"登录失败",MB_ICONERROR);
			return false;
		}
		//接收服务器返回公钥信息
		char rsa_n[DATA_BUFSIZE] = {0};
		while(true)
		{
			int retval=recv(m_sock,rsa_n,DATA_BUFSIZE,0);
			if(SOCKET_ERROR==retval)
			{
				int err=WSAGetLastError();
				if(err==WSAEWOULDBLOCK)
				{
					Sleep(200);
					continue;
				}
				else
				{
					closesocket(m_sock);
					//重新登录
					MessageBox(L"与服务器连接超时，请重新登录！",L"连接失败",MB_ICONERROR);
					return false;
				}
			}
			break;
		}
		//将用户信息加密后发送给服务器
		//加密
		if(rsa_publickey)
		{
			RSA_free(rsa_publickey);
			rsa_publickey = NULL;
		}
		rsa_publickey = RSA_new();//生成公钥
		BN_dec2bn(&rsa_publickey->n,rsa_n);
		BN_dec2bn(&rsa_publickey->e,"65537");
		int inputlen=RSA_size(rsa_publickey);
		ret = RSA_public_encrypt(inputlen,UserInfo,UserInfoEncode, rsa_publickey, RSA_NO_PADDING);

		if (ret < 0)
		{
			RSA_free(rsa_publickey);
			rsa_publickey = NULL;
			closesocket(m_sock);
			Sleep(200);
			continue;
		}
		else
		{
			break;
		}

	}while(1);

	//发送加密后的数据
	while(true)
	{
		int retval=send(m_sock,(char*)UserInfoEncode,DATA_BUFSIZE,0);
		if(SOCKET_ERROR==retval)
		{
			int err=WSAGetLastError();
			if(err==WSAEWOULDBLOCK)
			{
				Sleep(200);
				continue;
			}
			else
			{
				closesocket(m_sock);
				if(rsa_publickey)
				{
					RSA_free(rsa_publickey);
					rsa_publickey = NULL;
				}
				//重新登录
				MessageBox(L"与服务器连接超时，请重新登录！",L"连接失败",MB_ICONERROR);
				return false;
			}
		}
		break;
	}

	//接收服务器返回的登录消息
	char LogState[1024] = {0};
	while(true)
	{
		int retval=recv(m_sock,LogState,1024,0);
		if(SOCKET_ERROR==retval)
		{
			int err=WSAGetLastError();
			if(err==WSAEWOULDBLOCK)
			{
				Sleep(200);
				continue;
			}
			else
			{
				closesocket(m_sock);
				if(rsa_publickey)
				{
					RSA_free(rsa_publickey);
					rsa_publickey = NULL;
				}
				//重新登录
				MessageBox(L"与服务器连接超时，请重新登录！",L"连接失败",MB_ICONERROR);
				return false;
			}
		}
		break;
	}
	if(strcmp(LogState,"UserNonExist") == 0)
	{
		closesocket(m_sock);
		if(rsa_publickey)
		{
			RSA_free(rsa_publickey);
			rsa_publickey = NULL;
		}
		MessageBox(L"不存在的用户名！",L"登录失败",MB_ICONERROR);
		return false;
	}
	if(strcmp(LogState,"IncorrectPassWord") == 0)
	{
		closesocket(m_sock);
		if(rsa_publickey)
		{
			RSA_free(rsa_publickey);
			rsa_publickey = NULL;
		}
		MessageBox(L"密码错误！",L"登录失败",MB_ICONERROR);
		return false;
	}
	if(strcmp(LogState,"LoginSuccess") == 0)
	{
		return true;
	}
	MessageBox(L"与服务器连接超时，请重新登录！",L"连接失败",MB_ICONERROR);
	return false;
}


void CUserLoginDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(m_sock!=INVALID_SOCKET)
	{
		closesocket(m_sock);
	}
	AfxGetApp()->ExitInstance();
	ExitProcess(0);
	CDialogEx::OnClose();
}
void CUserLoginDlg::OnBnClickedExit()
{
	// TODO: 在此添加控件通知处理程序代码
	OnClose();
}
