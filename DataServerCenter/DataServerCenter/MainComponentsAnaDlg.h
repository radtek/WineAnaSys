#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "mschart.h"
#include "CListCtrlEx.h"
#include "SpectrumCompareDlg.h"
#include "ClusterDlg.h"
#include "ShowInfoDlg.h"
#include <map>
// CMainComponentsAnaDlg 对话框
#define WM_MY_PRINT (WM_USER+1003)
#define WM_BEGINCLUSTER (WM_USER+161)
#define WM_ENDCLUSTER (WM_USER+162)

class CMainComponentsAnaDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMainComponentsAnaDlg)

public:
	CMainComponentsAnaDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMainComponentsAnaDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_MAINCOMPONENTANALASIS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CImportProjectDataProgressDlg *m_pCImportProjectDataProgressDlg;
	CShowInfoDlg *m_pShowInfoDlg;

	CListCtrl m_ListCtrlAnaSpec;
	CListCtrl m_ListSpecFromLib;
	CListCtrl m_ListCtrlAnaComponents;

	CStatic m_StaticCompareSampleSetting;
	CStatic m_StaticAnaSpec;
	CStatic m_StaticAnaComponent;
	CMSChart m_MsChart;

	CButton m_btnImportSpecFromLib;
	CButton m_btnClearList;


	bool m_bShowAnaComSetting;
	CString m_projectName;
	CString m_projectPath;

	CArray<CString,CString> m_ArraySpecPath;
	CArray<CString,CString> m_ArraySpecName;
	CArray<CString,CString> m_ArraySpecPreResult;
	CArray<SpecInfo,SpecInfo> m_ArraySpecInfoFromLib;

	CMatrix m_MatrixSpecData; //存放项目中的光谱数据，以矩阵形式	

	//CMatrix m_MatrixComponentConcentration;

	int UpDateCListCtrlEx(void);
	CMenu m_Menu;
	CWnd *m_pPrintFrame;
	CRect m_rect;

	CArray<CString,CString> m_ComponentArray;

	std::map<CString,CMatrix> m_MapProSpecNameComponent;
	std::map<CString,CMatrix> m_MapLibSpecIDComponent;

	CMatrix m_Weight;
	CMatrix m_Offset;

public:
	//PCA
	bool isPCA;
	void _PCAanalasis();
	CMatrix TotalSpecData;
	vector<CString> WineNameVector;
	vector<int> EachWineCount;	
	int m_xPC;
	int m_yPC;
	int m_PCNumShow;
public:
	//聚类
	//假酒库中的数据
	CArray<CString,CString> m_ArraySpectrumID;
	CArray<CString,CString> m_ArrayWineNameFaked;
	CArray<CString,CString> m_ArrayComfrom;
	CArray<CString,CString> m_ArrayImportDate;
	CArray<CString,CString> m_ArrayManager;

	void _Fakewineclustering();
	CMatrix TotalDataset;
	CMatrix FakeDataset;
	vector<CString> VectorCurveName;
	int m_RadioCluMethod;
	double m_eps;
	int m_Minpts;
	int m_ClusterNum;

	CClusterDlg m_ClusterDlg;

	virtual BOOL OnInitDialog();
	void InitChart();
	void DrawChartSample();
	void DrawChart(const int &newItem,const bool &bCheck,const bool &bSpecOrComponent,bool bProOrLib = true);

	double m_CurMaxValueShow;

	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnNMClickListComponents(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickListProspectrum(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkListProspectrum(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkListComponents(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnPrint();
	afx_msg void OnPrintpreview();
protected:
	afx_msg LRESULT OnMyPrint(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnAnacomponentsetting();
	afx_msg void OnUpdateAnacomponentsetting(CCmdUI *pCmdUI);
	afx_msg void OnImportspectrum();

	afx_msg void OnBnClickedButtonImportspecfromlib();
	afx_msg void OnBnClickedButtonClearlist();
	afx_msg void OnNMClickListLibsamplesetting(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkListLibsamplesetting(NMHDR *pNMHDR, LRESULT *pResult);
protected:
	afx_msg LRESULT OnBeginimportdata(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEndimportdata(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnPca();
	afx_msg void OnCluster();
protected:
	afx_msg LRESULT OnBegincluster(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEndcluster(WPARAM wParam, LPARAM lParam);
};
