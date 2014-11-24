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

// ClientView.h : CClientView 类的接口
//

#pragma once
#include "ClientDoc.h"
#include "SpectrumCompareDlg.h"

//SOCKET sClient;

class CClientView : public CView
{
protected: // 仅从序列化创建
	CClientView();
	DECLARE_DYNCREATE(CClientView)

// 特性
public:
	CClientDoc* GetDocument() const;

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
	virtual ~CClientView();
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
	bool isDlgOpen;
public:
	CWnd *m_pCallPrintDlg; //请求打印功能的对话框

	CDialogEx* pDlg;
	CDialogEx* pDlg2;
	CRect rc;
	bool isDlgLogin;
	virtual void OnInitialUpdate();
	bool isDlgAnalasis;
protected:
	afx_msg LRESULT OnUidisplay(WPARAM wParam, LPARAM lParam);
public:
	bool isSpecCompareDlg;
	afx_msg void OnUpdateButtonLogin(CCmdUI *pCmdUI);
	afx_msg void OnButtonLogout();
	afx_msg void OnUpdateButtonNormspecsend(CCmdUI *pCmdUI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	SOCKET m_sClient;
	bool ExitThreadCheckConnect;
	CString m_UserName;
	CString m_PassWord;
	void ConnectServer(void);
	afx_msg void OnClose();
	afx_msg void OnPaint();
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	void CallPrint(void);
    void CallPrint(CWnd *);
	void CallPrintPreview(void);
	void CallPrintPreview(CWnd *);
protected:
	afx_msg LRESULT OnUiselect(WPARAM wParam, LPARAM lParam);
};


#ifndef _DEBUG  // ClientView.cpp 中的调试版本
inline CClientDoc* CClientView::GetDocument() const
   { return reinterpret_cast<CClientDoc*>(m_pDocument); }
#endif

