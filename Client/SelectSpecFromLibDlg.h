#pragma once

#include "afxwin.h"
#include "afxcmn.h"
#include "TrueWineLib.h"

// CSelectSpecFromLibDlg �Ի���

class CSelectSpecFromLibDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSelectSpecFromLibDlg)

public:
	CSelectSpecFromLibDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSelectSpecFromLibDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_SELECTSPECFROMLIB };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_ComboField1;
	CComboBox m_ComboField2;
	CComboBox m_ComboField3;
	CComboBox m_ComboAndOr1;
	CComboBox m_ComboAndOr2;
	CString m_Key1;
	CString m_Key2;

	CEdit m_key3;
	CListCtrl m_ListSpec;
	afx_msg void OnBnClickedButtonSearchwithkeywords();
	afx_msg void OnBnClickedButtonShowall();
	virtual BOOL OnInitDialog();
	CTrueWineLib m_TrueWineLib;
	int ShowSpec(void);
	CString m_Key3;
	afx_msg void OnBnClickedOk();
};
