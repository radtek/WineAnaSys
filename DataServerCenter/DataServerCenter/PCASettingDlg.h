#pragma once
#include "afxwin.h"


// CPCASettingDlg �Ի���

class CPCASettingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPCASettingDlg)

public:
	CPCASettingDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPCASettingDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_PCAANASETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	int m_RadioPCnum;   //ѡ����ʾ��PC��

	CComboBox m_ComboXaxisPC; //x����ʾ��PC
	int xPC;

	CComboBox m_ComboYaxisPC; //y����ʾ��PC
	int yPC;

	CComboBox m_ComboPCNum; //��ʾ��PC��
	int PCNum;

	int m_RadioSpecToPCA;

	int m_InitPCNum; //�Ի����ʼ��ʱ��ѡ�������ʾpc����
	//CComboBox m_ComboRefSpecType;   //�ο�ͼ������
	CString m_WineName;
	CString m_BatchNo;

public:
	afx_msg void OnBnClickedRadio2pca();
	afx_msg void OnBnClickedRadioMultipca();
	virtual BOOL OnInitDialog();	
	afx_msg void OnBnClickedRadioPlotedspec();
	afx_msg void OnBnClickedRadioAllspec();	
	afx_msg void OnBnClickedOk();
};
