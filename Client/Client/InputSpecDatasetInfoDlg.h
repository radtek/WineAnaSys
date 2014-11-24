#pragma once


// CInputSpecDatasetInfoDlg 对话框

class CInputSpecDatasetInfoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CInputSpecDatasetInfoDlg)

public:
	CInputSpecDatasetInfoDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CInputSpecDatasetInfoDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_INPUTSPECDATASETINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_WineName;
	CString m_AlcoholContent;
	CString m_Flavour;
	CString m_Brand;
	afx_msg void OnBnClickedOk();
};
