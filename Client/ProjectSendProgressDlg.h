#pragma once
#include "afxcmn.h"



// CProjectSendProgressDlg �Ի���

class CProjectSendProgressDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CProjectSendProgressDlg)

public:
	CProjectSendProgressDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CProjectSendProgressDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_PROJECTSENDPROGRESS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_SendState;
	CString m_SendPercent;
	CString m_ProjectSize;
	CString m_SendedSize;
	CProgressCtrl m_ProgressProjectSend;

	int m_CurrentProgress;
	int m_MaxRange;
protected:
	afx_msg LRESULT OnShowsendprogress(WPARAM wParam, LPARAM lParam);
public:
	virtual BOOL OnInitDialog();
};
