#pragma once
#include "atltypes.h"
#include "ClientView.h"

// CAnalasisOfflineDlg �Ի���

class CAnalasisOfflineDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAnalasisOfflineDlg)

public:
	CAnalasisOfflineDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAnalasisOfflineDlg();

// �Ի�������
	enum { IDD = IDD_ANALASISOFFLINE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CRect m_rect;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonBackfromoffline();
};
