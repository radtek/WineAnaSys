#pragma once
#include "afxwin.h"


// CDiffSettingDlg �Ի���

class CDiffSettingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDiffSettingDlg)

public:
	CDiffSettingDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDiffSettingDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_DIFFSETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	int m_Order;
public:
	CComboBox m_ComboOrder;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
