#pragma once
#include "afxwin.h"


// CWdenSettingDlg �Ի���

class CWdenSettingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CWdenSettingDlg)

public:
	CWdenSettingDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CWdenSettingDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_WDENSETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_wName;//С������
	int m_DecLevel; //�ֽ����
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
