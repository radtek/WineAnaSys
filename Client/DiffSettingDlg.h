#pragma once
#include "afxwin.h"


// CDiffSettingDlg 对话框

class CDiffSettingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDiffSettingDlg)

public:
	CDiffSettingDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDiffSettingDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_DIFFSETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_Order;
public:
	CComboBox m_ComboOrder;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
