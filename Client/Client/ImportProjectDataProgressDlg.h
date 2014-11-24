#pragma once
#include "afxcmn.h"

#define WM_PCAIMPORTDATA WM_USER+150
#define WM_PCAPRETREAT WM_USER+151
#define WM_PCACOMPUTE WM_USER+152
#define WM_PCAFINISH WM_USER+153
// CImportProjectDataProgressDlg �Ի���

class CImportProjectDataProgressDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CImportProjectDataProgressDlg)

public:
	CImportProjectDataProgressDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CImportProjectDataProgressDlg();

	// �Ի�������
	enum { IDD = IDD_DIALOG_IMPORTDATA_PROGRESSINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_PCAProgressInfo;
	CProgressCtrl m_PCA_Progress;
protected:
	afx_msg LRESULT OnPcaimportdata(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPcapretreat(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPcacompute(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPcafinish(WPARAM wParam, LPARAM lParam);
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCancel();
};
