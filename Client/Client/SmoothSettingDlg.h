#pragma once
#include "afxwin.h"


// CSmoothSettingDlg �Ի���

class CSmoothSettingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSmoothSettingDlg)

public:
	CSmoothSettingDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSmoothSettingDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_SMOOTHSETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	int m_SmoothPts;
public:
	CComboBox m_ComboSmooth;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};

