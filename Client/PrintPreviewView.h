// PrintPreviewView.h: interface for the CPrintPreviewView class.
//
////////////////////////////////////////
#if !defined(AFX_MYPREVIEWVIEW_H__0AE8B670_B1AE_11DA_812E_00E04C39032F__INCLUDED_)
#define AFX_MYPREVIEWVIEW_H__0AE8B670_B1AE_11DA_812E_00E04C39032F__INCLUDED_
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <afxpriv.h>
class CPrintPreviewView : public CPreviewView  
{
DECLARE_DYNCREATE(CPrintPreviewView)
  public:
	afx_msg void OnPreviewClose();
  
  protected:
	CPrintPreviewView();
	virtual ~CPrintPreviewView();
	void OnDraw(CDC* pDC);
	void CPrintPreviewView::OnEndPrintPreview(CDC* pDC, CPrintInfo* pInfo, POINT point, CPreviewView* pView);
	afx_msg void OnPreviewPrint( );
DECLARE_MESSAGE_MAP( )
};
#endif // !defined(AFX_MYPREVIEWVIEW_H__0AE8B670_B1AE_11DA_812E_00E04C39032F__INCLUDED_)