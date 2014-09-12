// UserManageDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DataServerCenter.h"
#include "UserManageDlg.h"
#include "afxdialogex.h"
#include "InputUserInfoDlg.h"

// CUserManageDlg �Ի���

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


// CUserManageDlg ��Ϣ�������

BOOL CUserManageDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	if(!m_UserManage.Open())
	{
		MessageBox(L"�������ݿ�ʧ��",L"���ݿ����",MB_ICONWARNING);
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
	m_ListUserInfo.InsertColumn(0,L"�û�", LVCFMT_LEFT, 150 );
	m_ListUserInfo.InsertColumn(1,L"������", LVCFMT_LEFT, 150 );
	m_ListUserInfo.InsertColumn(2,L"���뵥λ", LVCFMT_LEFT, 150 );
	m_ListUserInfo.InsertColumn(3,L"�绰", LVCFMT_LEFT, 150 );
	m_ListUserInfo.InsertColumn(4,L"��������", LVCFMT_LEFT, 150 );
	m_ListUserInfo.InsertColumn(5,L"��������", LVCFMT_LEFT, 150 );

	DWORD dwStyle = m_ListUserInfo.GetExtendedStyle(); //��ȡ��ǰ��չ��ʽ
	dwStyle |= LVS_EX_FULLROWSELECT; //ѡ��ĳ��ʹ���и�����report���ʱ��
	dwStyle |= LVS_EX_GRIDLINES; //�����ߣ�report���ʱ��
	dwStyle |= LVS_EX_CHECKBOXES; //
	m_ListUserInfo.SetExtendedStyle(dwStyle); //������չ���

	UpdateUserList();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_UserManage.Close();
	CDialogEx::OnOK();
}
