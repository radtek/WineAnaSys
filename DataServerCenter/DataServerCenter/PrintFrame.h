#if !defined(AFX_PrintFRAME_H__3A5D542F_CB2E_45B6_8FCF_089F28D50363__INCLUDED_)
#define AFX_PrintFRAME_H__3A5D542F_CB2E_45B6_8FCF_089F28D50363__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PrintFrame.h : header file
//
//#include "MyPrintPreviewDlg.h"
#include "PrintView.h"
#include "DataServerCenterView.h"
/////////////////////////////////////////////////////////////////////////////
// CPrintFrame frame

class CPrintFrame : public CFrameWnd
{
	DECLARE_DYNCREATE(CPrintFrame)
protected:
	CPrintFrame();           // protected constructor used by dynamic creation
	
// Attributes
public:
	CPrintFrame(CWnd* pOld);
    CWnd* m_pOldWnd; //用于保存主对话框对象
    CPrintView* m_pView;//用于保存视图类对象
	CWnd* m_pOldFram;   //保存原主框架
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPrintFrame)
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CPrintFrame();

	// Generated message map functions
	//{{AFX_MSG(CPrintFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PrintFRAME_H__3A5D542F_CB2E_45B6_8FCF_089F28D50363__INCLUDED_)
