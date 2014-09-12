#pragma once
#include "afxwin.h"


// CSmoothSettingDlg 对话框

class CSmoothSettingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSmoothSettingDlg)

public:
	CSmoothSettingDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSmoothSettingDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_SMOOTHSETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_SmoothPts;
public:
	CComboBox m_ComboSmooth;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};

