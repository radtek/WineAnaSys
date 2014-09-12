#pragma once
#include "afxwin.h"


// CWdenSettingDlg 对话框

class CWdenSettingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CWdenSettingDlg)

public:
	CWdenSettingDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CWdenSettingDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_WDENSETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_wName;//小波名称
	int m_DecLevel; //分解层数
	CString m_TTppe;
	CString m_TScal;
	CString m_TSORH;

public:
	CComboBox m_ComboWname1;
	CComboBox m_ComboWname2;
	CComboBox m_ComboDecLevel;
	CComboBox m_ComboTType;
	CComboBox m_ComboSorH;
	CComboBox m_ComboTScal;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnCbnSelchangeComboWname1set();
};
