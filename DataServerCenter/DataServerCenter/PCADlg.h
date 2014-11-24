#pragma once
#include "pcachart.h"
#include "PCAResettingDlg.h"

// CPCADlg 对话框
#define WM_PCACHANGECHARTMODE WM_USER+110
class CPCADlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPCADlg)

public:
	CPCADlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPCADlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_PCA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CRect m_rect;
	CImageList m_Imagelist;
	CToolBar m_ToolBar;    //工具栏控件
public:
	CPCAChart m_PCAChart;
	CMenu m_Menu;
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
	CMatrix m_PCAData;
	size_t m_MaxPCNum; //最大主成分数
	size_t m_RefSpecNum; //参考图谱数
	size_t m_AnaSpecNum; //对比图谱数

	size_t m_PCNumShow; //显示的PC
	size_t m_xPC;   //仅适用于pc=2
	size_t m_yPC;   //仅适用于pc=2

	CString m_RefName;

	std::vector<CString> m_VectorRefName;
	std::vector<int> m_VectorEachRefNameCount;

   std::vector<CString> m_VectorCurveName;//对比图谱名称
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
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnPcacommonCursor();
	afx_msg void OnPcamoveCursor();
	afx_msg void OnPcazoominCursor();
	afx_msg void OnPcazoomoutCursor();
protected:
	afx_msg LRESULT OnPcachangechartmode(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMyPrint(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnBeginPrinting(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEndPrinting(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnPcashowtoolbar();
	afx_msg void OnPcaSetting();
	afx_msg void OnPrintpreview();
	afx_msg void OnPrint();
	afx_msg void OnClose();
	CButtonST m_btnSelect;
	CButtonST m_btnMove;
	CButtonST m_btnZommin;
	CButtonST m_btnZoomout;
	afx_msg void OnBnClickedButtonSelect();
	afx_msg void OnBnClickedButtonMove();
	afx_msg void OnBnClickedButtonZoomin();
	afx_msg void OnBnClickedButtonZoomout();

};
