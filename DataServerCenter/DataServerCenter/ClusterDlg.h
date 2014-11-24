#pragma once
#include "ClusterChart.h"
#include "afxcmn.h"
#include "FakeWineLib.h"
#include "ClusterAnalysis.h"
#include "afxwin.h"
#include <set>
#include "ImportProjectDataProgressDlg.h"
#include "CListCtrlEx.h"
#include "ShowInfoDlg.h"

// CClusterDlg 对话框
#define WM_CHANGECHARTMODE WM_USER+110

class CClusterDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CClusterDlg)

public:
	CClusterDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CClusterDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_CLUSTER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:

	CMenu m_Menu;
	CClusterChart m_ClusterChart;
	CListCtrlEx m_ListTotalFake;
	CListCtrlEx m_ListMatchedFake;
	CStatic m_StaticAll;
	CStatic m_StaticSimilarity;

public:
	int m_RadioCluMethod; //聚类类别
	double m_eps;         //扫描半径
	int m_Minpts;         //最小点数
	int m_ClusterNum;     //聚类类别	

public:

	CFakeWineLib m_FakeWineLib;
	CMatrix m_AnaDataSet;
	CMatrix m_TotalDataSet;
	CMatrix m_FakeDataset;
	vector<CString> m_VectorCurveName;
	ClusterAnalysis m_ClusterAnalysis;
	std::set<int> m_AnaSpecClusterID;
	CArray<int,int> m_ArraySimilaNo;

	//假酒库中的数据
	CArray<CString,CString> m_ArraySpectrumID;
	CArray<CString,CString> m_ArrayWineNameFaked;
	CArray<CString,CString> m_ArrayComfrom;
	CArray<CString,CString> m_ArrayImportDate;
	CArray<CString,CString> m_ArrayManager;

	CRect m_rect;
public:
	CWnd *m_pMFrame;
	CWnd *m_pPrintFrame;
	CFont m_fontPrinter;
	UINT m_cyPrinter;
	UINT m_cxWidth;
	UINT m_nLinesPerPage;
	UINT m_cxOffset;
	UINT m_MaxPage;

public:
	//是否显示工具栏
	bool m_ShowToolBar;

	//bool LoadSkin;
	//鼠标状态
	bool m_CursorCommon;
	bool m_CursorMove;
	bool m_CursorZoomIn;
	bool m_CursorZoomOut;

public:
	CButtonST m_btnSelect;
	CButtonST m_btnMove;
	CButtonST m_btnZommin;
	CButtonST m_btnZoomout;

	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();

	afx_msg void OnCommonCursor();
	afx_msg void OnMoveCursor();
	afx_msg void OnZoominCursor();
	afx_msg void OnZoomoutCursor();

	afx_msg void OnNMDblclkListTotalfake(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkListMatchedfake(NMHDR *pNMHDR, LRESULT *pResult);
protected:
	afx_msg LRESULT OnChangechartmode(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMyPrint(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnBeginPrinting(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEndPrinting(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnPrintpreview();
	afx_msg void OnPrint();
	afx_msg void OnBnClickedButtonSelect();
	afx_msg void OnBnClickedButtonMove();
	afx_msg void OnBnClickedButtonZoomin();
	afx_msg void OnBnClickedButtonZoomout();

	afx_msg void OnClose();
	afx_msg void OnClusterSetting();
	void ShowClusterResult();
};
