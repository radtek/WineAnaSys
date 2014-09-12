#pragma once
#include "atltypes.h"
#include "ClientView.h"

// CAnalasisOfflineDlg 对话框

class CAnalasisOfflineDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAnalasisOfflineDlg)

public:
	CAnalasisOfflineDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAnalasisOfflineDlg();

// 对话框数据
	enum { IDD = IDD_ANALASISOFFLINE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CRect m_rect;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonBackfromoffline();
};
