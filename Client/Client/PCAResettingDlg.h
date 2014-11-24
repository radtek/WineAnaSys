#pragma once
#include "afxwin.h"


// CPCAResettingDlg �Ի���

class CPCAResettingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPCAResettingDlg)

public:
	CPCAResettingDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPCAResettingDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_PCARESETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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

