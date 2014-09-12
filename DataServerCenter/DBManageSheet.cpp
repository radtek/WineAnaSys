// DBManageSheet.cpp : 实现文件
//

#include "stdafx.h"
#include "DataServerCenter.h"
#include "DBManageSheet.h"


// CDBManageSheet

IMPLEMENT_DYNAMIC(CDBManageSheet, CPropertySheet)

CDBManageSheet::CDBManageSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{

}

CDBManageSheet::CDBManageSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	//AddPage(&m_DBManagePageTrue);
	//AddPage(&m_DBManagePageFake);
	//this->SetActivePage(iSelectPage);
}

CDBManageSheet::~CDBManageSheet()
{

}


BEGIN_MESSAGE_MAP(CDBManageSheet, CPropertySheet)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CDBManageSheet 消息处理程序


void CDBManageSheet::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CPropertySheet::OnShowWindow(bShow, nStatus);

	// TODO: 在此处添加消息处理程序代码
	/*CRect rect;
	GetParent()->GetClientRect(&rect);
	int nWidth =rect.right-rect.left;
	int nHeight =rect.bottom-rect.top;

	if(bShow)
	{
		GetParent()->ShowWindow(SW_HIDE);
		GetParent()->SetWindowPos(NULL,0,0,nWidth,nHeight,SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE);
		GetParent()->ShowWindow(SW_SHOW);
	}*/
}
