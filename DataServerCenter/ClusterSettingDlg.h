#pragma once
#include "afxwin.h"


// CClusterSettingDlg �Ի���

class CClusterSettingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CClusterSettingDlg)

public:
	CClusterSettingDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CClusterSettingDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_CLUSTERSETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	double m_eps;   //ɨ��뾶
	int m_Minpts;//��С����
	int m_ClusterNum;//�������
public:
	CString m_szeps;   //ɨ��뾶
	CString m_szMinpts;//��С����
	CString m_szClusterNum;//�������
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
