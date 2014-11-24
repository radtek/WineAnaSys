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

// ClientView.h : CClientView ��Ľӿ�
//

#pragma once
#include "ClientDoc.h"
#include "SpectrumCompareDlg.h"

//SOCKET sClient;

class CClientView : public CView
{
protected: // �������л�����
	CClientView();
	DECLARE_DYNCREATE(CClientView)

// ����
public:
	CClientDoc* GetDocument() const;

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
	virtual ~CClientView();
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
	bool isDlgOpen;
public:
	CWnd *m_pCallPrintDlg; //�����ӡ���ܵĶԻ���

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


#ifndef _DEBUG  // ClientView.cpp �еĵ��԰汾
inline CClientDoc* CClientView::GetDocument() const
   { return reinterpret_cast<CClientDoc*>(m_pDocument); }
#endif

