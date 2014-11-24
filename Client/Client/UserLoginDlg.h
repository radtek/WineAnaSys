#pragma once


// CUserLoginDlg �Ի���
#include "LoginingDlg.h"
extern CLoginingDlg *pCLoginingDlg;

class CUserLoginDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CUserLoginDlg)

public:
	CUserLoginDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CUserLoginDlg();

// �Ի�������
	enum { IDD = IDD_LOGIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	SOCKET m_sock;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CRect m_rect;
	virtual BOOL OnInitDialog();
	//afx_msg void OnBnClickedBtnLogin();
	//afx_msg void OnButtonLogin();
	afx_msg void OnBnClickedExit();
	CString m_UserName;
	CString m_PassWord;
	CWnd *m_pParent;
	afx_msg void OnBnClickedOk();
	afx_msg void OnClose();
	//afx_msg void OnBnClickedButton1();
	bool ConnectServer();
	BOOL m_CheckPW;
	CString m_UserIniFilePath;
};
