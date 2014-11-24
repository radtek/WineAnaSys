#pragma once
#include "afxcmn.h"


// CProjectInfoConfirmDlg 对话框

class CProjectInfoConfirmDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CProjectInfoConfirmDlg)

public:
	CProjectInfoConfirmDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CProjectInfoConfirmDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_PROJECTINFOCONFIRM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CListCtrl m_ListProjectInfo;
    CString m_ProjectName;
	CString m_WineName;
	CString m_AlcoholContent;
	CString m_Flavour;
	CString m_Brand;
	CString m_MailAddr;
	CString m_SenderName;
	CString m_AnaType;
	virtual BOOL OnInitDialog();
};
