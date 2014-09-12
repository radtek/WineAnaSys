#pragma once
#include "ClusterChart.h"
#include "afxcmn.h"
#include "FakeWineLib.h"
#include "ClusterAnalysis.h"
#include "afxwin.h"
#include <set>
// CClusterDlg 对话框

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
	CClusterChart m_ClusterChart;
	CListCtrl m_ListTotalFake;
	CListCtrl m_ListMatchedFake;
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
	vector<CString> m_VectorCurveName; //
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
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();

	afx_msg void OnNMDblclkListTotalfake(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkListMatchedfake(NMHDR *pNMHDR, LRESULT *pResult);
};
