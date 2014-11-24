#pragma once

#include "afxwin.h"
#include "afxcmn.h"
#include "FakeWineLib.h"
#include "CListCtrlEx.h"
// CSelectSpecFromLibDlg �Ի���

class CSelectSpecFromFakeLibDlg : public CPropertyPage
{
	DECLARE_DYNAMIC(CSelectSpecFromFakeLibDlg)

public:
	CSelectSpecFromFakeLibDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSelectSpecFromFakeLibDlg();

	// �Ի�������
	enum { IDD = IDD_DIALOG_SELECTSPECFROMLIB_FAKE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_Key;
	CEdit m_key;
	CListCtrlEx m_ListSpec;
	afx_msg void OnBnClickedButtonSearchwithkeywords();
	afx_msg void OnBnClickedButtonShowall();
	virtual BOOL OnInitDialog();
	CFakeWineLib m_FakeWineLib;
	int ShowSpec(void);

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonClearlist();
	afx_msg void OnBnClickedButtonDelete();
};