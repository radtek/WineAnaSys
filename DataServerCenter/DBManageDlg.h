#pragma once
#include "DBManageSheet.h"

// CDBManageDlg 对话框

class CDBManageDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDBManageDlg)

public:
	CDBManageDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDBManageDlg();

	CDBManageSheet m_CDBManageSheet;
	CDBManagePageTrue m_DBManagePageTrue;
    CDBManagePageFake m_DBManagePageFake;
	int SelectPage;

	CWnd * pwnd; //用于指向“父”对话框
// 对话框数据
	enum { IDD = IDD_DIALOG_DATABASEMANAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
};
