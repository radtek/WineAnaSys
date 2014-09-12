#pragma once
#include "afxcmn.h"
#define WM_DISPLAY WM_USER+3

// CAddToLibDlg �Ի���
//UINT ThreadShowProgress(LPVOID lpParam);


class CAddToLibDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAddToLibDlg)

public:
	CAddToLibDlg(int AddNumber,int CurrentProgress,CWnd* pParent = NULL);   // ��׼���캯��
	//CAddToLibDlg::CAddToLibDlg(CWnd* pParent /*=NULL*/);
	virtual ~CAddToLibDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_ADDTOLIBPROGRESS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
//	CString m_AddProgressInfo;
	CProgressCtrl m_AddToLibProgress;
	int m_AddNumber;
	int m_CurrentProgress;
	virtual BOOL OnInitDialog();
	LRESULT OnDisplay(WPARAM wParam,LPARAM lParam);
	void CAddToLibDlg::OnSize(UINT nType, int cx, int cy);
	CRect m_rect;
	CString m_StaticAddInfo;
};
