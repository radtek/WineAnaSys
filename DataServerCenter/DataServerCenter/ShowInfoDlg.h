#pragma once


// CShowInfoDlg 对话框

class CShowInfoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CShowInfoDlg)

public:
	CShowInfoDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CShowInfoDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_SHOWINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_StaticInfoShow;
};
