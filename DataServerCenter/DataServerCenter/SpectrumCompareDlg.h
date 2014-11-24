#ifndef SPECTRUMCOMPAREDLG_H
#define SPECTRUMCOMPAREDLG_H
#pragma once


// CSpectrumCompareDlg �Ի���
#include "DataServerCenter.h"
#include "Chart.h"
#include "ImportProjectDataProgressDlg.h"
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
#include "CListCtrlEx.h"
#include "ShowInfoDlg.h"
#include "ClusterDlg.h"
using std::list;
#define WM_CHANGESELECTEDCURVE (WM_USER+109)
#define WM_CHANGECHARTMODE (WM_USER+111)
#define WM_BEGINIMPORTDATA (WM_USER+113)
#define WM_ENDIMPORTDATA (WM_USER+112)
#define WM_BEGINCLUSTER (WM_USER+161)
#define WM_ENDCLUSTER (WM_USER+162)


struct SpecInfo
{
	CString m_SpecData;
	CString m_WaveNumber;
	CString m_SpecID;
	CString m_WineName;
	CString m_ComFrom;
	CString m_ImportDate;
	CString m_Manager;
};


class CSpectrumCompareDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSpectrumCompareDlg)

public:
	CSpectrumCompareDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSpectrumCompareDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_SPECCOMPARE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CImportProjectDataProgressDlg *m_pCImportProjectDataProgressDlg;
	CShowInfoDlg *m_pShowInfoDlg;

	CWnd *m_pMFrame;
	CWnd *m_pPrintFrame;
	CFont m_fontPrinter;
	UINT m_cyPrinter;
	UINT m_cxWidth;
	UINT m_nLinesPerPage;
	UINT m_cxOffset;
	UINT m_MaxPage;
public:  // ���ڳ�ʼ�����׷���Ԥ����Ի���
	bool m_IsDenoiseChecked; //���븴ѡ���Ƿ�ѡ��
	bool m_IsCorrChecked;   //У����ѡ���Ƿ�ѡ��
	int m_RadioDenoise;  //����
	int m_RadioCorrelation;//У��	
	int m_SmoothPtsIndex; //ƽ����������
	int m_Wname1Index; //С����1����
	int m_Wname2Index;  //С����2����
	int m_DecLevelIndex; //�ֽ��������
	int m_TTppeIndex;  //��ֵѡ�񷽷�����
	int m_TScalIndex; //��ֵ����������
	int m_TSORHIndex; //��ֵʹ�÷�����
	int m_DerivativeOrderIndex; //������������
	int m_AnaPts;
public:
	int m_SmoothPt; //ƽ������
	CString m_wName;//С������
	int m_DecLevel; //�ֽ����
	int m_DerivativeOrder; //�����״�
	CString m_TTppe;
	CString m_TScal;
	CString m_TSORH;
public:
	CRect m_rect;
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CListCtrlEx m_ListSelectedProject;
	CListCtrlEx m_ListSpecFromLib;
	afx_msg void OnBnClickedButtonSelectspecinlib();

	CString m_projectPath;
	CString m_projectName;

	CArray<CString,CString> m_ArraySpecPath;
	CArray<CString,CString> m_ArraySpecName;
	CArray<CString,CString> m_ArraySpecPreResult;
	CMatrix m_MatrixSpecData; //�����Ŀ�еĹ������ݣ��Ծ�����ʽ	
	CArray<SpecInfo,SpecInfo> m_ArraySpecInfoFromLib;
	CChart m_ChartSpectrumCompare;
	int m_LineWidth;
	COLORREF m_crBackColor;

	bool m_MaxChart;
	bool m_ShowTransAbs; //͸���������
	bool m_bShowtoolbox;
private:
	CFrameWnd*      m_pFrameSplit;    // �ָ�����  
	CSplitterWnd    m_wndSpliter;     // ���ҷָ�  
	bool m_bDragFullWindow;
	HICON m_hIcon;
	CImageList m_Imagelist; //������ͼ����Դ

	CMenu m_Menu;
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	CButton m_btnImportSpec;
	COLORREF m_LineColor;
	COLORREF m_BkgColor;
	afx_msg void OnNMClickListSpectocompare(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickListSpecinlib(NMHDR *pNMHDR, LRESULT *pResult);
	int UpDateCListCtrlEx(void);

	
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
protected:
	afx_msg LRESULT OnEndimportdata(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnBeginimportdata(WPARAM wParam, LPARAM lParam);
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
	//����
	CClusterDlg m_ClusterDlg;
	//�پƿ��е�����
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
	afx_msg void OnShowtoolbox();
	afx_msg void OnUpdateShowtoolbox(CCmdUI *pCmdUI);
protected:
	afx_msg LRESULT OnBegincluster(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEndcluster(WPARAM wParam, LPARAM lParam);
};

#endif