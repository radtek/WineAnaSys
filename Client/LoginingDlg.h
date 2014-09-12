#pragma once
#include "shockwaveflash.h"


// CLoginingDlg 对话框

class CLoginingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLoginingDlg)

public:
	CLoginingDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLoginingDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_LOGINING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CShockwaveflash m_shockWaveFlash;
protected:
	afx_msg LRESULT OnPlaylogininganimation(WPARAM wParam, LPARAM lParam);
};
