#ifndef SPECTRUMCOMPAREDLG_H
#define SPECTRUMCOMPAREDLG_H
#pragma once


// CSpectrumCompareDlg 对话框
#include "DataServerCenter.h"
#include "Chart.h"
//#error("测试是会导入了。。。")
#include "DataServerCenterView.h"
#include "afxcmn.h"
#include <list>
#include <set>
#include "afxwin.h"
#include "SelectSpecFromLibDlg.h"
#include "PCADlg.h"
#include "PCASettingDlg.h"
#include "TrueWineLib.h"
#include "FakeWineLib.h"
#include "ClusterAnalysis.h"
#include <algorithm>
#include "shadebuttonst.h"
using std::list;
#define WM_CHANGESELECTEDCURVE WM_USER+109
#define WM_CHANGECHARTMODE WM_USER+111
struct SpecInfo
{
	CString m_SpecData;
	CString m_WaveNumber;
	CString m_SpecID;
	CString m_WineName;
	CString m_AlcoholContent;
	CString m_Flavour;
	CString m_Brand;
	CString m_ProductionDate;
	CString m_ProductionBatchNo;
	CString m_ComFrom;
	CString m_ImportDate;
	CString m_Manager;
};


class CSpectrumCompareDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSpectrumCompareDlg)

public:
	CSpectrumCompareDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSpectrumCompareDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_SPECCOMPARE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CWnd *m_pMFrame;
	CWnd *m_pPrintFrame;
	CFont m_fontPrinter;
	UINT m_cyPrinter;
	UINT m_cxWidth;
	UINT m_nLinesPerPage;
	UINT m_cxOffset;
	UINT m_MaxPage;
public:  // 用于初始化光谱分析预处理对话框
	bool m_IsDenoiseChecked; //降噪复选框是否选中
	bool m_IsCorrChecked;   //校正复选框是否选中
	int m_RadioDenoise;  //降噪
	int m_RadioCorrelation;//校正	
	int m_SmoothPtsIndex; //平滑点数索引
	int m_Wname1Index; //小波名1索引
	int m_Wname2Index;  //小波名2索引
	int m_DecLevelIndex; //分解层数索引
	int m_TTppeIndex;  //阈值选择方法索引
	int m_TScalIndex; //阈值处理方法索引
	int m_TSORHIndex; //阈值使用法索引
	int m_DerivativeOrderIndex; //导数阶数索引
	int m_AnaPts;
public:
	int m_SmoothPt; //平滑点数
	CString m_wName;//小波名称
	int m_DecLevel; //分解层数
	int m_DerivativeOrder; //导数阶次
	CString m_TTppe;
	CString m_TScal;
	CString m_TSORH;
public:
	CRect m_rect;
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CListCtrl m_ListSelectedProject;
	CListCtrl m_ListSpecFromLib;
	afx_msg void OnBnClickedButtonSelectspecinlib();

	CString m_projectPath;
	CString m_projectName;

	CArray<CString,CString> m_ArraySpecPath;
	CArray<CString,CString> m_ArraySpecName;
	CArray<CString,CString> m_ArraySpecPreResult;
	CMatrix m_MatrixSpecData; //存放项目中的光谱数据，以矩阵形式

	
	CArray<SpecInfo,SpecInfo> m_ArraySpecInfoFromLib;

	CChart m_ChartSpectrumCompare;
	int m_LineWidth;
	COLORREF m_crBackColor;
	//CTrueWineLib *m_pTrueWineLib;
	bool m_MaxChart;

	bool m_ShowTransAbs; //透过率吸光度

private:
	CFrameWnd*      m_pFrameSplit;    // 分隔窗口  
	CSplitterWnd    m_wndSpliter;     // 左右分隔  
	bool m_bDragFullWindow;
	HICON m_hIcon;
//	CToolBar m_ToolBar;
	//工具栏控件
	CImageList m_Imagelist; //工具栏图标资源

	CMenu m_Menu;
public:
//	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	CButton m_btnImportSpec;
	COLORREF m_LineColor;
	COLORREF m_BkgColor;
	afx_msg void OnNMClickListSpectocompare(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickListSpecinlib(NMHDR *pNMHDR, LRESULT *pResult);
	int UpDateCListCtrl(void);

	
	afx_msg void OnDestroy();
	
	afx_msg void OnPaint();
	CComboBox m_ComboSelectedSpecName;
	afx_msg void OnCbnSelchangeComboSelectedspecname();
protected:
	afx_msg LRESULT OnChangeselectedcurve(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnUpdateMaxchartshow(CCmdUI *pCmdUI);
	afx_msg void OnMaxchartshow();

public:
	CArray<CString,CString> m_ArrayComboBak;

	afx_msg void OnSpecminus();
	afx_msg void OnChangetoabs();
	afx_msg void OnChangetotrans();
	afx_msg void OnDeleteselectedcurve();
	afx_msg void OnDeleteallcurve();
	afx_msg void OnSmoothspec();
	afx_msg void OnShownoise();
	afx_msg void OnSpecmean();
	afx_msg void OnSpecmove();
	afx_msg void OnSpecselect();
	afx_msg void OnSpeczoomin();
	afx_msg void OnSpeczoomout();
	afx_msg void OnPeakarea();
	afx_msg void OnLabel();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	afx_msg void OnDeletelabel();
	afx_msg void OnPcaanalasis();
protected:
	afx_msg LRESULT OnChangechartmode(WPARAM wParam, LPARAM lParam);
protected:
	afx_msg LRESULT OnMyPrint(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnBeginPrinting(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEndPrinting(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnFakewineclustering();
	afx_msg void OnNMDblclkListSpectocompare(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkListSpecinlib(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnPlot();
	afx_msg void OnSpecanapretrsetting();
	afx_msg void OnWden();
	afx_msg void OnDiff();
	afx_msg void OnBnClickedButtonClear();
	CButton m_btnClear;
	afx_msg void OnPrintpreview();
	afx_msg void OnPrint();
	afx_msg void OnClose();
	CButtonST m_btnPlot;
	CButtonST m_btnTrans;
	CButtonST m_btnAbs;
	CButtonST m_btnSelect;
	CButtonST m_btnMove;
	CButtonST m_btnZommin;
	CButtonST m_btnZoomout;
	CButtonST m_btnLabel;
	afx_msg void OnBnClickedButtonPlot();
	afx_msg void OnBnClickedButtonTrans();
	afx_msg void OnBnClickedButtonAbs();
	afx_msg void OnBnClickedButtonSelect();
	afx_msg void OnBnClickedButtonMove();
	afx_msg void OnBnClickedButtonZoomin();
	afx_msg void OnBnClickedButtonZoomout();
	afx_msg void OnBnClickedButtonLabel();
};

#endif