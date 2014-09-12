#pragma once


// CInputUserInfoDlg 对话框

class CInputUserInfoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CInputUserInfoDlg)

public:
	CInputUserInfoDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CInputUserInfoDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_INPUT_USERINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_UserName;
	CString m_PassWord;
	CString m_Petitioner;
	CString m_Institution;
	CString m_TeleNum;
	CString m_Email;
	afx_msg void OnBnClickedOk();
};
