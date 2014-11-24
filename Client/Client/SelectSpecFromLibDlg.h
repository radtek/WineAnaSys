#pragma once

#include "afxwin.h"
#include "afxcmn.h"
#include "TrueWineLib.h"
#include "SelectSpecFromLibSheet.h"
#include "SelectSpecFromFakeLibDlg.h"
#include "SelectSpecFromTrueLibDlg.h"
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

	CSelectSpecFromLibSheet    m_SelectSpecFromLibSheet;
	CSelectSpecFromTrueLibDlg  m_SelectSpecFromTrueLibDlg;
	CSelectSpecFromFakeLibDlg  m_SelectSpecFromFakeLibDlg;

	int m_SelPage;
	virtual BOOL OnInitDialog();

	afx_msg void OnPaint();
	afx_msg void OnBnClickedOk();
};

