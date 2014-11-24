#pragma once
#include "afxwin.h"


// CClusterSettingDlg 对话框

class CClusterSettingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CClusterSettingDlg)

public:
	CClusterSettingDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CClusterSettingDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_CLUSTERSETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	double m_eps;   //扫描半径
	int m_Minpts;//最小点数
	int m_ClusterNum;//聚类类别
public:
	CString m_szeps;   //扫描半径
	CString m_szMinpts;//最小点数
	CString m_szClusterNum;//聚类类别
public:
	int m_RadioCluMethod;
	CEdit m_EditEps;
	CComboBox m_ComboMinPts;
	CComboBox m_ComboClusterNum;

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedRadioDbscan();
	afx_msg void OnBnClickedRadioKmeans();
	afx_msg void OnBnClickedOk();
};
