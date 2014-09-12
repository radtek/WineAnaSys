#pragma once
#include "afxcmn.h"


// CPredictedRecordDlg 对话框

class CPredictedRecordDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPredictedRecordDlg)

public:
	CPredictedRecordDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPredictedRecordDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_PREDICTEDRECORD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_ListPredictedRecord;
	virtual BOOL OnInitDialog();
};
