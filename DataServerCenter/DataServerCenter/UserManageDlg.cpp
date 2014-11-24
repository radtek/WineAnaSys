// UserManageDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DataServerCenter.h"
#include "UserManageDlg.h"
#include "afxdialogex.h"
#include "InputUserInfoDlg.h"

// CUserManageDlg 对话框

IMPLEMENT_DYNAMIC(CUserManageDlg, CDialogEx)

CUserManageDlg::CUserManageDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUserManageDlg::IDD, pParent)
{

}

CUserManageDlg::~CUserManageDlg()
{
}

void CUserManageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_USER, m_ListUserInfo);
}


BEGIN_MESSAGE_MAP(CUserManageDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_ADDUSER, &CUserManageDlg::OnBnClickedButtonAdduser)
	ON_BN_CLICKED(IDC_BUTTON_DELETEUSER, &CUserManageDlg::OnBnClickedButtonDeleteuser)
	ON_BN_CLICKED(IDOK, &CUserManageDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CUserManageDlg 消息处理程序

BOOL CUserManageDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	if(!m_UserManage.Open())
	{
		MessageBox(L"连接数据库失败",L"数据库出错",MB_ICONWARNING);
	}
	if(!m_UserManage.IsEOF())
	{
		m_UserManage.MoveFirst();
		while(!m_UserManage.IsEOF())
		{
			UserInfo tmpUserInfo;
			tmpUserInfo.m_UserName = m_UserManage.m_UserName;
			tmpUserInfo.m_Petitioner = m_UserManage.m_Petitioner;
			tmpUserInfo.m_Institution = m_UserManage.m_Institution;
			tmpUserInfo.m_Telephone = m_UserManage.m_Telephone;
			tmpUserInfo.m_Email = m_UserManage.m_Email;
			tmpUserInfo.m_Applicationdate = m_UserManage.m_Applicationdate;
			m_UserArray.Add(tmpUserInfo);
			m_UserManage.MoveNext();
		}
	}
	m_ListUserInfo.InsertColumn(0,L"用户", LVCFMT_LEFT, 150 );
	m_ListUserInfo.InsertColumn(1,L"申请人", LVCFMT_LEFT, 150 );
	m_ListUserInfo.InsertColumn(2,L"申请单位", LVCFMT_LEFT, 150 );
	m_ListUserInfo.InsertColumn(3,L"电话", LVCFMT_LEFT, 150 );
	m_ListUserInfo.InsertColumn(4,L"电子邮箱", LVCFMT_LEFT, 150 );
	m_ListUserInfo.InsertColumn(5,L"申请日期", LVCFMT_LEFT, 150 );

	DWORD dwStyle = m_ListUserInfo.GetExtendedStyle(); //获取当前扩展样式
	dwStyle |= LVS_EX_FULLROWSELECT; //选中某行使整行高亮（report风格时）
	dwStyle |= LVS_EX_GRIDLINES; //网格线（report风格时）
	dwStyle |= LVS_EX_CHECKBOXES; //
	m_ListUserInfo.SetExtendedStyle(dwStyle); //设置扩展风格

	UpdateUserList();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CUserManageDlg::UpdateUserList(void)
{
	m_ListUserInfo.DeleteAllItems();
	int totalItem = m_UserArray.GetCount();
	for(int i=0;i<totalItem;++i)
	{
		m_ListUserInfo.InsertItem(i,m_UserArray[i].m_UserName);
		m_ListUserInfo.SetItemText(i,1,m_UserArray[i].m_Petitioner);
		m_ListUserInfo.SetItemText(i,2,m_UserArray[i].m_Institution);
		m_ListUserInfo.SetItemText(i,3,m_UserArray[i].m_Telephone);
		m_ListUserInfo.SetItemText(i,4,m_UserArray[i].m_Email);
		m_ListUserInfo.SetItemText(i,5,m_UserArray[i].m_Applicationdate);
	}

}

void CUserManageDlg::OnBnClickedButtonAdduser()
{
	// TODO: 在此添加控件通知处理程序代码
	CInputUserInfoDlg InputUserInfoDlg;
	if(IDOK == InputUserInfoDlg.DoModal())
	{
		CString	m_UserName = InputUserInfoDlg.m_UserName;
		CString	m_PassWord = InputUserInfoDlg.m_PassWord;
		CString	m_Petitioner = InputUserInfoDlg.m_Petitioner;
		CString	m_Institution = InputUserInfoDlg.m_Institution;
		CString	m_Telephone = InputUserInfoDlg.m_TeleNum;
		CString	m_Email = InputUserInfoDlg.m_Email;
		CString	m_Applicationdate = GetDate();

		m_UserManage.AddNew();
		m_UserManage.m_UserName = m_UserName;
		m_UserManage.m_PassWord = m_PassWord;
		m_UserManage.m_Petitioner = m_Petitioner;
		m_UserManage.m_Institution = m_Institution;
		m_UserManage.m_Telephone = m_Telephone;
		m_UserManage.m_Email = m_Email;
		m_UserManage.m_Applicationdate = m_Applicationdate;
		m_UserManage.Update();

		UserInfo tmpUserInfo;
		tmpUserInfo.m_UserName = m_UserName;
		tmpUserInfo.m_Petitioner = m_Petitioner;
		tmpUserInfo.m_Institution = m_Institution;
		tmpUserInfo.m_Telephone = m_Telephone;
		tmpUserInfo.m_Email = m_Email;
		tmpUserInfo.m_Applicationdate = m_Applicationdate;
		m_UserArray.Add(tmpUserInfo);
		UpdateUserList();
	}
}


void CUserManageDlg::OnBnClickedButtonDeleteuser()
{
	// TODO: 在此添加控件通知处理程序代码
	int iitem=m_ListUserInfo.GetItemCount();
	if(0 == iitem)
		return;
	for(int i=0; i<m_ListUserInfo.GetItemCount(); i++)
	{
		if(m_ListUserInfo.GetItemState(i, LVIS_SELECTED)==
			LVIS_SELECTED || m_ListUserInfo.GetCheck(i))
		{
			m_UserManage.m_strFilter.Format(L" UserName = '%s' ",m_UserArray[i].m_UserName);
			m_UserManage.Requery();
			m_UserManage.Delete();
			m_UserArray.RemoveAt(i);
		}
	}
	UpdateUserList();
}

void CUserManageDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	m_UserManage.Close();
	CDialogEx::OnOK();
}
