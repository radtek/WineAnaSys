#pragma once
#include "afxwin.h"


// CCompleteSpecInfoAddToLibDlg 对话框

class CCompleteSpecInfoAddToLibDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCompleteSpecInfoAddToLibDlg)

public:
	CCompleteSpecInfoAddToLibDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCompleteSpecInfoAddToLibDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_COMPLETESPECINFOADDTOLIB };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnSetfocusEditWinename();
	CEdit m_EditWineName;
	CEdit m_EditWineNameInstruction;
	afx_msg void OnEnSetfocusEditAlcoholcontent();
	afx_msg void OnEnSetfocusEditFlavour();
	afx_msg void OnEnSetfocusEditBrand();
	afx_msg void OnEnSetfocusEditProductiondate();
	afx_msg void OnEnSetfocusEditBatchno();
	afx_msg void OnEnSetfocusEditSpecidtitle();
	afx_msg void OnEnSetfocusEditComfrom();
	afx_msg void OnEnSetfocusEditManager();

	CString m_WineName;
	CString m_AlcoholContent;
	CString m_Flavour;
	CString m_Brand;
	CString m_ProductionDate;
	CString m_BatchNo;
	CString m_SpecIDTitle;
	CString m_ComFrom;
	CString m_Manager;

	CString m_WineNameInstruction;
	CString m_AlcoholContentInstruction;
	CString m_FlavourInstruction;
	CString m_BrandInstruction;
	CString m_ProductionDateInstruction;
	CString m_BatchNoInstruction;
	CString m_SpecIDTitleInstruction;
	CString m_ComFromInstruction;
	CString m_ManagerInstruction;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
