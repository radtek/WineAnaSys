// SelectSpecFromLibSheet.cpp : 实现文件
//

#include "stdafx.h"
#include "DataServerCenter.h"
#include "SelectSpecFromLibSheet.h"


// CSelectSpecFromLibSheet

IMPLEMENT_DYNAMIC(CSelectSpecFromLibSheet, CPropertySheet)

CSelectSpecFromLibSheet::CSelectSpecFromLibSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{

}

CSelectSpecFromLibSheet::CSelectSpecFromLibSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{

}

CSelectSpecFromLibSheet::~CSelectSpecFromLibSheet()
{
}


BEGIN_MESSAGE_MAP(CSelectSpecFromLibSheet, CPropertySheet)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CSelectSpecFromLibSheet 消息处理程序
void CSelectSpecFromLibSheet::OnShowWindow(BOOL bShow, UINT nStatus)
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