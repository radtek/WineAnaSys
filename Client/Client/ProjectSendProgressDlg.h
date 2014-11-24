#pragma once
#include "afxcmn.h"


extern volatile bool ExitThreadPackAndSendProject;
extern volatile bool ThreadPackAndSendProjectExited;
// CProjectSendProgressDlg 对话框

class CProjectSendProgressDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CProjectSendProgressDlg)

public:
	CProjectSendProgressDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CProjectSendProgressDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_PROJECTSENDPROGRESS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
	afx_msg void OnBnClickedCancel();
};
