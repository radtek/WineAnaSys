// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面 
// (“Fluent UI”)。该示例仅供参考，
// 用以补充《Microsoft 基础类参考》和 
// MFC C++ 库软件随附的相关电子文档。
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问  
// http://msdn.microsoft.com/officeui。
//
// 版权所有(C) Microsoft Corporation
// 保留所有权利。

// DataServerCenterView.h : CDataServerCenterView 类的接口
//

#pragma once
#include "resource.h"
#include "DataServerCenterDoc.h"
#include "SpectrumCompareDlg.h"
#include "DBManageDlg.h"
#include "ClusterDlg.h"
#include "zipimplement.h" 
#include "ExcelOperator.h"
#include "AlcoholTypeIndex.h"
#include "FlavourTypeIndex.h"
#include "TrueWineLib.h"
#include "BrandTypeIndex.h"
#include "WineTypeIndex.h"
#include "PLSModel.h"
#include "Matrix.h"
#include <map>
using std::map;
#define UIDisplay WM_USER+1
#define ManualAnalasisUIChange WM_USER+2
#define	WM_MY_PRINT			(WM_USER+1003)
#define	WM_BEGIN_PRINTING		(WM_USER+1004)
#define	WM_END_PRINTING			(WM_USER+1005)
#define PRINTMARGIN 2

class CDataServerCenterView : public CView
{
protected: // 仅从序列化创建
	CDataServerCenterView();
	DECLARE_DYNCREATE(CDataServerCenterView)

// 特性
public:
	CDataServerCenterDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CDataServerCenterView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	CRect rc;
	virtual void OnInitialUpdate();
	CWnd* pDlg;
	CWnd * pSpectrumCompareDlg;

	CWnd* pDlgAnaOnlineInfo;

	bool isMainUIDlg;
	bool isSpecCompareDlg;

	SOCKET m_hSocket;



protected:
	afx_msg LRESULT OnUidisplay(WPARAM wParam, LPARAM lParam);
public:
	
	CWinThread* pThreadCheckLibZip;
	CWinThread* pThreadCheckAnaZip;
	CWinThread* pListen;


protected:
	afx_msg LRESULT OnManualanalasisuichange(WPARAM wParam, LPARAM lParam);
public:
	CWnd* pClusterDlg;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnUpdateButtonManualana(CCmdUI *pCmdUI);
	afx_msg void OnUpdateButton2(CCmdUI *pCmdUI);
	afx_msg void OnUpdateButtonSpeccompare(CCmdUI *pCmdUI);
	afx_msg void OnUpdateButtonDbmanage(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCheckTrue(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCheckFake(CCmdUI *pCmdUI);
	int SelectPage;
	afx_msg void OnCheckTrue();
	afx_msg void OnCheckFake();
	afx_msg void OnButtonManualana();
	afx_msg void OnButtonDbmanage();
protected:
	afx_msg LRESULT OnShowspeccompare(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnButtonLineColor();
	afx_msg void OnUpdateButtonLineColor(CCmdUI *pCmdUI);


	afx_msg void OnButtonLineColorDiy();
	afx_msg void OnUpdateOnButtonLineColorDiy(CCmdUI *pCmdUI);
	afx_msg void OnNcDestroy();
	afx_msg void OnButtonBackup();
	afx_msg void OnButtonRestore();
	afx_msg void OnPaint();
//	afx_msg void OnUpdateMaxchartshow(CCmdUI *pCmdUI);
	afx_msg void OnButtonUsermanage();
	afx_msg void OnClose();
	afx_msg void OnButtonDbmanageTrue();
	afx_msg void OnButtonDbmanageFake();
};

#ifndef _DEBUG  // DataServerCenterView.cpp 中的调试版本
inline CDataServerCenterDoc* CDataServerCenterView::GetDocument() const
   { return reinterpret_cast<CDataServerCenterDoc*>(m_pDocument); }
#endif

