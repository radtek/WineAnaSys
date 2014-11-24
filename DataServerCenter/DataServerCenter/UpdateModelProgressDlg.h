#pragma once


// CUpdateModelProgressDlg 对话框

class CUpdateModelProgressDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CUpdateModelProgressDlg)

public:
	CUpdateModelProgressDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CUpdateModelProgressDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_UPDATEMODELPROGRESS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
