#pragma once


// CDBManageFakeDlg 对话框

class CDBManageFakeDlg : public CPropertySheet
{
	DECLARE_DYNAMIC(CDBManageFakeDlg)

public:
	CDBManageFakeDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDBManageFakeDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_DATABASEMANAGE_FAKE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CRect m_rect;
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
