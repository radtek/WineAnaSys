// InputUserInfoDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DataServerCenter.h"
#include "InputUserInfoDlg.h"
#include "afxdialogex.h"


// CInputUserInfoDlg �Ի���

IMPLEMENT_DYNAMIC(CInputUserInfoDlg, CDialogEx)

CInputUserInfoDlg::CInputUserInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CInputUserInfoDlg::IDD, pParent)
	, m_UserName(_T(""))
	, m_PassWord(_T(""))
	, m_Petitioner(_T(""))
	, m_Institution(_T(""))
	, m_TeleNum(_T(""))
	, m_Email(_T(""))
{

}

CInputUserInfoDlg::~CInputUserInfoDlg()
{
}

void CInputUserInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_USERNAME, m_UserName);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_PassWord);
	DDX_Text(pDX, IDC_EDIT_PETITIONER, m_Petitioner);
	DDX_Text(pDX, IDC_EDIT_INSTITUTION, m_Institution);
	DDX_Text(pDX, IDC_EDIT_PHONENUMBER, m_TeleNum);
	DDX_Text(pDX, IDC_EDIT_EMAIL, m_Email);
}


BEGIN_MESSAGE_MAP(CInputUserInfoDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CInputUserInfoDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CInputUserInfoDlg ��Ϣ�������


void CInputUserInfoDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);

	if(m_UserName.IsEmpty())
	{
		MessageBox(L"�û�������Ϊ�գ�",L"�û���Ϣ",MB_ICONERROR);
		return;
	}
	if(m_PassWord.IsEmpty())
	{
		MessageBox(L"���벻��Ϊ�գ�",L"�û���Ϣ",MB_ICONERROR);
		return;
	}
	if(m_Petitioner.IsEmpty())
	{
		MessageBox(L"�����˲���Ϊ�գ�",L"�û���Ϣ",MB_ICONERROR);
		return;
	}
	if(m_Institution.IsEmpty())
	{
		MessageBox(L"���뵥λ����Ϊ�գ�",L"�û���Ϣ",MB_ICONERROR);
		return;
	}
	if(m_TeleNum.IsEmpty())
	{
		MessageBox(L"�绰���벻��Ϊ�գ�",L"�û���Ϣ",MB_ICONERROR);
		return;
	}
	if(m_Email.IsEmpty())
	{
		MessageBox(L"�������䲻��Ϊ�գ�",L"�û���Ϣ",MB_ICONERROR);
		return;
	}
	CDialogEx::OnOK();
}
