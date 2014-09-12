#pragma once
#include "ClusterChart.h"
#include "afxcmn.h"
#include "FakeWineLib.h"
#include "ClusterAnalysis.h"
#include "afxwin.h"
#include <set>
// CClusterDlg �Ի���

class CClusterDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CClusterDlg)

public:
	CClusterDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CClusterDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_CLUSTER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CClusterChart m_ClusterChart;
	CListCtrl m_ListTotalFake;
	CListCtrl m_ListMatchedFake;
	CStatic m_StaticAll;
	CStatic m_StaticSimilarity;
public:
	int m_RadioCluMethod; //�������
	double m_eps;         //ɨ��뾶
	int m_Minpts;         //��С����
	int m_ClusterNum;     //�������	

public:
	CFakeWineLib m_FakeWineLib;
	CMatrix m_AnaDataSet;
	CMatrix m_TotalDataSet;
	CMatrix m_FakeDataset;
	vector<CString> m_VectorCurveName; //
	ClusterAnalysis m_ClusterAnalysis;
	std::set<int> m_AnaSpecClusterID;
	CArray<int,int> m_ArraySimilaNo;

	//�پƿ��е�����
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
