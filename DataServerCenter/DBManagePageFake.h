#pragma once

#include "CSVOperator.h"
#include "ExcelOperator.h"
#include "FakeWineLib.h"
#include "AddToLibDlg.h"
#include "WinePictures.h"
#include "CompleteSpecAddToFakeLibInfoDlg.h"
#include "afxcmn.h"
#include "afxwin.h"
UINT ThreadAddSpecToFakeLib(LPVOID lpParam);
// CDBManagePageFake 对话框

class CDBManagePageFake : public CPropertyPage
{
	DECLARE_DYNAMIC(CDBManagePageFake)

public:
	CDBManagePageFake();
	virtual ~CDBManagePageFake();

// 对话框数据
	enum { IDD = IDD_DIALOG_DATABASEMANAGE_FAKE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonOpendatapackageFake();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	CRect m_rect;
	afx_msg void OnBnClickedButtonSearchwithkeywordsFake();
	afx_msg void OnBnClickedButtonShowallFake();
public:
	CFakeWineLib m_FakeWineLib;

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
	
	//CString m_Field1;
	//CString m_AndOr1;
	//CString m_Field2;
	//CString m_AndOr2;
	//CString m_Field3;

	CAddToLibDlg *m_pAddToLibDlg;
	CWinThread* pThreadAddSpecToLib;
	//CWinThread* pThreadUpdatePLSModel;

	CString m_WineName;
	//CString m_AlcoholContent;
	//CString m_Flavour;
	//CString m_Brand;
	//CString m_ProductionDate;
	//CString m_BatchNo;
	CString m_Comfrom;
	CString m_SpecIDTitle;
	CString m_Manager;
	CString m_WaveNumber;
	CString m_TraceToSourceInfo;

	CString m_KeyWords;

public:
	void ShowQuerySpecRecord();
	//void UpdateIndexTable();



	afx_msg void OnBnClickedButtonClearspectoaddFake();
	
	
	afx_msg void OnBnClickedButtonAddtolibFake();
	afx_msg void OnBnClickedButtonSelectallFake();
	afx_msg void OnBnClickedButtonInvertselectFake();
	afx_msg void OnBnClickedButtonClearsearchFake();
	afx_msg void OnBnClickedButtonDeleteFake();
	afx_msg void OnBnClickedButtonUpdatelibinfoFake();
	afx_msg void OnBnClickedButtonLastFake();
	afx_msg void OnBnClickedButtonNextFake();
	afx_msg void OnBnClickedButtonLastlibFake();
	afx_msg void OnBnClickedButtonNextlibFake();
	afx_msg void OnPaint();
	afx_msg void OnNMClickListSpeclibFake(NMHDR *pNMHDR, LRESULT *pResult);
	
};
