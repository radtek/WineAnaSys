// UserLoginDlg.cpp : ʵ���ļ�
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
// CUserLoginDlg �Ի���
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


// CUserLoginDlg ��Ϣ�������
void CUserLoginDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	// TODO: �ڴ˴������Ϣ����������
}


BOOL CUserLoginDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	HICON m_hIcon;
	m_hIcon=AfxGetApp()->LoadIcon(IDR_MAINFRAME);//�޸ĶԻ����ͼ��
	SetIcon(m_hIcon,TRUE);


	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	ModifyStyleEx(0,WS_EX_APPWINDOW);
	GetClientRect(&m_rect);
	SetBackgroundColor(RGB(247,252,255));
	//��ʼ��ȫ�ֱ���g_CurrentDir(��ǰĿ¼����·��)
	GetCurrentDirectory(MAX_PATH,g_CurrentDir.GetBuffer(MAX_PATH));
	g_CurrentDir.ReleaseBuffer(); 
	//��ȡ֮ǰ������û���Ϣ������еĻ�,����sqlite���ݿ�
	sqlite3* UserInfodb = NULL;
	//�����ݿ⣬�������򴴽�
	if(sqlite3_open("User\\UserInfo.db3", &UserInfodb) != 0)
	{
		MessageBox(L"���û���Ϣ�б�ʧ��",L"��ʼ��",MB_ICONERROR);
		return FALSE;
	}
	//�����û���Ϣ����Ѵ����򴴽�ʧ�ܣ�������Ϣ������szErrmsg��
	char *sqlcmd = "CREATE TABLE UserInfo(\
				     UserName string PRIMARY KEY,\
					 PassWord string )";

	char *szErrmsg = NULL;
	sqlite3_exec(UserInfodb,sqlcmd,0,0,&szErrmsg);
	//��ѯ�û�����
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
		//������벻Ϊ�գ������
		char *userKey = "1234567887654321"; // ԭʼ��Կ128λ��16�ֽ�
		CString sDataLen;
		GetPrivateProfileString(L"UserSetting",L"PasswordEnCodeLen",L"",sDataLen.GetBuffer(10),10,m_UserIniFilePath);
		int datalen = _wtoi(sDataLen);
		char keyDecode[256]; //������Կ
		char *PassWordDecoded = NULL;
		//char *dataSrc = NULL; //���������
		//��תΪ16���ƴ�������תΪ��ʼ״̬

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
		//���ý�����Կ
		if(0!=AES_set_decrypt_key((const unsigned char *)userKey,128,(AES_KEY *)keyDecode))
		{
			return -1;
		}
		//ѭ�����ܣ���16�ֽ�Ϊ��λ
		int count = datalen/16;
		//����
		for(int i = 0;i<count;++i)
		{
			memset(in, 0, 16);             
			memset(out, 0, 16); 	
			memcpy(in,passwordEncode+i*16,16);
			AES_ecb_encrypt(in,out,(AES_KEY *)keyDecode,AES_DECRYPT);
			memcpy(PassWordDecoded+i*16,out,16);
		}
		//��ʼ������
		m_PassWord = Char2CString(PassWordDecoded);
		//�ͷ���Դ
		free(passwordEncode);
		passwordEncode = NULL;
		free(PassWordDecoded);
		PassWordDecoded = NULL;
	}
	//�ر����ݿ�
	sqlite3_close(UserInfodb);
	UserInfodb = NULL;	
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CUserLoginDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	//�����û���Ϣ
	//��������ѡ��
	CString szCheckPW;
	szCheckPW.Format(L"%d",m_CheckPW);
	WritePrivateProfileString(L"UserSetting",L"SavePassword",szCheckPW,m_UserIniFilePath);


	sqlite3* pUserInfodb = NULL;
	char *pErrMsg;
	//�����ݿ⣬�������򴴽�
	if(sqlite3_open("User\\UserInfo.db3", &pUserInfodb) != 0)
	{
		MessageBox(L"���û���Ϣ�б�ʧ��",L"��ʼ��",MB_ICONERROR);
		return;
	}

	char *pUserName = CString2Char(m_UserName);

	char UserName[MAX_USERNAMELEN] = {0};
	char *PassWordEncode = NULL;
	memset(UserName,0,MAX_USERNAMELEN);
	//memset(PassWordEncode,0,MAX_PASSWORDLEN);
	memcpy(UserName,pUserName,strlen(pUserName));
	//�ͷ���Դ
	free(pUserName);
	pUserName = NULL;
	//ɾ����������
	sqlite3_exec(pUserInfodb,"DELETE from UserInfo",0,0,&pErrMsg);
	if(m_CheckPW)
	{
		//��������
		char *pPassWord = CString2Char(m_PassWord);
		char PassWord[MAX_PASSWORDLEN];
		memset(PassWord,0,MAX_PASSWORDLEN);
		memcpy(PassWord,pPassWord,strlen(pPassWord));
		free(pPassWord);
		char *userKey = "1234567887654321"; // ԭʼ��Կ128λ��16�ֽ�
		unsigned char keyEncode[256] = {0};   // ������Կ
		unsigned char keyDecode[256] = {0}; //������Կ

		char *PassWordSrc = NULL; //���������
		unsigned char in[16];
		unsigned char out[16];
		int datalen = strlen(PassWord);
		int data_rest = datalen%16;
		//���������������16λ
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
		//����֮�������
		memset(PassWordSrc,0,datalen);
		memcpy(PassWordSrc,PassWord,strlen(PassWord));
		//���ü�����Կ
		if(0!=AES_set_encrypt_key((const unsigned char *)userKey,128,(AES_KEY *)keyEncode))
		{
			return;
		}
		//ѭ�����ܣ���16�ֽ�Ϊ��λ
		int count = datalen/16;
		for(int i = 0;i<count;++i)
		{
			memset(in, 0, 16);             
			memset(out, 0, 16); 
			memcpy(in,PassWordSrc+i*16,16);
			AES_ecb_encrypt(in,out,(AES_KEY *)keyEncode,AES_ENCRYPT);
			memcpy(PassWordEncode+i*16,out,16);
		}
		//�������볤��
		CString szPasswordEnCodeLen;
		szPasswordEnCodeLen.Format(L"%d",datalen);
		WritePrivateProfileString(L"UserSetting",L"PasswordEnCodeLen",szPasswordEnCodeLen,m_UserIniFilePath);
		free(PassWordSrc);
		PassWordSrc = NULL;
	}
    //�������û�
	//�����ܺ������תΪ16����
	char PassWordEncodeHex[500];
	memset(PassWordEncodeHex,0,500);
	for (int i = 0; i < strlen(PassWordEncode); i++)
	{
		_snprintf(PassWordEncodeHex +2*i,500-2*i,"%02X",(unsigned char)PassWordEncode[i]);
	}
	char sqlcmd[500];
	_snprintf(sqlcmd,500,"INSERT INTO UserInfo(UserName,PassWord)values('%s','%s')",UserName,PassWordEncodeHex);
	sqlite3_exec(pUserInfodb,sqlcmd,0,0,&pErrMsg);
	if(!ConnectServer())
		return;

	CDialogEx::OnOK();
}
//���ӷ�����
bool CUserLoginDlg::ConnectServer()
{
	
	//�û���Ϣ
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

	//���ӷ�����
	int err;
	WSADATA wsaData;
	err=WSAStartup(MAKEWORD(2,2),&wsaData);
	//��������ַ
	sockaddr_in serveraddr;
	serveraddr.sin_family=AF_INET;  //Э��
	serveraddr.sin_port=htons(SERVER_PROT);//���������̶˿�
	serveraddr.sin_addr.s_addr=inet_addr("127.0.0.1");//���������̵�IP

	int ret = 0;
	do
	{
		m_sock=socket(AF_INET,SOCK_STREAM,0);
		int retval=connect(m_sock,(sockaddr*)&serveraddr,sizeof(serveraddr));
		if(SOCKET_ERROR==retval)
		{
			int err = GetLastError();
			MessageBox(L"�����������ʧ�ܣ�",L"��¼ʧ��",MB_ICONERROR);
			closesocket(m_sock);
			return false;
		}
		//����Ϊ������ģʽ
		int mode=1;
		if(SOCKET_ERROR==ioctlsocket(m_sock,FIONBIO,(u_long FAR*) &mode))
		{
			//WSACleanup();
			closesocket(m_sock);
			MessageBox(L"�����������ʧ�ܣ�",L"��¼ʧ��",MB_ICONERROR);
			return false;
		}
		//���շ��������ع�Կ��Ϣ
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
					//���µ�¼
					MessageBox(L"����������ӳ�ʱ�������µ�¼��",L"����ʧ��",MB_ICONERROR);
					return false;
				}
			}
			break;
		}
		//���û���Ϣ���ܺ��͸�������
		//����
		if(rsa_publickey)
		{
			RSA_free(rsa_publickey);
			rsa_publickey = NULL;
		}
		rsa_publickey = RSA_new();//���ɹ�Կ
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

	//���ͼ��ܺ������
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
				//���µ�¼
				MessageBox(L"����������ӳ�ʱ�������µ�¼��",L"����ʧ��",MB_ICONERROR);
				return false;
			}
		}
		break;
	}

	//���շ��������صĵ�¼��Ϣ
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
				//���µ�¼
				MessageBox(L"����������ӳ�ʱ�������µ�¼��",L"����ʧ��",MB_ICONERROR);
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
		MessageBox(L"�����ڵ��û�����",L"��¼ʧ��",MB_ICONERROR);
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
		MessageBox(L"�������",L"��¼ʧ��",MB_ICONERROR);
		return false;
	}
	if(strcmp(LogState,"LoginSuccess") == 0)
	{
		//g_pCAnalasisDlg=new CAnalasisDlg;
		//g_pCAnalasisDlg->Create(IDD_ANALASIS,0);
		//g_pCAnalasisDlg->m_sock = m_sock;
		//g_pCAnalasisDlg->ShowWindow(SW_HIDE);
		return true;
	}
	MessageBox(L"����������ӳ�ʱ�������µ�¼��",L"����ʧ��",MB_ICONERROR);
	return false;
}


void CUserLoginDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnClose();
}
