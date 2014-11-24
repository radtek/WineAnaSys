#pragma once
#include "afxeditbrowsectrl.h"


// CDBBackUpAndRestoreDlg �Ի���

class CDBBackUpAndRestoreDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDBBackUpAndRestoreDlg)

public:
	CDBBackUpAndRestoreDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDBBackUpAndRestoreDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_DBBACKUPANDRESTORE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	int m_radioBackupOrRestore;
	CMFCEditBrowseCtrl m_EditBrowseBackup;
	CMFCEditBrowseCtrl m_EditBrowseRestore;
	CString m_BackupOrRestorePath;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedRadioBackup();
	afx_msg void OnBnClickedRadioRestore();
};
