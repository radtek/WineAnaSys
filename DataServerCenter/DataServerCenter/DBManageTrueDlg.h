#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "TrueWineLib.h"
#include "CSVOperator.h"
#include <Windows.h>
#include <time.h>
#include "AddToLibDlg.h"
UINT ThreadAddSpecToLib(LPVOID lpParam);
UINT ThreadUpdatePLSModel(LPVOID lpParam);
// CDBManageTrueDlg 对话框

class CDBManageTrueDlg : public CPropertyPage
{
	DECLARE_DYNAMIC(CDBManageTrueDlg)

public:
	CDBManageTrueDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDBManageTrueDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_DATABASEMANAGE_TRUE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CRect m_rect;
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	int m_cxold;
	int m_cyold;
	CListCtrl m_ListSpectrumToAdd;
	CListCtrl m_ListSpectrunQueryed;
	afx_msg void OnBnClickedButtonOpendatapackageTrue();
	CString m_SpecDataFoldPath;

	CArray<CString,CString> m_ary_SpecFileName;
	CArray<CString,CString> m_ary_SpecFilePath;
	CArray<CString,CString> m_ary_PicturesPath;
	CString m_ExcelInfoPath;
	int m_PicIndex;
//	afx_msg void OnStnDblclickStaticPictureofspectoaddTrue();
	CString m_AlcoholContent;
	CString m_Flavour;
	CString m_Brand;
	afx_msg void OnBnClickedButtonLastTrue();
	afx_msg void OnBnClickedButtonNextTrue();
	CString m_Field1;
	CString m_AndOr1;
	CString m_Field2;
	CString m_AndOr2;
	CString m_Field3;
	CComboBox m_ComboField1;
	CComboBox m_ComboAndOr1;
	CComboBox m_ComboField2;
	CComboBox m_ComboAndOr2;
	CComboBox m_ComboField3;
	afx_msg void OnBnClickedButtonAddtolibTrue();

	CWinThread* pThreadAddSpecToLib;
	CWinThread* pThreadUpdatePLSModel;

	CString m_WineName;
	CString m_ProductionDate;
	CString m_BatchNo;
	CString m_Comfrom;
	CAddToLibDlg *m_pAddToLibDlg;
	void UpdateIndexTable(void);
	afx_msg void OnBnClickedButtonUpdatelibinfo();
	void UpdatePLSModel(void);
	afx_msg void OnBnClickedButtonShowallTrue();
	afx_msg void OnBnClickedButtonDeleteTrue();
	CTrueWineLib m_TrueWineLib;
	void ShowQuerySpecRecord(void);
	afx_msg void OnBnClickedButtonClearspectoaddTrue();
	afx_msg void OnBnClickedButtonUpdatemodelTrue();
	afx_msg void OnBnClickedButtonSearchwithkeywordsTrue();
	CString m_Key1;
	CString m_Key2;
	CString m_Key3;
};
