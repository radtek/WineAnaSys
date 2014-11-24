#pragma once
#include "afxwin.h"


// CPCAResettingDlg 对话框

class CPCAResettingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPCAResettingDlg)

public:
	CPCAResettingDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPCAResettingDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_PCARESETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_RadioPCAShow;
	afx_msg void OnBnClickedRadioReset2pca();
	afx_msg void OnBnClickedRadioResetMultipca();
	CComboBox m_ComboXaxisPC;
	int xPC;
	CComboBox m_ComboYaxisPC;
	int yPC;
	CComboBox m_ComboPCNum;
	int PCNum;

	int TotalPCNum;

	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
