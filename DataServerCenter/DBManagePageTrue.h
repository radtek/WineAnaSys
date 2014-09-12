#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "TrueWineLib.h"
#include "WinePictures.h"
#include "CSVOperator.h"
#include <Windows.h>
#include <time.h>
#include "AddToLibDlg.h"
// CDBManagePageTrue 对话框
UINT ThreadUpdatePLSModel(LPVOID lpParam);
UINT ThreadAddSpecToLib(LPVOID lpParam);

class CDBManagePageTrue : public CPropertyPage
{
	DECLARE_DYNAMIC(CDBManagePageTrue)

public:
	CDBManagePageTrue();
	virtual ~CDBManagePageTrue();

// 对话框数据
	enum { IDD = IDD_DIALOG_DATABASEMANAGE_TRUE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CRect m_rect;
	virtual BOOL OnInitDialog();
	int m_cxold;
	int m_cyold;
	CListCtrl m_ListSpectrumToAdd;
	CListCtrl m_ListSpectrunQueryed;
	
	CString m_SpecDataFoldPath;

	CArray<CString,CString> m_ary_SpecFileName;
	CArray<CString,CString> m_ary_SpecFilePath;

	CArray<CString,CString> m_ary_PicturesName;
	CArray<CString,CString> m_ary_PicturesPath;


	CArray<CString,CString> m_ary_PicturesQueryedPath;


	CString m_ExcelInfoPath;
	int m_PicIndex;
	int m_PicQueryedIndex;
//	afx_msg void OnStnDblclickStaticPictureofspectoaddTrue();

	afx_msg void OnBnClickedButtonLastTrue();
	afx_msg void OnBnClickedButtonNextTrue();
	afx_msg void OnBnClickedButtonOpendatapackageTrue();
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
	

	CWinThread* pThreadAddSpecToLib;
	CWinThread* pThreadUpdatePLSModel;

	CString m_WineName;
	CString m_AlcoholContent;
	CString m_Flavour;
	CString m_Brand;
	CString m_ProductionDate;
	CString m_BatchNo;
	CString m_Comfrom;
	CString m_SpecIDTitle;
	CString m_Manager;
	CString m_WaveNumber;

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
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedButtonSelectallTrue();
	afx_msg void OnBnClickedButtonInvertselectTrue();
	afx_msg void OnBnClickedButtonClearsearch();
//	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButtonLastlibTrue();
	afx_msg void OnBnClickedButtonNextlibTrue();
	afx_msg void OnNMClickListSpeclibTrue(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonAddtolibTrue();
	afx_msg void OnPaint();
};
