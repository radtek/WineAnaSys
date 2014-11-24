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
}

CDBManageSheet::~CDBManageSheet()
{

}


BEGIN_MESSAGE_MAP(CDBManageSheet, CPropertySheet)
	ON_WM_SHOWWINDOW()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CDBManageSheet 消息处理程序


void CDBManageSheet::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CPropertySheet::OnShowWindow(bShow, nStatus);

	// TODO: 在此处添加消息处理程序代码
}


void CDBManageSheet::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CPropertySheet::OnPaint()
}
