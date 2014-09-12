#pragma once
#include "afxwin.h"
#include "TrueWineLib.h"

// CAddSpecToAnaInfo 对话框

class CAddSpecToAnaInfo : public CDialogEx
{
	DECLARE_DYNAMIC(CAddSpecToAnaInfo)

public:
	CAddSpecToAnaInfo(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAddSpecToAnaInfo();

// 对话框数据
	enum { IDD = IDD_DIALOG_ADDSPECINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_ComboWineName;
	CString m_Winename;
	CComboBox m_ComboAlcoholContent;
	CString m_Alcoholcontent;
	CComboBox m_ComboFlavour;
	CString m_Flavour;
	CComboBox m_ComboBrand;
	CString m_Brand;
	CString m_WinenameExam;
	CString m_AlcoholContentExam;
	CString m_FlavourExam;
	CString m_BrandExam;
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeComboWinename();
	CTrueWineLib Alcohol;
	CTrueWineLib Flavour;
    CTrueWineLib Brand;
    CTrueWineLib WineName;
	afx_msg void OnCbnEditchangeComboWinename();
	afx_msg void OnCbnSelchangeComboAlcoholcentent();
	afx_msg void OnCbnSelchangeComboFlavour();
	afx_msg void OnCbnSelchangeComboBrand();
};
