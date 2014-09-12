#pragma once


// CUserManageDlg �Ի���
#include "UserManage.h"
#include "afxcmn.h"
struct UserInfo
{
	CString m_UserName;
	CString m_Petitioner;
	CString m_Institution;
	CString m_Telephone;
	CString m_Email;
	CString m_Applicationdate;
};

class CUserManageDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CUserManageDlg)

public:
	CUserManageDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CUserManageDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_USERMANAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonAdduser();
	afx_msg void OnBnClickedButtonDeleteuser();
private:
	CUserManage m_UserManage;
	CArray<UserInfo> m_UserArray;

public:
	virtual BOOL OnInitDialog();
	void UpdateUserList(void);
	CListCtrl m_ListUserInfo;
	afx_msg void OnBnClickedOk();
};


