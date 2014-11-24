#pragma once

#include "DBManagePageFake.h"
#include "DBManagePageTrue.h"

// CDBManageSheet

class CDBManageSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CDBManageSheet)

public:
	CDBManageSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CDBManageSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CDBManageSheet();

protected:
	DECLARE_MESSAGE_MAP()
public:
	CDBManagePageFake m_DBManagePageFake;
	CDBManagePageTrue m_DBManagePageTrue;
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnPaint();
};


