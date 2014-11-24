// InputUserInfoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DataServerCenter.h"
#include "InputUserInfoDlg.h"
#include "afxdialogex.h"


// CInputUserInfoDlg 对话框

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


// CInputUserInfoDlg 消息处理程序


void CInputUserInfoDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	if(m_UserName.IsEmpty())
	{
		MessageBox(L"用户名不能为空！",L"用户信息",MB_ICONERROR);
		return;
	}
	if(m_PassWord.IsEmpty())
	{
		MessageBox(L"密码不能为空！",L"用户信息",MB_ICONERROR);
		return;
	}
	if(m_Petitioner.IsEmpty())
	{
		MessageBox(L"申请人不能为空！",L"用户信息",MB_ICONERROR);
		return;
	}
	if(m_Institution.IsEmpty())
	{
		MessageBox(L"申请单位不能为空！",L"用户信息",MB_ICONERROR);
		return;
	}
	if(m_TeleNum.IsEmpty())
	{
		MessageBox(L"电话号码不能为空！",L"用户信息",MB_ICONERROR);
		return;
	}
	if(m_Email.IsEmpty())
	{
		MessageBox(L"电子邮箱不能为空！",L"用户信息",MB_ICONERROR);
		return;
	}
	CDialogEx::OnOK();
}
