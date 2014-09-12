// ��� MFC ʾ��Դ������ʾ���ʹ�� MFC Microsoft Office Fluent �û����� 
// (��Fluent UI��)����ʾ�������ο���
// ���Բ��䡶Microsoft ������ο����� 
// MFC C++ ������渽����ص����ĵ���
// ���ơ�ʹ�û�ַ� Fluent UI ����������ǵ����ṩ�ġ�
// ��Ҫ�˽��й� Fluent UI ��ɼƻ�����ϸ��Ϣ�������  
// http://msdn.microsoft.com/officeui��
//
// ��Ȩ����(C) Microsoft Corporation
// ��������Ȩ����

// DataServerCenterView.h : CDataServerCenterView ��Ľӿ�
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
protected: // �������л�����
	CDataServerCenterView();
	DECLARE_DYNCREATE(CDataServerCenterView)

// ����
public:
	CDataServerCenterDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CDataServerCenterView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
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

#ifndef _DEBUG  // DataServerCenterView.cpp �еĵ��԰汾
inline CDataServerCenterDoc* CDataServerCenterView::GetDocument() const
   { return reinterpret_cast<CDataServerCenterDoc*>(m_pDocument); }
#endif

