#pragma once
#include "afxwin.h"


// CProjectInfoWriteDlg �Ի���

class CProjectInfoWriteDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CProjectInfoWriteDlg)

public:
	CProjectInfoWriteDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CProjectInfoWriteDlg();

// �Ի�������
	enum { IDD = IDD_PROJECTINFOWRITE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_ProjectName;
	CString m_WineName;
	CString m_AlcoholContent;
	CString m_Flavour;
	CString m_Brand;
	CString m_MailAddr;
	CString m_SenderName;
	CComboBox m_ComboAnaType;
	afx_msg void OnBnClickedButtonSenddata();
	virtual BOOL OnInitDialog();
	CString m_AnaType;
	
};
