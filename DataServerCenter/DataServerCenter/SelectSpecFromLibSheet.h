#pragma once

#include "SelectSpecFromFakeLibDlg.h"
#include "SelectSpecFromTrueLibDlg.h"
// CSelectSpecFromLibSheet

class CSelectSpecFromLibSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CSelectSpecFromLibSheet)

public:
	CSelectSpecFromLibSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CSelectSpecFromLibSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CSelectSpecFromLibSheet();

protected:
	DECLARE_MESSAGE_MAP()
public:
	CSelectSpecFromFakeLibDlg m_SelectSpecFromFakeLibDlg;
	CSelectSpecFromTrueLibDlg m_SelectSpecFromTrueLibDlg;
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};


